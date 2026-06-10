#ifndef __BOULDER_ATMEL_SAM3X8E_CAN_H
#define __BOULDER_ATMEL_SAM3X8E_CAN_H


#include <libsam/include/can.h>

#include <stddef.h>


#define SAM3X8E_CAN_MBOX_COUNT 8


typedef struct _can_soc_state_t {
	Can           *iface;
	uint32_t       periph_id;
	can_mb_conf_t  mbox_conf[ SAM3X8E_CAN_MBOX_COUNT ];
	size_t         tx_mbox_count;
} can_soc_state_t;


#endif // ! __BOULDER_ATMEL_SAM3X8E_CAN_H
