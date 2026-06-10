#include <slave/soc.h>

#include <common/config.h>
#include <soc/state.h>

#include <libsam/sam3xa/include/sam3xa.h>


static uint32_t _clock_init( void );
static void _eefc_init( void );
static uint32_t _measure_main_clock( void );
static void _plla_init( void );


// по сути, это то же самое, что и SystemInit из system_sam3xa.c

soc_init_error_t
slave_soc_init( __STATE soc_state_t *soc )
{
	_eefc_init();

	soc->mclk = _clock_init();
	
	return SOC_INIT_SUCCESS;
}


uint32_t
_clock_init( void )
{
	// соответствует первому шагу 28.12 programming sequence
	// (см. стр. 531 мануала)
	
	PMC->CKGR_MOR = CKGR_MOR_KEY( 0x37 )
			      | CKGR_MOR_MOSCXTST( 0x8 )
			      | CKGR_MOR_MOSCRCEN
			      | CKGR_MOR_MOSCXTEN;
	
	while ( ! ( PMC->PMC_SR & PMC_SR_MOSCXTS ) ) { }
	
#ifdef CONFIG_SAM3X8E_USE_XTAL
	// переключение main clock на кристаллический осциллятор
	
	PMC->CKGR_MOR |= CKGR_MOR_MOSCSEL;
	while ( ! ( PMC->PMC_SR & PMC_SR_MOSCSELS) ) { }
#endif

	// переключение на plla
	
	_plla_init();

	PMC->PMC_MCKR = PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK;
	while ( ! ( PMC->PMC_SR & PMC_SR_MCKRDY ) ) { }

	// второй шаг 28.12
	
	uint32_t mclk = _measure_main_clock();

	return mclk;
}


void
_eefc_init( void )
{
	EFC0->EEFC_FMR = EFC1->EEFC_FMR = EEFC_FMR_FWS(4);
}


uint32_t
_measure_main_clock( void )
{
	while ( ! ( PMC->CKGR_MCFR & CKGR_MCFR_MAINFRDY ) ) { }
	
	return PMC->CKGR_MCFR & CKGR_MCFR_MAINF_Msk;
}


void
_plla_init( void )
{
	PMC->CKGR_PLLAR = CKGR_PLLAR_ONE
		            | CKGR_PLLAR_MULA( 0xDUL )
		            | CKGR_PLLAR_PLLACOUNT( 0x3FUL )
		            | CKGR_PLLAR_DIVA( 0x1UL );

	while ( ! ( PMC->PMC_SR & PMC_SR_LOCKA ) ) { }
}


