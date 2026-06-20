#ifndef __BOULDER_ATMEL_SAM3X8E_CAN_H
#define __BOULDER_ATMEL_SAM3X8E_CAN_H


#include <libsam/include/can.h>

#include <common/config.h>

#include <stddef.h>


#define SAM3X8E_CAN_MBOX_COUNT 8


typedef struct _can_soc_state_t {
	Can           *iface;
	uint32_t       periph_id;
	can_mb_conf_t  mbox_conf[ SAM3X8E_CAN_MBOX_COUNT ];
	size_t         tx_mbox_count;
} can_soc_state_t;


#ifdef CONFIG_SAM3X8E_USE_CAN1
#   define CAN_PIO_BUS    PIOB
#   define CAN_PIO_MASK   ( PIO_PB15A_CANRX1 | PIO_PB14A_CANTX1 )
#   define CAN_PERIPH_ID  ID_CAN1
#else
#   define CAN_PIO_BUS    PIOA
#   define CAN_PIO_MASK   ( PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0 )
#   define CAN_PERIPH_ID  ID_CAN0
#endif


#endif // ! __BOULDER_ATMEL_SAM3X8E_CAN_H
