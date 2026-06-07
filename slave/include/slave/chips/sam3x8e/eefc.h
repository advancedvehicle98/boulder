#ifndef __BOULDER_SLAVE_CHIPS_SAM3X8E_EEFC_H
#define __BOULDER_SLAVE_CHIPS_SAM3X8E_EEFC_H


#define SAM3X8E_EEFC0_BASE ( (uint32_t *) 0x400E0A00UL )
#define SAM3X8E_EEFC1_BASE ( (uint32_t *) 0x400E0C00UL )


typedef struct _eefc_t {
	__IO  uint32_t fmr; // flash mode register
	__OUT uint32_t fcr; // flash command register
	__IN  uint32_t fsr; // flash status register
	__IN  uint32_t frr; // flash result register
} eefc_t;

#define sam3x8e_eefc0 ( (eefc_t *) SAM3X8E_EEFC0_BASE )
#define sam3x8e_eefc1 ( (eefc_t *) SAM3X8E_EEFC1_BASE )

// flahs mode register

#define SAM3X8E_EEFC_FMR_FWS_POS 8
#define SAM3X8E_EEFC_FMR_FWS_MASK ( 0b1111 << SAM3X8E_EEFC_FMR_FWS_POS )

#define SAM3X8E_EEFC_FMR_FWS( X ) \
	( ( ( X-1) << SAM3X8E_EEFC_FMR_FWS_POS ) & SAM3X8E_EEFC_FMR_FWS_MASK )


#endif // ! __BOULDER_SLAVE_CHIPS_SAM3X8E_EEFC_H
