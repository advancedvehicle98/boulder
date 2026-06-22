#include <slave/soc.h>

#include <slave/arch.h>
#include <soc/state.h>


soc_init_error_t
slave_soc_init( __STATE soc_state_t *soc )
{
	// инициализация процессора/-ов -----------------
	
	arch_init_error_t arch_init_error = slave_arch_init();

	if ( arch_init_error != ARCH_INIT_SUCCESS )
		return SOC_INIT_FAILED_ARCH_INIT;
	
	return SOC_INIT_SUCCESS;
}
