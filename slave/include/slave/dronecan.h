#ifndef __BOULDER_SLAVE_DRONECAN_H
#define __BOULDER_SLAVE_DRONECAN_H


#include <canard.h>
#include <dsdl/include/dronecan_msgs.h>

#include <slave/can.h>


void slave_can_handle_get_node_info( __STATE CanardInstance   *canard,
									 __IN    CanardRxTransfer *rx );


#endif // ! __BOULDER_SLAVE_DRONECAN_H
