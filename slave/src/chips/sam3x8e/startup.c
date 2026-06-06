#include <slave/chip.h>

#include <slave/chips/sam3x8e.h>
#include <slave/chips/arm/cortex-m3.h>


const handler_func_t cm3_nmi_handler = slave_general_purpose_handler;
const handler_func_t cm3_hard_fault_handler = slave_general_purpose_handler;
const handler_func_t cm3_mem_manage_handler = slave_general_purpose_handler;
const handler_func_t cm3_bus_fault_handler = slave_general_purpose_handler;
const handler_func_t cm3_usage_fault_handler = slave_general_purpose_handler;
const handler_func_t cm3_svc_handler = slave_general_purpose_handler;
const handler_func_t cm3_debug_monitor_handler = slave_general_purpose_handler;
const handler_func_t cm3_pend_sv_handler = slave_general_purpose_handler;
const handler_func_t cm3_sys_tick_handler = slave_general_purpose_handler;

const handler_func_t sam3x8e_supc_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_rstc_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_rtc_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_rtt_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_wdt_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_pmc_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_efc0_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_efc1_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_uart_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_pio_a_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_pio_b_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_pio_c_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_pio_d_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_pio_e_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_usart0_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_usart1_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_usart2_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_usart3_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_hsmci_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_twi0_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_twi1_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_spi0_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_ssc_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_tc0_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_tc1_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_tc2_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_tc3_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_tc4_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_tc5_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_tc6_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_tc7_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_tc8_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_pwm_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_adc_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_dacc_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_dmac_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_uotghs_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_trng_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_emac_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_can0_handler = slave_general_purpose_handler;
const handler_func_t sam3x8e_can1_handler = slave_general_purpose_handler;


__attribute__(( section( ".vectors" ) ))
const handler_func_t exception_handlers[] = {
	  (handler_func_t) &_estack
	, slave_chip_reset
	
	, cm3_nmi_handler
	, cm3_hard_fault_handler
	, cm3_mem_manage_handler
	, cm3_bus_fault_handler
	, cm3_usage_fault_handler

	, (handler_func_t) 0x0
	, (handler_func_t) 0x0
	, (handler_func_t) 0x0
	, (handler_func_t) 0x0

	, cm3_svc_handler
	, cm3_debug_monitor_handler
	
	, (handler_func_t) 0x0
	
	, cm3_pend_sv_handler
	, cm3_sys_tick_handler
	
	, sam3x8e_supc_handler
	, sam3x8e_rstc_handler
	, sam3x8e_rtc_handler
	, sam3x8e_rtt_handler
	, sam3x8e_wdt_handler
	, sam3x8e_pmc_handler
	, sam3x8e_efc0_handler
	, sam3x8e_efc1_handler
	, sam3x8e_uart_handler
	
	, (handler_func_t) 0x0
	, (handler_func_t) 0x0
	
	, sam3x8e_pio_a_handler
	, sam3x8e_pio_b_handler
	, sam3x8e_pio_c_handler
	, sam3x8e_pio_d_handler
	, sam3x8e_pio_e_handler
	, (handler_func_t) 0x0
	
	, sam3x8e_usart0_handler
	, sam3x8e_usart1_handler
	, sam3x8e_usart2_handler
	, sam3x8e_usart3_handler
	
	, sam3x8e_hsmci_handler
	
	, sam3x8e_twi0_handler
	, sam3x8e_twi1_handler
	
	, sam3x8e_spi0_handler
	, (handler_func_t) 0x0
	
	, sam3x8e_ssc_handler
	
	, sam3x8e_tc0_handler
	, sam3x8e_tc1_handler
	, sam3x8e_tc2_handler
	, sam3x8e_tc3_handler
	, sam3x8e_tc4_handler
	, sam3x8e_tc5_handler
	, sam3x8e_tc6_handler
	, sam3x8e_tc7_handler
	, sam3x8e_tc8_handler
	
	, sam3x8e_pwm_handler
	
	, sam3x8e_adc_handler
	, sam3x8e_dacc_handler
	
	, sam3x8e_dmac_handler
	
	, sam3x8e_uotghs_handler
	
	, sam3x8e_trng_handler
	
	, sam3x8e_emac_handler
	
	, sam3x8e_can0_handler
	, sam3x8e_can1_handler
};
