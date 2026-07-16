#ifndef __BOULDER_MASTER_DRONECAN_H
#define __BOULDER_MASTER_DRONECAN_H


#include <master/can.h>

#include <dsdl/include/dronecan_msgs.h>


void master_dronecan_handle_get_node_info( __STATE CanardInstance   *canard,
										   __IN    CanardRxTransfer *trx );


#endif // ! __BOULDER_MASTER_DRONECAN_H
