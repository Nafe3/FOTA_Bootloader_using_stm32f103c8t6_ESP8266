#include "STD_TYPES.h"
#include "Flash.h"
#include "RCC.h"

void main (void)
{
	u32 address = 0x0800F000;
	u32 address2= 0x0800F004;

	RCC_voidSelectSysClk(HSE);
	RCC_voidEnablePeripheral(AHB,RCC_AHBENR_FLITFEN,ON);

	Flash_voidUnlock();
	Flash_voidePageErase(address);
	Flash_voidWriteWord((u32*)address,0x55555555);
	Flash_voidWriteProgram((u32*)address,(u32*)address2,100);
	while(1);
}
