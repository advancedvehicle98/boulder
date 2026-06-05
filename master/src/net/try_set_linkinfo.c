#include <master/net.h>

#include <common/defines.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/uio.h>


try_set_linkinfo_error_t
master_net_try_set_linkinfo( __IN const int32_t          fd,
							 __IN       struct nlmsghdr *hdr )
{
	int32_t status;
	struct sockaddr_nl address;
	struct nlmsghdr *hdr_aux;

	struct iovec iovec = {
		.iov_base = (void *) hdr,
		.iov_len  = hdr->nlmsg_len
	};

	struct msghdr message = {
		.msg_name    = &address,
		.msg_namelen = sizeof( struct sockaddr_nl ),
		.msg_iov     = &iovec,
		.msg_iovlen  = 1
	};

	{
		memset( &address, 0, sizeof( struct sockaddr_nl ) );
		address.nl_family = AF_NETLINK;
	}

	{
		hdr->nlmsg_seq    = 0;
		hdr->nlmsg_flags |= NLM_F_ACK;
	}

	// пробуем чето отправить ------------------------------------------
	
	{
		status = sendmsg( fd, &message, 0 );
		if ( _unlikely( status < 0 ) ) return NET_TRY_SET_LINKINFO_RFNETLINK_UNAVAILABLE;
	}

	{
		char buffer[ MASTER_NET_TRY_SET_LINKINFO_BUFFER_LEN ];
		
		iovec.iov_base = buffer;

		while ( true ) {
			iovec.iov_len = sizeof( buffer );

			status = recvmsg( fd, &message, 0 );

			hdr_aux = (struct nlmsghdr *) buffer;

			while ( (size_t) status >= sizeof( struct nlmsghdr ) ) {
				size_t total_len = hdr_aux->nlmsg_len;
				int32_t len = total_len - sizeof( struct nlmsghdr );

				bool is_malformed = len < 0 || total_len > (size_t) status;
				
				if ( _unlikely( is_malformed ) ) return NET_TRY_SET_LINKINFO_MALFORMED_MESSAGE;

				if ( _likely( hdr_aux->nlmsg_type != NLMSG_ERROR ) ) {
					status -= NLMSG_ALIGN( total_len );
					hdr_aux = (struct nlmsghdr *) ( (uint8_t *) hdr_aux
													+ NLMSG_ALIGN( total_len ) );
					continue;
				}

				struct nlmsgerr *error_message = (struct nlmsgerr *) NLMSG_DATA( hdr_aux );

				bool is_truncated_error = (size_t) len < sizeof( struct nlmsgerr );
				
				if ( _unlikely( is_truncated_error ) )
					return NET_TRY_SET_LINKINFO_TRUNCATED_NLMSG_ERROR;

				if ( ! error_message->error ) return NET_TRY_SET_LINKINFO_SUCCESS;

				printf( "master_net_try_set_linkinfo: %d", -error_message->error );
				return NET_TRY_SET_LINKINFO_RECEIVE_ERROR;
			}
		}
	}
	
	return NET_TRY_SET_LINKINFO_SUCCESS;
}


void
master_net_try_set_linkinfo_print_error( __IN const try_set_linkinfo_error_t e )
{
	const char *log_prefix = "master_net_try_set_linkinfo";
	const char *message = NULL;

	switch ( e ) {
		
	case NET_TRY_SET_LINKINFO_RFNETLINK_UNAVAILABLE:
		message = "Невозможно связаться с rfnetlink"; break;
		
	case NET_TRY_SET_LINKINFO_TRUNCATED_NLMSG_ERROR:
		message = "Сообщение некорректного формата"; break;
		
	case NET_TRY_SET_LINKINFO_MALFORMED_MESSAGE:
		message = "Обрезанное сообщение NLMSG_ERROR"; break;
		
	case NET_TRY_SET_LINKINFO_RECEIVE_ERROR:
		message = "Ошибка при получении (см. выше)"; break;
		
	}
	
	printf( "%s: %s\n", log_prefix, message );
}
