#include <slave/chip.h>

#include <slave/chips/sam3x8e.h>
#include <slave/chips/arm/cortex-m3.h>

#include <common/defines.h>


void
slave_chip_reset( void )
{
	uint32_t *src, *dest;

	// перемещаем перемещаемый код, если есть
	
	{
		src = &_etext;
		dest = &_srelocate;

		if ( src != dest ) for ( ; dest < &_erelocate; ) *dest++ = *src++;
	}

	// зануляем BSS-секцию
	
	{	
		for ( dest = &_szero; dest < &_ezero; ) *dest++ = 0;
	}

	// копируем адрес векторов в SCB_VTOR
	
	{
		src = (uint32_t *) &_sfixed;
		cm3_scb->vtor = (uint32_t) src & CM3_SCB_VTOR_TBLOFF_MASK;
	
		if (    src >= SAM3X8E_IRAM0_BASE
			 && src <  SAM3X8E_NFC_RAM_BASE )
			cm3_scb->vtor |= 1UL << CM3_SCB_VTOR_TBLBASE_POS;
	}

	// то ради чего мы все здесь собрались
	main();
}
