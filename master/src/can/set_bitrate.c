#include <master/can.h>

#include <common/defines.h>

#include <master/net.h>

#include <linux/can/netlink.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


// здесь слизано с libsocketcan
// см https://github.com/lalten/libsocketcan/blob/b464485031b6f2a4e53d3ef1b3d405f9ba159c07/src/libsocketcan.c#L910


static void _addattr( __STATE       struct nlmsghdr *hdr,
					  __IN    const size_t           max_len,
					  __IN    const int32_t          type,
					  __IN    const void            *data,
					  __IN    const size_t           data_len );


can_set_bitrate_error_t
master_can_set_bitrate( __IN const uint32_t if_index,
					    __IN const uint32_t bitrate )
{
	const char *log_prefix = "master_can_set_bitrate";
	
	int32_t fd;
	can_set_bitrate_error_t error = CAN_SET_BITRATE_SUCCESS;
	struct can_bittiming bittiming;

	// открываем netlink-сокет ------------------------------------------------------------------
	
	{
		fd = master_net_open_netlink_socket();
		if ( _unlikely( fd < 0 ) ) return CAN_SET_BITRATE_NO_SOCKET;
	}

	// это потом скормим в linkinfo ----------------------------------------------------------
	
	{
		memset( &bittiming, 0, sizeof( struct can_bittiming ) );
		bittiming.bitrate = bitrate;
	}
	
	// отправляем запрос на изменение битрейта -------------------------------------------------
	
	{
		set_linkinfo_t slinfo;
		memset( &slinfo, 0, sizeof( set_linkinfo_t ) );

		{
			slinfo.header.nlmsg_len   = NLMSG_LENGTH( sizeof( struct ifinfomsg ) );
			slinfo.header.nlmsg_type  = RTM_NEWLINK;
			slinfo.header.nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK;

			slinfo.if_info.ifi_family = 0;
			slinfo.if_info.ifi_index  = if_index;
		}
		
		{
			struct rtattr *linkinfo = NLMSG_TAIL( &slinfo.header );

			_addattr( &slinfo.header, sizeof( set_linkinfo_t ), IFLA_LINKINFO,  NULL,  0 );
			_addattr( &slinfo.header, sizeof( set_linkinfo_t ), IFLA_INFO_KIND, "can", 3 );

			struct rtattr *data = NLMSG_TAIL( &slinfo.header );

			_addattr( &slinfo.header, sizeof( set_linkinfo_t ), IFLA_INFO_DATA, NULL, 0 );
		
			_addattr( &slinfo.header, 1024, IFLA_CAN_BITTIMING, &bittiming, sizeof( struct can_bittiming ) );
		
			data->rta_len     = (void *) NLMSG_TAIL( &slinfo.header ) - (void *) data;
			linkinfo->rta_len = (void *) NLMSG_TAIL( &slinfo.header ) - (void *) linkinfo;
		}
		
		{
			try_set_linkinfo_error_t try_set_linkinfo_status =
				master_net_try_set_linkinfo( fd, &slinfo.header );

			if ( _unlikely( try_set_linkinfo_status != NET_TRY_SET_LINKINFO_SUCCESS ) ) {
				error = CAN_SET_BITRATE_FAILED_LINKINFO_SET;
				master_net_try_set_linkinfo_print_error( try_set_linkinfo_status );
			}
		}
	}

_master_can_set_bitrate_defer_close_socket:
	close( fd );
	
	return error;
}


void
master_can_set_bitrate_print_error( __IN const can_set_bitrate_error_t e )
{
	const char *log_prefix = "master_can_set_bitrate";
	const char *message = NULL;

	switch ( e ) {
		
	case CAN_SET_BITRATE_NO_SOCKET:
		message = "Не удалось создать netlink-сокет"; break;
		
	case CAN_SET_BITRATE_FAILED_LINKINFO_SET:
		message = "Не удалось настроить linkinfo"; break;
				
	}
	
	printf( "%s: %s\n", log_prefix, message );
}


void
_addattr( __STATE       struct nlmsghdr *hdr,
		  __IN    const size_t           max_len,
		  __IN    const int32_t          type,
		  __IN    const void            *data,
		  __IN    const size_t           data_len )
{
	int32_t len = RTA_LENGTH( data_len );
	struct rtattr *rta = NLMSG_TAIL( hdr );

	rta->rta_type = type;
	rta->rta_len  = len;

	memcpy( RTA_DATA( rta ), data, data_len );

	hdr->nlmsg_len = NLMSG_ALIGN( hdr->nlmsg_len ) + RTA_ALIGN( len );
}
