#include "STD_TYPES.h"
#include "Flash.h"
#include "RCC.h"
#include "Trace.h"

void main (void)
{
	u32 address = 0x0800F004;
	u32 address2= 0x0800F008;
	volatile u8 status;
	volatile u32 WRP;
	u32 new[]={0x33};

	RCC_voidSelectSysClk(HSE);
	RCC_voidEnablePeripheral(AHB,RCC_AHBENR_FLITFEN,ON);

	status = FLASH_Unlock();

	//status = FLASH_PageErase(address);


	status = FLASH_MultiplePageErase(address,1);
	status = FLASH_WriteWord((u32*)address,0x55555555);
	status = FLASH_WriteProgram((u32*)address,(u32*)address2,1024);

	status = FLASH_savePage(address,SAVE_FLASH);			/*save*/
	status = FLASH_MultiplePageErase(address,1);        	/*erase*/
	FLASH_updatePage(new,2,3,SAVE_FLASH);                   /*update*/
	FLASH_reloadPage(address,SAVE_FLASH);                   /*reload*/


	FLASH_OPT_ReadProtection_Enable();
	FLASH_OPT_ReadProtection_Disable();
	FLASH_OPT_WriteProtection_Enable(FLASH_WRProt_Pages28to31);
	FLASH_OPT_WriteProtection_Enable(FLASH_WRProt_Pages32to35);
	FLASH_OPT_WriteProtection_Disable(FLASH_WRProt_Pages28to31);
	FLASH_SystemReset();

	//status = FLASH_Lock();
	//status = FLASH_OPT_Unlock();
	//status = FLASH_OPT_Lock();
	//FLASH_OPT_EraseOPT();
	//FLASH_SystemReset();
	//FLASH_OPT_ReadProtection_Disable();

	//FLASH_OPT_ReadProtection_Enable();
    //
	//status = FLASH_OPT_GetRDPStatus();


	//status = FLASH_PageErase((FLASH_MEMORY_PAGE_63+0x400));
	//status = FLASH_WriteWord((u32*)(FLASH_MEMORY_PAGE_63+0x400),0x55555555);
	//status = FLASH_WriteProgram((u32*)(FLASH_MEMORY_PAGE_63+0x400),(u32*)(FLASH_MEMORY_PAGE_63+0x404),1023);
	status = FLASH_OPT_GetRDPStatus();
	WRP = FLASH_OPT_GetWRPStatus();

	//FLASH_OPT_ReadProtection_Disable();
	//FLASH_savePage(OPT_BASE_ADDRESS,16);
	//FLASH_OPT_WriteProtection_Enable(FLASH_WRProt_Pages64to67);
	//FLASH_OPT_WriteProtection_Enable(FLASH_WRProt_Pages68to71);
	//FLASH_OPT_WriteProtection_Disable(FLASH_WRProt_Pages68to71);
	trace_printf("status: \t%d\t%#x\n",status,status);
	trace_printf("WRP: \t%d\t%#x\n",WRP,WRP);
	WRP = FLASH_OPT_GetWRPStatus();
	FLASH_SystemReset();
}
