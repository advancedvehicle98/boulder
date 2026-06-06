#ifndef __BOULDER_SLAVE_CHIPS_ARM_CORTEX_M3_H
#define __BOULDER_SLAVE_CHIPS_ARM_CORTEX_M3_H


#include <slave/chip.h>


// очень важные адреса ------------------------------------------------------------

#define CM3_SCS_BASE 0xE000E000UL


// регистры -----------------------------------------------------------------------------------------

// SCB --------------------------------

typedef struct _scb_t {
	__IN uint32_t cpuid;
	__IO uint32_t icsr;
	__IO uint32_t vtor;
	__IO uint32_t aircr;
	__IO uint32_t scr;
	__IO uint32_t ccr;
	__IO uint32_t shp[ 12 ];
	__IO uint32_t shcsr;
	__IO uint32_t cfsr;
	__IO uint32_t hfsr;
	__IO uint32_t dfsr;
	__IO uint32_t mmfar;
	__IO uint32_t bfar;
	__IO uint32_t afsr;
	__IN uint32_t pfr[ 2 ];
	__IN uint32_t dfr;
	__IN uint32_t adr;
	__IN uint32_t mmfr[ 4 ];
	__IN uint32_t isar[ 5 ];
	
	     uint32_t _0[ 5 ];
	
	__IO uint32_t cpacr;
} scb_t;

#define CM3_SCB_BASE ( CM3_SCS_BASE + 0x0D00UL )

#define cm3_scb ( (scb_t *) CM3_SCB_BASE )

#define SCB_VTOR_TBLBASE_POS  29
#define SCB_VTOR_TBLBASE_MASK ( 1UL << SCB_VTOR_TBLBASE_POS )

#define SCB_VTOR_TBLOFF_POS  7
#define SCB_VTOR_TBLOFF_MASK ( 0x3FFFFFUL << SCB_VTOR_TBLOFF_POS )


// обработчики исключений ----------------------------------------------------------------------------------

extern const handler_func_t cm3_nmi_handler;
extern const handler_func_t cm3_hard_fault_handler;
extern const handler_func_t cm3_mem_manage_handler;
extern const handler_func_t cm3_bus_fault_handler;
extern const handler_func_t cm3_usage_fault_handler;
extern const handler_func_t cm3_svc_handler;
extern const handler_func_t cm3_debug_monitor_handler;
extern const handler_func_t cm3_pend_sv_handler;
extern const handler_func_t cm3_sys_tick_handler;


#endif // ! __BOULDER_SLAVE_CHIPS_ARM_CORTEX_M3_H
