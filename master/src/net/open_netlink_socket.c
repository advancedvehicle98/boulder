#include <master/net.h>

#include <common/defines.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include <unistd.h>


// см https://github.com/lalten/libsocketcan/blob/b464485031b6f2a4e53d3ef1b3d405f9ba159c07/src/libsocketcan.c#L910

// про netlink
// https://docs.kernel.org/userspace-api/netlink/intro.html


int32_t
master_net_open_netlink_socket( void )
{
	const char *log_prefix = "master_net_open_netlink_socket";
	
	uint32_t fd;
	int32_t status;

	// открываем netlink-сокет ---------------------------------------------------------------
	{ 
		fd = socket( AF_NETLINK, SOCK_RAW, NETLINK_ROUTE );

		if ( _unlikely( fd < 0 ) ) {
			printf( "%s: Не удалось открыть netlink-сокет", log_prefix );
			return -1;
		}
	}

	// настраиваем буфера для получения/отправки на 32 кб ------------------------------------
	{
		bool configuration_error_occured = false;
		int send_buffer, receive_buffer;
		
		send_buffer = receive_buffer = 32768;
		
		status = setsockopt( fd, SOL_SOCKET, SO_SNDBUF, (void *) &send_buffer, sizeof( int ) );
		configuration_error_occured |= status < 0;
		
		status = setsockopt( fd, SOL_SOCKET, SO_RCVBUF, (void *) &receive_buffer, sizeof( int ) );
		configuration_error_occured |= status < 0;

		if ( _unlikely( configuration_error_occured ) ) {
			printf( "%s: Не удалось настроить буферы на netlink-сокете", log_prefix );
			goto _master_net_open_netlink_socket_defer_close_socket;
		}
	}
	
	{	
		struct sockaddr_nl address;
		// если всё по нулям, то ядро само присваивает nl_pid
		memset( &address, 0, sizeof( struct sockaddr_nl ) );
		
		address.nl_family = AF_NETLINK;
		
		status = bind( fd, (struct sockaddr *) &address, sizeof( struct sockaddr_nl ) );
		
		if ( _unlikely( status < 0 ) ) {
			printf( "%s: Не удалось привязать netlink-сокет", log_prefix );
			goto _master_net_open_netlink_socket_defer_close_socket;
		}

		uint32_t address_length = sizeof( address );

		status = getsockname( fd, (struct sockaddr *) &address, &address_length);

		bool is_wrong_socket = status < 0
			                || address_length != sizeof( address )
			                || address.nl_family != AF_NETLINK;
		
		if ( _unlikely( is_wrong_socket ) ) {
			printf( "%s: Netlink-сокет привязан к неправильному адресу", log_prefix );
			goto _master_net_open_netlink_socket_defer_close_socket;
		}
	}
	
	return fd;

_master_net_open_netlink_socket_defer_close_socket:
	close( fd );
	
	return -1;
}
