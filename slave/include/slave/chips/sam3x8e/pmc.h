#ifndef __BOULDER_SLAVE_CHIPS_SAM3X8E_PMC_H
#define __BOULDER_SLAVE_CHIPS_SAM3X8E_PMC_H


#include "ckgr.h"

#include <stdbool.h>


#define SAM3X8E_PMC_BASE ( (uint32_t *) 0x400E0600UL )


typedef struct _pmc_t {
	__OUT uint32_t scer; // system clock enable register
	__OUT uint32_t scdr; // system clock disable register
	__IN  uint32_t scsr; // system clock status register
	
	uint32_t _0;
	
	__OUT uint32_t pcer0; // peripheral clock enable register 0
	__OUT uint32_t pcdr0; // peripheral clock disable register 0
	__IN  uint32_t pcsr0; // peripheral clock status register 0

	ckgr_t ckgr; // clock generator

	uint32_t _1;
	__IO  uint32_t mckr; // master clock register

	uint32_t _2;
	__IO  uint32_t usb;  // usb clock register

	uint32_t _3;
	__IO  uint32_t pck[ 3 ];  // programmable clock 0 register

	uint32_t _4[ 5 ];

	__OUT uint32_t ier;  // interrupt enable register
	__OUT uint32_t idr;  // interrupt disable register
	__IN  uint32_t sr;   // status register
	__IN  uint32_t imr;  // interrupt mask register
	
	__IO  uint32_t fsmr; // fast startup mode register
	__IO  uint32_t fspr; // fast startup polarity register
	
	__OUT uint32_t focr; // fault output clear register
	
	uint32_t _5[ 26 ];

	__IO  uint32_t wpmr; // write protect mode register
	__IN  uint32_t wpsr; // write protect status register
	
	uint32_t _6[ 5 ];

	__OUT uint32_t pcer1; // peripheral clock enable register 1
	__OUT uint32_t pcdr1; // peripheral clock disable register 1
	__IN  uint32_t pcsr1; // peripheral clock status register 1
	
	__IO  uint32_t pcr; // peripheral control register
} pmc_t;


_assert( sizeof( pmc_t ) == 0x010C + 0x4 );


#define sam3x8e_pmc ( (pmc_t *) SAM3X8E_PMC_BASE )


// всё че связано с тактовым генератором (ckgr)

static inline bool
sam3x8e_is_rc_oscillator_selected( void )
{
	return ! ( sam3x8e_pmc->ckgr.mor & SAM3X8E_CKGR_MOR_MOSCSEL );
}


// отсальное для pmc

// status register -----------------------------------------------------

#define SAM3X8E_PMC_SR_MOSCXTS  ( 1 << 0 )
#define SAM3X8E_PMC_SR_LOCKA    ( 1 << 1 )
#define SAM3X8E_PMC_SR_MCKRDY   ( 1 << 3 )
#define SAM3X8E_PMC_SR_MOSCSELS ( 1 << 16 )

static inline bool
sam3x8e_is_main_clock_ready( void )
{
	return sam3x8e_pmc->sr & SAM3X8E_PMC_SR_MCKRDY;
}

static inline bool
sam3x8e_is_main_oscillator_selected( void )
{
	return ! ( sam3x8e_pmc->sr & SAM3X8E_PMC_SR_MOSCSELS );
}

static inline bool
sam3x8e_is_plla_locked( void )
{
	return sam3x8e_pmc->sr & SAM3X8E_PMC_SR_LOCKA;
}

static inline bool
sam3x8e_is_xtal_stabilized( void )
{
	return sam3x8e_pmc->sr & SAM3X8E_PMC_SR_MOSCXTS;
}


// master clock register -------------------------------------------

#define SAM3X8E_PMC_MCKR_CSS ( 0b11UL << 0 )

#define SAM3X8E_PMC_MCKR_CSS_SLOW_CLK ( 0UL << 0 )
#define SAM3X8E_PMC_MCKR_CSS_MAIN_CLK ( 1UL << 0 )
#define SAM3X8E_PMC_MCKR_CSS_PLLA_CLK ( 2UL << 0 )
#define SAM3X8E_PMC_MCKR_CSS_UPLL_CLK ( 3UL << 0 )

#define SAM3X8E_PMC_MCKR_PRES ( 0x7UL << 4 )

#define SAM3X8E_PMC_MCKR_PRES_CLK_2 ( 1UL << 4 )


// peripheral clock enable register(s) ---------------------------------

#define SAM3X8E_PMC_PCER0_UART ( 1 << 8 )
#define SAM3X8E_PMC_PCER0_PIOA ( 1 << 11 )
#define SAM3X8E_PMC_PCER0_PIOB ( 1 << 12 )
#define SAM3X8E_PMC_PCER0_PIOC ( 1 << 13 )
#define SAM3X8E_PMC_PCER0_PIOD ( 1 << 14 )

#define SAM3X8E_PMC_PCER1_CAN0 ( 1 << 11 )
#define SAM3X8E_PMC_PCER1_CAN1 ( 1 << 12 )


#endif // ! __BOULDER_SLAVE_CHIPS_SAM3X8E_PMC_H
