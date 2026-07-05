#ifndef __BOULDER_ATMEL_SAM3X8E_CAN_H
#define __BOULDER_ATMEL_SAM3X8E_CAN_H


#include <libsam/include/can.h>

#include <common/config.h>

#include <stddef.h>


#define SAM3X8E_CAN_MBOX_COUNT 8


#if CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT < CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT_MIN

#warning "TX mailbox count (CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT) subceeds the minimum count value. Changing it to the minimum allowed value"

#undef CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT
#define CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT_MIN

#elsif CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT > CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT_MAX

#warning "TX mailbox count (CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT) exceeds the maximum count value. Changing it to the maximum allowed value"

#undef CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT
#define CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT CONFIG_SAM3X8E_CAN_TX_MBOX_COUNT_MAX

#endif


typedef struct _can_soc_state_t {
	can_mb_conf_t  mbox_conf[ SAM3X8E_CAN_MBOX_COUNT ];
} can_soc_state_t;


#ifdef CONFIG_SAM3X8E_USE_CAN1
#   define CAN_IRQ        CAN1_IRQn
#   define CAN_IF         CAN1
#   define CAN_PIO_BUS    PIOB
#   define CAN_PIO_MASK   ( PIO_PB15A_CANRX1 | PIO_PB14A_CANTX1 )
#   define CAN_PERIPH_ID  ID_CAN1
#else
#   define CAN_IRQ        CAN0_IRQn
#   define CAN_IF         CAN0
#   define CAN_PIO_BUS    PIOA
#   define CAN_PIO_MASK   ( PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0 )
#   define CAN_PERIPH_ID  ID_CAN0
#endif


#endif // ! __BOULDER_ATMEL_SAM3X8E_CAN_H
