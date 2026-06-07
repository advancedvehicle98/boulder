#ifndef __BOULDER_SLAVE_CHIPS_SAM3X8E_CAN_H
#define __BOULDER_SLAVE_CHIPS_SAM3X8E_CAN_H


#include <common/defines.h>

#include <stdint.h>


#define SAM3X8E_CAN_MBOX_COUNT 8

typedef struct _can_mbox_t {
	__IO  uint32_t mmr;  // mailbox mode register
	__IO  uint32_t mam;  // mailbox acceptance mask register
	__IO  uint32_t mid;  // mailbox id register
	__IN  uint32_t mfid; // mailbox family register 
	__IN  uint32_t msr;  // mailbox status register
	__IO  uint32_t mdl;  // mailbox data low register
	__IO  uint32_t mdh;  // mailbox data high register 
	__OUT uint32_t mcr;  // mailbox control register
} can_mbox_t;

_assert( sizeof( can_mbox_t ) == 0x20 );


typedef struct _can_t {
	__IO  uint32_t mr;      // mode register
	__OUT uint32_t ier;     // interrupt enable register
	__OUT uint32_t idr;     // interrupt disable register
	__IN  uint32_t imr;     // interrupt mask register
	__IN  uint32_t sr;      // status register 
	__IO  uint32_t br;      // baudrate register 
	__IN  uint32_t tim;     // timer register 
	__IN  uint32_t timestp; // timestamp register 
	__IN  uint32_t ect;     // error counter register
	__OUT uint32_t tcr;     // transfer command register
	__OUT uint32_t acr;     // abort command register
	
	uint32_t _0[ ( 0xE0 + 0x4 - 0x2C )/4 ];
	
	__IO  uint32_t wpmr;    // write protect mode register
	__IN  uint32_t wpsr;    // write protect status register
	
	uint32_t _1[ ( 0x1FC + 0x4 - 0xEC )/4 ];

	can_mbox_t mbox[ SAM3X8E_CAN_MBOX_COUNT ];
} can_t;

_assert( sizeof( can_t ) == 0x200 + SAM3X8E_CAN_MBOX_COUNT*sizeof( can_mbox_t ) );


#define SAM3X8E_CAN_CAN0_BASE ( (uint32_t *) 0x400B4000UL )
can_t * const sam3x8e_can0 = (can_t * const) SAM3X8E_CAN_CAN0_BASE;

#define SAM3X8E_CAN_CAN1_BASE ( (uint32_t *) 0x400B8000UL )
can_t * const sam3x8e_can1 = (can_t * const) SAM3X8E_CAN_CAN1_BASE;


// ----------------------------------------------------------------------

typedef struct _can_chip_specific_state_t {
	can_t *iface;
} can_chip_specific_state_t;


#endif // ! __BOULDER_SLAVE_CHIPS_SAM3X8E_CAN_H
