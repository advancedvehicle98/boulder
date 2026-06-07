#ifndef __BOULDER_SLAVE_CHIPS_SAM3X8E_CKGR_H
#define __BOULDER_SLAVE_CHIPS_SAM3X8E_CKGR_H


typedef struct _ckgr_t {
	__IO  uint32_t uckr;  // utmi clock register
	__IO  uint32_t mor;   // main oscillator register
	__IN  uint32_t mcfr;  // main clock frequency register
	__IO  uint32_t pllar; // plla register
} ckgr_t;


// main oscillator register -----------------------------------------

// main crystall oscillator enable
#define SAM3X8E_CKGR_MOR_MOSCXTEN ( 1 << 0 )

// main on-chip rc oscillator enable
#define SAM3X8E_CKGR_MOR_MOSCRCEN ( 1 << 3 )

// main crystal oscillator start-up time
#define SAM3X8E_CKGR_MOR_MOSCXTST( X ) ( ( ( X ) << 8 ) & 0xFFUL )

// password
#define SAM3X8E_CKGR_MOR_KEY ( 0x37 << 16 )

// main oscillator selection
#define SAM3X8E_CKGR_MOR_MOSCSEL ( 1 << 24 )


// plla register ----------------------------------------------------------------

// plla divider
#define SAM3X8E_CKGR_PLLAR_DIVA( X ) ( ( ( X ) << 0 ) & 0xFFUL )

// plla counter
#define SAM3X8E_CKGR_PLLAR_PLLACOUNT( X ) ( ( ( X ) << 8 ) & 0x3FUL )

// plla multiplier
#define SAM3X8E_CKGR_PLLAR_MULA( X ) ( ( ( X ) << 16 ) & 0x7FFUL )

// one (чтобы можно было записывать в pllar)
#define SAM3X8E_CKGR_PLLAR_ONE ( 1 << 29 )


#endif // ! __BOULDER_SLAVE_CHIPS_SAM3X8E_CKGR_H
