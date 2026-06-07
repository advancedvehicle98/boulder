#ifndef __BOULDER_SLAVE_CHIPS_SAM3X8E_H
#define __BOULDER_SLAVE_CHIPS_SAM3X8E_H


#include <slave/chip.h>

#include <common/defines.h>

#include <stdint.h>

#include "sam3x8e/can.h"
#include "sam3x8e/ckgr.h"
#include "sam3x8e/eefc.h"
#include "sam3x8e/pmc.h"


#define SAM3X8E_IRAM0_BASE   ( (uint32_t *) 0x20000000UL )
#define SAM3X8E_NFC_RAM_BASE ( (uint32_t *) 0x20100000UL )


// это тянется из link.ld ----------------------------------------------

extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;


// пускай будет ----------------------------------------------------------
                             
extern const handler_func_t sam3x8e_supc_handler;
extern const handler_func_t sam3x8e_rstc_handler;
extern const handler_func_t sam3x8e_rtc_handler;
extern const handler_func_t sam3x8e_rtt_handler;
extern const handler_func_t sam3x8e_wdt_handler;
extern const handler_func_t sam3x8e_pmc_handler;
extern const handler_func_t sam3x8e_efc0_handler;
extern const handler_func_t sam3x8e_efc1_handler;
extern const handler_func_t sam3x8e_uart_handler;
extern const handler_func_t sam3x8e_pio_a_handler;
extern const handler_func_t sam3x8e_pio_b_handler;
extern const handler_func_t sam3x8e_pio_c_handler;
extern const handler_func_t sam3x8e_pio_d_handler;
extern const handler_func_t sam3x8e_pio_e_handler;
extern const handler_func_t sam3x8e_usart0_handler;
extern const handler_func_t sam3x8e_usart1_handler;
extern const handler_func_t sam3x8e_usart2_handler;
extern const handler_func_t sam3x8e_usart3_handler;
extern const handler_func_t sam3x8e_hsmci_handler;
extern const handler_func_t sam3x8e_twi0_handler;
extern const handler_func_t sam3x8e_twi1_handler;
extern const handler_func_t sam3x8e_spi0_handler;
extern const handler_func_t sam3x8e_ssc_handler;
extern const handler_func_t sam3x8e_tc0_handler;
extern const handler_func_t sam3x8e_tc1_handler;
extern const handler_func_t sam3x8e_tc2_handler;
extern const handler_func_t sam3x8e_tc3_handler;
extern const handler_func_t sam3x8e_tc4_handler;
extern const handler_func_t sam3x8e_tc5_handler;
extern const handler_func_t sam3x8e_tc6_handler;
extern const handler_func_t sam3x8e_tc7_handler;
extern const handler_func_t sam3x8e_tc8_handler;
extern const handler_func_t sam3x8e_pwm_handler;
extern const handler_func_t sam3x8e_adc_handler;
extern const handler_func_t sam3x8e_dacc_handler;
extern const handler_func_t sam3x8e_dmac_handler;
extern const handler_func_t sam3x8e_uotghs_handler;
extern const handler_func_t sam3x8e_trng_handler;
extern const handler_func_t sam3x8e_emac_handler;
extern const handler_func_t sam3x8e_can0_handler;
extern const handler_func_t sam3x8e_can1_handler;


#endif // ! __BOULDER_SLAVE_CHIPS_SAM3X8E_H
