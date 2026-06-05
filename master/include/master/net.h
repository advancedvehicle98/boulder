#ifndef __BOULDER_MASTER_NET_H
#define __BOULDER_MASTER_NET_H


#include <common/defines.h>

#include <linux/if_link.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <stdint.h>


#define MASTER_NET_SET_LINKINFO_BUFFER_LEN 1024
#define MASTER_NET_TRY_SET_LINKINFO_BUFFER_LEN 16*1024

// см. https://github.com/lalten/libsocketcan/blob/b464485031b6f2a4e53d3ef1b3d405f9ba159c07/src/libsocketcan.c
#define NLMSG_TAIL( MSG ) \
	( (struct rtattr *) \
	    ( (void *) MSG \
		  + NLMSG_ALIGN( ( MSG )->nlmsg_len ) ) )


typedef struct _set_linkinfo_t {
	struct nlmsghdr  header;
	struct ifinfomsg if_info;
	
	char buffer[ MASTER_NET_SET_LINKINFO_BUFFER_LEN ];
} set_linkinfo_t;


typedef enum {
	NET_TRY_SET_LINKINFO_SUCCESS,
	NET_TRY_SET_LINKINFO_RFNETLINK_UNAVAILABLE,
	NET_TRY_SET_LINKINFO_TRUNCATED_NLMSG_ERROR,
	NET_TRY_SET_LINKINFO_MALFORMED_MESSAGE,
	NET_TRY_SET_LINKINFO_RECEIVE_ERROR
} try_set_linkinfo_error_t;


// net/open_netlink_socket.c
int32_t master_net_open_netlink_socket( void );
// net/try_set_linkinfo.c
try_set_linkinfo_error_t master_net_try_set_linkinfo( __IN const int32_t          fd,
													  __IN       struct nlmsghdr *h );
void master_net_try_set_linkinfo_print_error( __IN const try_set_linkinfo_error_t e );


#endif // ! __BOULDER_MASTER_NET_H
