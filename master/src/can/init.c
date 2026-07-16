#include <master/can.h>

#include <common/defines.h>
#include <common/config.h>

#include <fcntl.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


static void _handle_socket_error( const int socket_error );


// большая часть слизана с ardupilot
// как говориться, monkey see, monkey do

// см. https://github.com/ArduPilot/ardupilot/blob/3e40de50b1f0bf3203a48a7e79e79517a80dda12/libraries/AP_HAL_Linux/CANSocketIface.cpp

can_init_error_t
master_can_init( __STATE       can_state_t      *s,
				 __IN    const can_state_args_t *args )
{	
	if ( s->ready ) return CAN_INIT_SUCCESS;

	can_init_error_t error;
	int32_t status;
	int32_t fd;
	uint32_t bitrate;
	
	struct ifreq ifr;
	memset( &ifr, 0, sizeof( struct ifreq ) );
	
	const char *if_name = args->if_name
		                ? args->if_name
		                : master_can_default_if_name;
	
	// открывает сокет --------------------------------------------------
	
	fd = socket( PF_CAN, SOCK_RAW, CAN_RAW );
	if ( _unlikely( fd < 0 ) ) return CAN_INIT_ERROR_NO_SOCKET;
	
	// тащим информацию об интерфейсе ------------------------------------

	// пока это надо чисто штобы получить номер инетрфейса
	// это нужно будет для привязки сокета к CAN (см. следующий шаг)
	
	strncpy( ifr.ifr_name, if_name, MASTER_CAN_IF_NAME_LEN );

	status = ioctl( fd, SIOCGIFINDEX, &ifr );

	if ( _unlikely( status != EXIT_SUCCESS ) ) {
		error = CAN_INIT_ERROR_IOCTL;
		goto _master_can_init_defer_close_socket;
	}

	if ( _unlikely( ifr.ifr_ifindex < 0 ) ) {
		error = CAN_INIT_ERROR_WRONG_IFR_INDEX;
		goto _master_can_init_defer_close_socket;
	}

	// привязываем сокет к CAN -----------------------------------------

	struct sockaddr_can address;

	address.can_family = AF_CAN;
	address.can_ifindex = ifr.ifr_ifindex;

	status = bind( fd, (struct sockaddr *) &address, sizeof( struct sockaddr_can ) );

	if ( _unlikely( status < 0 ) ) {
		error = CAN_INIT_ERROR_CANNOT_BIND;
		goto _master_can_init_defer_close_socket;
	}
	
	// настройка сокета --------------------------------------------------
	
#define _ENABLE &enable, sizeof( enable )
	
	const int32_t enable = 1;
	bool configuration_error_occured = false;

	// включаем учёт времени прибытия сообщений
	// (хз как по русски красиво сказать timestamp)
	status = setsockopt( fd, SOL_SOCKET, SO_TIMESTAMP, _ENABLE );
	configuration_error_occured |= status < 0;

	// разрешаем самим себе отправлять сообщения
	status = setsockopt( fd, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, _ENABLE );
	configuration_error_occured |= status < 0;

	// делаем сокет неблокирующим
	status = fcntl( fd, F_SETFL, O_NONBLOCK );
	configuration_error_occured |= status < 0;
		
	if ( _unlikely( configuration_error_occured ) ) {
		error = CAN_INIT_ERROR_FAILED_CONFIGURATION;
		goto _master_can_init_defer_close_socket;
	}
	
#undef _ENABLE
	
	// проверяем ошибки с сокета ------------------------------------------------

	int32_t socket_error = 0;
	socklen_t socket_error_len = sizeof( socket_error );

	getsockopt( fd, SOL_SOCKET, SO_ERROR, &socket_error, &socket_error_len );
		
	if ( _unlikely( socket_error ) ) {
		_handle_socket_error( socket_error );
		error = CAN_INIT_ERROR_FAILED_SOCKET_VALIDATION;
		goto _master_can_init_defer_close_socket;
	}
	
	// настройка битрейта -----------------------------------------

	bitrate = args->bitrate ? args->bitrate : CONFIG_CAN_BITRATE_KBPS;
	bitrate *= 1000; // перевод в Гц
		
	can_set_bitrate_error_t bitrate_status = master_can_set_bitrate( ifr.ifr_ifindex, bitrate );

	if ( bitrate_status != CAN_SET_BITRATE_SUCCESS ) {
		error = CAN_INIT_ERROR_FAILED_BAUDRATE_SET;
		master_can_set_bitrate_print_error( bitrate_status );
		goto _master_can_init_defer_close_socket;
	}

	// настройка canard ------------------------------------------

	canardInit( &( s->canard ),
				s->canard_pool, CONFIG_CANARD_MASTER_POOL_SIZE,
				master_can_on_receive,
				master_can_should_accept_transfer,
				s );

	canardSetLocalNodeID( &( s->canard ), CONFIG_DRONECAN_MASTER_ID );
	
	// присвание говна идёт строго в конце после всего остального
	// когда мы точно знаем, что с полями всё будет в порядке
	
	strncpy( s->if_name, if_name, MASTER_CAN_IF_NAME_LEN );
		
	s->bitrate   = bitrate;
	s->if_index  = ifr.ifr_ifindex;
	s->socket_fd = fd;
	
	s->ready = true;
	
	return CAN_INIT_SUCCESS;

_master_can_init_defer_close_socket:
	close( fd );
	
	return error;
}


void
master_can_init_print_error( __IN const can_init_error_t e )
{
	const char *error_msg = NULL;
	const char *log_prefix = "master_can_init";
	
	switch ( e ) {

	case CAN_INIT_ERROR_NO_SOCKET:
		error_msg = "Не удалось открыть сокет"; break;

	case CAN_INIT_ERROR_CANNOT_BIND:
		error_msg = "Не удалось привязать сокет к CAN-интерфейсу"; break;

	case CAN_INIT_ERROR_IOCTL:
		// честно хз, стоит ли оставлять в таком виде
		error_msg = "Неизвестная ошибка, связанная с ioctl (скорее всего указанный интерфейс не существует)"; break;

	case CAN_INIT_ERROR_WRONG_IFR_INDEX:
		error_msg = "Некорректный индекс CAN-интерфейса (скорее всего указанный интерфейс не существует)"; break;

	case CAN_INIT_ERROR_FAILED_CONFIGURATION:
		error_msg = "Не удалось настроить сокет"; break;

	case CAN_INIT_ERROR_FAILED_BAUDRATE_SET:
		error_msg = "Не удалось настроить битрейт на интерфейсе"; break;

	default:
		
	}

	printf( "%s: %s\n", log_prefix, error_msg );
}



void
_handle_socket_error( const int socket_error )
{
	// пока так
	printf( "Ошибка на сокете: %d\n", socket_error );
}
