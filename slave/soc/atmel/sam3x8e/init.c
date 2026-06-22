#include <slave/soc.h>

#include <board/pins.h>
#include <common/config.h>
#include <slave/arch.h>
#include <soc/can.h>
#include <soc/state.h>

#include <libsam/include/pio.h>
#include <libsam/include/pmc.h>
#include <libsam/sam3xa/include/sam3xa.h>


static void _clock_init( void );
static void _eefc_init( void );
static void _periph_clock_init( void );
static void _pio_init( void );
static void _plla_init( void );


// по сути, это то же самое, что и SystemInit из system_sam3xa.c

soc_init_error_t
slave_soc_init( __STATE soc_state_t *soc )
{
	// инициализация процессора/-ов -----------------
	
	arch_init_error_t arch_init_error = slave_arch_init();

	if ( arch_init_error != ARCH_INIT_SUCCESS )
		return SOC_INIT_FAILED_ARCH_INIT;

	// инициализация перфиерии на камне -------------
	
	_eefc_init();
	_clock_init();
	_periph_clock_init();
	_pio_init();
	
	return SOC_INIT_SUCCESS;
}


void
_clock_init( void )
{
	// соответствует первому шагу 28.12 programming sequence
	// (см. стр. 531 мануала)
	
	PMC->CKGR_MOR = CKGR_MOR_KEY( 0x37 )
			      | CKGR_MOR_MOSCXTST( 0x8 )
			      | CKGR_MOR_MOSCRCEN
			      | CKGR_MOR_MOSCXTEN;
	
	while ( ! ( PMC->PMC_SR & PMC_SR_MOSCXTS ) ) { }
	
	// переключение на кристаллический осциллятор ---------------
	
	PMC->CKGR_MOR |= CKGR_MOR_KEY( 0x37 ) | CKGR_MOR_MOSCSEL;
	while ( ! ( PMC->PMC_SR & PMC_SR_MOSCSELS) ) { }

	// переключение на plla -------------------------------------

	// хз честно почему оно работает только так
	
	PMC->PMC_MCKR = ( PMC->PMC_MCKR & ~PMC_MCKR_CSS_Msk )
		            | PMC_MCKR_CSS_MAIN_CLK;
	while ( ! ( PMC->PMC_SR & PMC_SR_MCKRDY ) ) { }
	
	_plla_init();

	PMC->PMC_MCKR = ( ( PMC->PMC_MCKR
						| PMC_MCKR_PRES_CLK_2
						| PMC_MCKR_CSS_PLLA_CLK ) & ~PMC_MCKR_CSS_Msk )
		            | PMC_MCKR_CSS_MAIN_CLK;
	while ( ! ( PMC->PMC_SR & PMC_SR_MCKRDY ) ) { }

	PMC->PMC_MCKR = ( ( PMC->PMC_MCKR | PMC_MCKR_PRES_CLK_2 )
					  & ~PMC_MCKR_CSS_Msk )
		            | PMC_MCKR_CSS_PLLA_CLK;
	while ( ! ( PMC->PMC_SR & PMC_SR_MCKRDY ) ) { }
}


void
_eefc_init( void )
{
	EFC0->EEFC_FMR = EFC1->EEFC_FMR = EEFC_FMR_FWS( 4 );
}


void
_periph_clock_init( void )
{
	pmc_enable_periph_clk(   CAN_PERIPH_ID // soc/can.h
						   | ID_UART );
}




void
_pio_init( void )
{
	// PIOA, периферия ------------------------------

	uint32_t pioa_periph_mask =
		  ( 1 << PIN_UART_SERIAL_RX )
		| ( 1 << PIN_UART_SERIAL_TX );
	
	PIO_SetPeripheral( PIOA, PIO_PERIPH_A, pioa_periph_mask );
	PIO_DisableInterrupt( PIOA, pioa_periph_mask );
	PIO_PullUp( PIOA, pioa_periph_mask, PIO_PULLUP );
	
	// PIOB, output ---------------------------------

	uint32_t piob_out = 0;

	if ( BUS_PIN_LED_STATUS == PIOB )
		piob_out |= 1 << PIN_LED_STATUS;

	PIO_DisableInterrupt( PIOB, piob_out );

	PIOB->PIO_CODR = 
	PIOB->PIO_MDDR =
	PIOB->PIO_OWER =
	PIOB->PIO_OER  =
	PIOB->PIO_PER  = piob_out;

	// CAN ------------------------------------------
	
	PIO_SetPeripheral( CAN_PIO_BUS, PIO_PERIPH_A, CAN_PIO_MASK );
	PIO_DisableInterrupt( CAN_PIO_BUS, CAN_PIO_MASK );
	PIO_PullUp( CAN_PIO_BUS, CAN_PIO_MASK, PIO_PULLUP );
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
