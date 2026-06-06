#include <slave/chip.h>

#include <slave/chips/sam3x8e.h>
#include <slave/chips/arm/cortex-m3.h>

#include <common/defines.h>


__NORETURN void
slave_chip_reset( void )
{
	uint32_t *src, *dest;

	src = &_etext;
	dest = &_srelocate;

	if ( src != dest ) for ( ; dest < &_erelocate; ) *dest++ = *src++;

	for ( dest = &_szero; dest < &_ezero; ) *dest++ = 0;

	src = (uint32_t *) &_sfixed;
	cm3_scb->vtor = (uint32_t) src & SCB_VTOR_TBLOFF_MASK;

	if (    (uint32_t) src >= SAM3X8E_IRAM0_ADDR
		 && (uint32_t) src <  SAM3X8E_NFC_RAM_ADDR )
		cm3_scb->vtor |= 1UL << SCB_VTOR_TBLBASE_POS;

	main();
}
