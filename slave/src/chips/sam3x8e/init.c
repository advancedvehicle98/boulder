#include <slave/chip.h>

#include <slave/chips/sam3x8e.h>


// см. https://www.infinite-electronic.ru/datasheet/ab-ATSAM3X8EA-AU.pdf (стр. 318)
// в т.ч. оригинальный libsam


static void _eefc_init( void );
static void _enable_peripheral_clocks( void );
static void _plla_init( void );


chip_init_error_t
slave_chip_init( void )
{
	_eefc_init();
	_plla_init();
	_enable_peripheral_clocks();
	
	return CHIP_INIT_SUCCESS;
}


void
slave_chip_init_log( __IN const chip_init_error_t e )
{
	
}


void
_eefc_init( void )
{
	sam3x8e_eefc0->fmr =
	sam3x8e_eefc1->fmr = SAM3X8E_EEFC_FMR_FWS( 5 );
}


void
_enable_peripheral_clocks( void )
{
#ifdef CONFIG_SLAVE_CAN_IFACE_CAN0
	const uint32_t can_iface = SAM3X8E_PMC_PCER1_CAN0;
#else
	const uint32_t can_iface = SAM3X8E_PMC_PCER1_CAN1;
#endif
	
	sam3x8e_pmc->pcer0 = SAM3X8E_PMC_PCER0_UART
		               | SAM3X8E_PMC_PCER0_PIOA
		               | SAM3X8E_PMC_PCER0_PIOB
		               | SAM3X8E_PMC_PCER0_PIOC
		               | SAM3X8E_PMC_PCER0_PIOD;
	
	sam3x8e_pmc->pcer1 = can_iface;
}


void
_plla_init( void )
{
	pmc_t *  const pmc  = sam3x8e_pmc;
	ckgr_t * const ckgr = &( pmc->ckgr );
	
	// включаем основной осциллятор
	
	{
		if ( sam3x8e_is_rc_oscillator_selected() ) {
			ckgr->mor = SAM3X8E_CKGR_MOR_KEY
			          | SAM3X8E_CKGR_MOR_MOSCXTST( 8 )
			          | SAM3X8E_CKGR_MOR_MOSCRCEN
			          | SAM3X8E_CKGR_MOR_MOSCXTEN;

			while ( ! sam3x8e_is_xtal_stabilized() ) {}
		}
	}

	// переключаемся на осциллятор 3-20 мгц

	{
		ckgr->mor = SAM3X8E_CKGR_MOR_KEY
			      | SAM3X8E_CKGR_MOR_MOSCXTST( 8 )
			      | SAM3X8E_CKGR_MOR_MOSCRCEN
			      | SAM3X8E_CKGR_MOR_MOSCXTEN
			      | SAM3X8E_CKGR_MOR_MOSCSEL;

		while ( sam3x8e_is_main_oscillator_selected() ) { }

		pmc->mckr &= ~SAM3X8E_PMC_MCKR_CSS;
		pmc->mckr |=  SAM3X8E_PMC_MCKR_CSS_MAIN_CLK;

		while ( ! sam3x8e_is_main_clock_ready() ) { }
	}

	// включаем plla
	
	{
		ckgr->pllar = SAM3X8E_CKGR_PLLAR_ONE
			        | SAM3X8E_CKGR_PLLAR_MULA( 0xDUL )
			        | SAM3X8E_CKGR_PLLAR_PLLACOUNT( 0x3FUL )
			        | SAM3X8E_CKGR_PLLAR_DIVA( 0x1UL );

		while ( ! sam3x8e_is_plla_locked() ) { }
	}

	// переключаемся на основной тактовый сигнал
	
	{
		pmc->mckr = SAM3X8E_PMC_MCKR_PRES_CLK_2
			      | SAM3X8E_PMC_MCKR_CSS_MAIN_CLK;

		while ( ! sam3x8e_is_main_clock_ready() ) { }
	}

	// переключаемся обратно на plla (хз может этому объяснение
	// есть в доках, но в противном случае оригинальный код
	// (который system_sam3xa.c) писал шиз)
	
	{
		pmc->mckr = SAM3X8E_PMC_MCKR_PRES_CLK_2
			      | SAM3X8E_PMC_MCKR_CSS_PLLA_CLK;
		
		while ( ! sam3x8e_is_main_clock_ready() ) { }
	}
}
