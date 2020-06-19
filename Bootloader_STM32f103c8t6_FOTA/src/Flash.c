/*
 * Flash.c
 *
 *  Created on: May 5, 2020
 *      Author: Mohamed Nafea
 */

#include "STD_TYPES.h"
#include "Flash.h"

#ifndef  SCB_BASE_ADDRESS
#define  SCB_BASE_ADDRESS       			0xE000ED00
#endif

#ifndef SCB_AIRCR
#define SCB_AIRCR							*((u32 volatile*)(SCB_BASE_ADDRESS+0x0C))
#endif
/*Reset Mask*/
#ifndef SYSTEM_RESET_MASK
#define SYSTEM_RESET_MASK					((u32)(0x05FA0004))
#endif

/*Flash memory interface registers addresses*/
#define FLASH_INTERFACE_BASE_ADDRESS		0x40022000
#define FLASH_ACR							*((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x000))/*Flash Access Control Register*/
#define FLASH_KEYR							*((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x004))/*FPEC key register*/
#define FLASH_OPTKEYR						*((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x008))/*Flash OPTKEY register*/
#define FLASH_SR                            *((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x00C))/*Flash status register*/
#define FLASH_CR                            *((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x010))/*Flash control register*/
#define FLASH_AR                            *((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x014))/*Flash address register*/
#define FLASH_OBR                           *((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x01C))/*Option byte register*/
#define FLASH_WRPR                          *((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x020))/*Write protection register*/

/*Bit definitions for flash memory interface registers*/
/*FLASH_ACR*/
#define FLASH_ACR_LATENCY					((u32)0x00000007)
#define FLASH_ACR_HLFCYA					((u32)0x00000008) /*Flash half cycle access enable*/
#define FLASH_ACR_PRFTBE					((u32)0x00000010) /*Prefetch buffer enable*/
#define FLASH_ACR_PRFTBS					((u32)0x00000020) /*Prefetch buffer status*/

/*FLASH_SR*/
#define FLASH_SR_BSY						((u32)0x00000001) /*Busy*/
#define FLASH_SR_PGERR						((u32)0x00000004) /*Programming error*/
#define FLASH_SR_WRPRTERR					((u32)0x00000010) /*Write protection error*/
#define FLASH_SR_EOP						((u32)0x00000020) /*End of operation*/

/*FLASH_CR*/
#define FLASH_CR_PG							((u32)0x00000001) /*Flash Programming chosen*/
#define FLASH_CR_PER						((u32)0x00000002) /*Page erase*/
#define FLASH_CR_MER						((u32)0x00000004) /*Mass erase*/
#define FLASH_CR_OPTPG						((u32)0x00000010) /*Option byte Programming*/
#define FLASH_CR_OPTER						((u32)0x00000020) /*Option byte Erase*/
#define FLASH_CR_STRT						((u32)0x00000040) /*Start erase operation*/
#define FLASH_CR_LOCK						((u32)0x00000080) /*LOCK*/
#define FLASH_CR_OPTWRE						((u32)0x00000200) /*Option bytes write enable*/
#define FLASH_CR_ERRIE						((u32)0x00000400) /*Error interrupt enable*/
#define FLASH_CR_EOPIE						((u32)0x00001000) /*End of operation interrupt enable*/

/*FLASH_OBR*/
#define  FLASH_OBR_OPTERR                   ((u32)0x00000001) /*Option Byte Error */
#define  FLASH_OBR_RDPRT                    ((u32)0x00000002) /*Read protection */
#define  FLASH_OBR_WDG_SW                   ((u32)0x00000004) /*WDG_SW */
#define  FLASH_OBR_nRST_STOP                ((u32)0x00000008) /*nRST_STOP */
#define  FLASH_OBR_nRST_STDBY               ((u32)0x00000010) /*nRST_STDBY */
#define  FLASH_OBR_DATA0					((u32)0x0003FC00) /*Data 0 */
#define  FLASH_OBR_DATA1					((u32)0x03FC0000) /*Data 1 */

/*Option bytes addresses*/
#define OPT_BASE_ADDRESS			   		((u32)0x1FFFF800)
#define OPT_1_RDP_USR						0x1FFFF800								/*user configurations and read protection bytes*/
#define OPT_2_DAT0_DAT1                     0x1FFFF804								/*user data storage bytes*/
#define OPT_3_WRP0_WRP1                     *((volatile u32*)0x1FFFF808)			/*write protection bytes*/
#define OPT_4_WRP2_WRP3                     *((volatile u32*)0x1FFFF80C)			/*write protection bytes*/

/*Option bytes masks*/
#define OPT_RDP_MASK						0x000000FF
#define OPT_nRDP_MASK						0x0000FF00
#define OPT_USR_MASK						0x00FF0000
#define OPT_nUSR_MASK						0xFF000000
#define OPT_DAT0_MASK						0x000000FF
#define OPT_nDAT0_MASK                      0x0000FF00
#define OPT_DAT1_MASK                       0x00FF0000
#define OPT_nDAT1_MASK                      0xFF000000
#define OPT_WRP0_MASK						0x000000FF
#define OPT_nWRP0_MASK                      0x0000FF00
#define OPT_WRP1_MASK                       0x00FF0000
#define OPT_nWRP1_MASK                      0xFF000000
#define OPT_WRP2_MASK						0x000000FF
#define OPT_nWRP2_MASK                      0x0000FF00
#define OPT_WRP3_MASK                       0x00FF0000
#define OPT_nWRP3_MASK                      0xFF000000

/*Unlock keys*/
#define RDPRT 								((volatile u16)0x00A5)		/*when this value is written in RDP option byte the flash memory becomes unprotected and mass erase is executed*/
#define KEY1  								0x45670123
#define KEY2  								0xCDEF89AB

/* These buffers will be used to store the values at flash memory addresses before erasing them
 * Then updating them and reloading them again to their addresses*/
u8 FlashSaveBuffer[1024];
u8 OPTSaveBuffer[16];
/*This enum values will be used as the argument (startIndex) in (FLASH_updatePage) function
 *(FLASH_updatePage) function is going to be used automatically inside (flash memory protection functions)*/
enum OPT_bytes  { 	RDP   ,
					nRDP  ,
					USR   ,
					nUSR  ,
					DAT0  ,
					nDAT0 ,
					DAT1  ,
					nDAT1 ,
					WRP0  ,
					nWRP0 ,
					WRP1  ,
					nWRP1 ,
					WRP2  ,
					nWRP2 ,
					WRP3  ,
					nWRP3
		    	};


/***************** Locks FPEC block *****************/
extern ErrorStatus FLASH_Lock  	  (void)
{
	u8 error_status = STD_TYPES_ERROR_NOK;

	FLASH_CR |= FLASH_CR_LOCK;						/*setting the LOCK bit to Lock the FPEC*/

	if(FLASH_CR & FLASH_CR_LOCK) error_status = STD_TYPES_ERROR_OK;/*Verifying*/
 return error_status;
}

/***************** Unlocks FPEC block to enable programming and erasing operations *****************/
extern ErrorStatus FLASH_Unlock (void)
{
	u8 error_status = STD_TYPES_ERROR_NOK;

	FLASH_KEYR = KEY1;
	FLASH_KEYR = KEY2;
	while(FLASH_CR & FLASH_CR_LOCK); 				/*Wait until the FPEC is unlocked*/

	if(!(FLASH_CR & FLASH_CR_LOCK)) error_status = STD_TYPES_ERROR_OK;/*Verifying*/
return error_status;
}

/***************** Writing 32bit (4 bytes) to a selected flash memory address *****************/
/*
 * The main Flash memory programming sequence in standard mode is as follows:
 * 1. Check that no main Flash memory operation is ongoing by checking the BSY bit in FLASH_SR register
 * 2. Set the PG bit in the FLASH_CR register.
 * 3. Perform the data write (half-word) at the desired address.
 * 4. Wait for the BSY bit to be reset.
 * 5. Read the programmed value and verify.
*/
extern ErrorStatus FLASH_WriteWord (void* destAddress, u32 data)
{
	u8 error_status = STD_TYPES_ERROR_NOK;

	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	FLASH_CR |= FLASH_CR_PG;						/*Flash Programming enabled*/

	*((u16*)destAddress)= (u16)(data & 0x0000FFFF); /*Half-word write operation*/
	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	*((u16*)destAddress+1)= (u16)(data >> 16);		/*Half-word write operation*/
	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	while(!(FLASH_SR & FLASH_SR_EOP));              /*wait for end of operation flag to be set*/
	FLASH_SR |= FLASH_SR_EOP;						/*clearing end of operation flag by writing 1 to it*/

	FLASH_CR &=~ FLASH_CR_PG;						/*Flash Programming disabled*/

	if(*((u32*)destAddress) == data) error_status = STD_TYPES_ERROR_OK; /*verifying*/
return error_status;
}

/***************** Writing a certain number of bytes to a selected flash memory address *****************/
extern ErrorStatus FLASH_WriteProgram (void* srcAddress, void* destAddress, u32 numberOfBytes)
{
	u8 error_status = STD_TYPES_ERROR_NOK;
	u32 index;

	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	FLASH_CR |= FLASH_CR_PG;						/*Flash Programming enabled*/

	for(index=0; index<(numberOfBytes/2); index++)  /*looping for half the number of bytes chosen by the user,
													  since we are writing 2 bytes at a time*/
	{
		*((u16*)destAddress + index)= *((u16*)srcAddress + index);
		while(FLASH_SR & FLASH_SR_BSY);				/*wait for busy bit to be cleared*/
	}

	while(!(FLASH_SR & FLASH_SR_EOP));              /*wait for end of operation flag to be set*/
	FLASH_SR |= FLASH_SR_EOP;						/*clearing end of operation flag by writing 1 to it*/

	FLASH_CR &=~ FLASH_CR_PG;						/*Flash Programming disabled*/

	for(index=0; index<(numberOfBytes/4); index++)  /*looping for quarter the number of bytes chosen by the user,
													  since we are reading 4 bytes at a time*/
	{
		if(*((u32*)destAddress + index) == *((u32*)srcAddress + index)) error_status = STD_TYPES_ERROR_OK; /*verifying*/
		else return STD_TYPES_ERROR_NOK;
		//while(FLASH_SR & FLASH_SR_BSY);				/*wait for busy bit to be cleared*/
	}



return error_status;
}

/***************** Erasing a page (1KB) at flash memory address *****************/
/*the procedure below should be followed:
 * 1. Check that no Flash memory operation is ongoing by checking the BSY bit in the FLASH_CR register
 * 2. Set the PER bit in the FLASH_CR register
 * 3. Program the FLASH_AR register to select a page to erase
 * 4. Set the STRT bit in the FLASH_CR register
 * 5. Wait for the BSY bit to be reset
 * 6. Read the erased page and verify
*/
extern ErrorStatus FLASH_PageErase   (u32 pageAddress)
{
	volatile u8 error_status = STD_TYPES_ERROR_NOK;

	FLASH_CR |= FLASH_CR_PER;						/*page erase enable*/

	FLASH_AR  = pageAddress; 						/*passing the required destination address*/

	FLASH_CR |= FLASH_CR_STRT;						/*start erase operation*/

	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	while(!(FLASH_SR & FLASH_SR_EOP));              /*wait for end of operation flag to be set*/
	FLASH_SR |= FLASH_SR_EOP;						/*clearing end of operation flag by writing 1 to it*/

	FLASH_CR &=~ FLASH_CR_PER;						/*page erase disable*/

	if(*((u32*)pageAddress) == 0xFFFFFFFF) error_status = STD_TYPES_ERROR_OK; /*verifying*/

return error_status;
}

extern ErrorStatus FLASH_MultiplePageErase   		(u32 pageAddress, u8 numberOfPages)
{
	u8 error_status = STD_TYPES_ERROR_NOK;
	u32 index;

	FLASH_CR |= FLASH_CR_PER;						/*page erase enable*/

	for(index=0;index<numberOfPages;index++)
	{
		FLASH_AR  = (pageAddress+ index K); 			/*passing the required destination address*/
		FLASH_CR |= FLASH_CR_STRT;						/*start erase operation*/
		while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/
	}

	while(!(FLASH_SR & FLASH_SR_EOP));              	/*wait for end of operation flag to be set*/
	FLASH_SR |= FLASH_SR_EOP;							/*clearing end of operation flag by writing 1 to it*/

	FLASH_CR &=~ FLASH_CR_PER;							/*page erase disable*/

	for(index=0;index<(numberOfPages*256);index++)
	{
		if(*((u32*)(pageAddress+index)) == 0xFFFFFFFF) error_status = STD_TYPES_ERROR_OK; /*verifying*/
		else return STD_TYPES_ERROR_NOK;
	}

return error_status;
}

/***************** Erasing all flash memory *****************/
/*The following sequence is recommended:
 * 1. Check that no Flash memory operation is ongoing by checking the BSY bit in the FLASH_SR register
 * 2. Set the MER bit in the FLASH_CR register
 * 3. Set the STRT bit in the FLASH_CR register
 * 4. Wait for the BSY bit to be reset
 * 5. Read all the pages and verify
 */
extern ErrorStatus FLASH_MassErase    				(void)
{
	u8 error_status = STD_TYPES_ERROR_NOK;
	u32 index;

	FLASH_CR |=  FLASH_CR_MER;		/*Mass erase enabled*/
	FLASH_CR |=  FLASH_CR_STRT;		/*start erase operation*/
	while(FLASH_SR & FLASH_SR_BSY);	/*wait for busy bit to be cleared*/
	FLASH_CR &=~ FLASH_CR_MER;		/*Mass erase disabled*/

	for(index=0;index<(64*256);index++)
	{
		if(*((u32*)(FLASH_MEMORY_BASE_ADDRESS+index)) == 0xFFFFFFFF) error_status = STD_TYPES_ERROR_OK; /*verifying*/
		else return STD_TYPES_ERROR_NOK;
	}

return error_status;
}

/*******************************************************************************************************/
/********************************** Flash Memory Protection Functions **********************************/
/*******************************************************************************************************/

/***************** Locking the option bytes programming *****************/
extern ErrorStatus FLASH_OPT_Lock					(void)
{
	u8 error_status = STD_TYPES_ERROR_NOK;

	FLASH_CR &= ~FLASH_CR_OPTWRE;						/*Clearing the OPWRE bit to Lock option bytes*/

	if(!(FLASH_CR & FLASH_CR_OPTWRE)) error_status = STD_TYPES_ERROR_OK;/*Verifying -> if 0, Locked*/

	error_status = FLASH_Lock();

return error_status;
}

/***************** Unlocking the option bytes programming to enable changing its configurations *****************/
 /* The following sequence is recommended:
 * 1. Unlocking the FPEC
 * 2. Write the same set of KEYS (KEY1 and KEY2) to the FLASH_OPTKEYR register to set the OPTWRE bit in the FLASH_CR register
 * */
extern ErrorStatus FLASH_OPT_Unlock					(void)
{
	u8 error_status = STD_TYPES_ERROR_NOK;

	error_status = FLASH_Unlock();					/*Unlocking the FPEC*/

	FLASH_OPTKEYR = KEY1;
	FLASH_OPTKEYR = KEY2;
	while(!(FLASH_CR & FLASH_CR_OPTWRE)); 			/*Wait until the option bytes writing is enabled*/

	if(!(FLASH_CR & FLASH_CR_OPTWRE)) error_status = STD_TYPES_ERROR_OK;/*Verifying*/

return error_status;
}

/***************** To know which memory pages are read protected *****************/
extern u8 			FLASH_OPT_GetRDPStatus				(void)
{
	u8 status;
	//status = (u8)(*(volatile u32*)OPT_1_RDP_USR & OPT_RDP_MASK);
	status = (u8)(FLASH_OBR & FLASH_OBR_RDPRT);
return status;
}

/***************** Erasing the entire option byte area *****************/
/* The option byte erase sequence (OPTERASE) is as follows:
 * 1. Check that no Flash memory operation is ongoing by reading the BSY bit in the FLASH_SR register
 * 2. Unlock the OPTWRE bit in the FLASH_CR register
 * 3. Set the OPTER bit in the FLASH_CR register
 * 4. Set the STRT bit in the FLASH_CR register
 * 5. Wait for BSY to reset
 * 6. Read the erased option bytes and verify*/
extern ErrorStatus  FLASH_OPT_EraseOPT					(void)
{
	u8 index;
	u8 error_status = STD_TYPES_ERROR_NOK;

	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	error_status = FLASH_OPT_Unlock();				/*Unlocking Option bytes programming*/

	FLASH_CR |= FLASH_CR_OPTER;						/*Enabling Option byte erase*/

	FLASH_CR |= FLASH_CR_STRT;						/*start erase operation*/

	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	while(!(FLASH_SR & FLASH_SR_EOP));              /*wait for end of operation flag to be set*/
	FLASH_SR |= FLASH_SR_EOP;						/*clearing end of operation flag by writing 1 to it*/

	FLASH_CR &=~ FLASH_CR_OPTER;					/*Disabling Option byte erase*/

	for (index=0;index<16;index+=2)					/*Verifying*/
	{
		if(*(u8*)(OPT_1_RDP_USR+index)==0xFF) error_status = STD_TYPES_ERROR_OK;
		else return STD_TYPES_ERROR_NOK;
	}


return error_status;
}

/***************** Enabling Read Protection for all flash memory *****************/
/* Note: Reversing this operation forces a mass erase of the main Flash memory
 * The following sequence is recommended:
 * 1. Check that no main Flash memory operation is ongoing by checking the BSY bit in FLASH_SR register
 * 2. Unlocking Option bytes programming
 * 3. Check that OPTWRE is enabled
 * 4. Set the OPTPG bit in the FLASH_CR register
 * 5. Write 0x00FF (half-word) to RDP address in option bytes
 * 6. Wait for the BSY bit to be reset.
 * 7. Read the programmed value and verify.
 * */
extern void FLASH_OPT_ReadProtection_Enable		(void)
{
	u32 local_update = OPT_RDP_MASK;

	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	FLASH_savePage(OPT_BASE_ADDRESS,SAVE_OPT);		/*management: 1. save option bytes*/

	FLASH_OPT_EraseOPT();							/*management: 2. erase option bytes*/

	FLASH_CR |= FLASH_CR_OPTPG;						/*Enabling Option byte programming*/

	//*(volatile u16*)OPT_1_RDP_USR =  (u16)OPT_RDP_MASK;		/*Writing RDP byte to 0xFF to enable protection*/

	FLASH_updatePage(&local_update,2,RDP,SAVE_OPT);		/*management: 3. update the save buffer*/

	FLASH_reloadPage(OPT_BASE_ADDRESS, SAVE_OPT);				/*Writing RDP byte to 0xFF to enable protection
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	  management: 4. reload the save buffer in its flash address*/

	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	FLASH_CR &=~ FLASH_CR_OPTPG;					/*Disabling Option byte programming*/

//	FLASH_SystemReset();							/*System reset to update the changes*/

}

/***************** Disabling Read Protection for all flash memory *****************/
/*To disable the read protection from the embedded SRAM:
 * 1. Erase the entire option byte area.
 * 	  As a result, the read protection code (RDP) will be 0xFF. At this stage the read protection is still enabled.
 * 2. Program the correct RDP code 0x00A5 to unprotect the memory.
 *    This operation first forces a Mass Erase of the main Flash memory.
 * 3. Reset the device (POR Reset)
 * to reload the option bytes (and the new RDP code) and, to disable the read protection.
*/
extern void FLASH_OPT_ReadProtection_Disable		(void)
{
	u32 local_update = (u32)RDPRT;

	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	FLASH_savePage(OPT_BASE_ADDRESS,SAVE_OPT);		/*management: 1. save option bytes*/

	FLASH_OPT_EraseOPT();                           /*management: 2. erase option bytes*/

	FLASH_CR |= FLASH_CR_OPTPG;                     /*Enabling Option byte programming*/

	//*((volatile u16*)OPT_1_RDP_USR) =  RDPRT;		/*Writing RDP code 0x00A5 in RDP option byte*/

	FLASH_updatePage(&local_update,2,RDP,SAVE_OPT);	/*management: 3. update the save buffer*/

	FLASH_reloadPage(OPT_BASE_ADDRESS, SAVE_OPT);	/*Writing RDP code 0x00A5 in RDP option byte
	 	 	 	 	 	 	 	 	 	 	 	 	  management: 4. reload the save buffer in its flash address*/

	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	FLASH_CR &=~ FLASH_CR_OPTPG;					/*Disabling Option byte programming*/
	//	FLASH_SystemReset();
}

/***************** To know which memory pages are write protected *****************/
/*NOTE: protection is enabled by clearing the corresponding bit
 * 		0: Write protection active
 * 		1: Write protection not active*/
extern u32 			FLASH_OPT_GetWRPStatus				(void)
{
	u32 local_temp = FLASH_WRPR ;
	return local_temp;
}

/***************** Select the pages we want to protect *****************/
/* 1. configuring the WRP[3:0] option bytes
 * 2. applying a system reset to reload the new WRPx option bytes.*/
/*NOTE: protection is enabled by clearing the corresponding bit
 * 		0: Write protection active
 * 		1: Write protection not active*/
/*pageNumber : a number between 0 and 127*/
extern void FLASH_OPT_WriteProtection_Enable		(u32 WRProt_mask)
{
	u32 local_update = 		((OPT_3_WRP0_WRP1 & OPT_WRP0_MASK)
						|  ((OPT_3_WRP0_WRP1 & OPT_WRP1_MASK)>>8)
						|  ((OPT_4_WRP2_WRP3 & OPT_WRP2_MASK)<<16)
						|  ((OPT_4_WRP2_WRP3 & OPT_WRP3_MASK)<<8))
						& 	(~WRProt_mask) ;

	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	FLASH_savePage(OPT_BASE_ADDRESS,SAVE_OPT);		/*management: 1. save option bytes*/

	FLASH_OPT_EraseOPT();							/*management: 2. erase option bytes*/

	FLASH_CR |= FLASH_CR_OPTPG;						/*Enabling Option byte programming*/

	if      ( WRProt_mask >= FLASH_WRProt_Pages0to3   && WRProt_mask <= FLASH_WRProt_Pages28to31 )
		//*((volatile u16*)OPT_3_WRP0_WRP1) =~  (u16)WRProt_mask;
		FLASH_updatePage(&local_update,       1, WRP0, SAVE_OPT);

	else if ( WRProt_mask > FLASH_WRProt_Pages28to31  && WRProt_mask <= FLASH_WRProt_Pages60to63 ){
		local_update = local_update>>8;
		FLASH_updatePage(&local_update,  1, WRP1, SAVE_OPT);}

	else if ( WRProt_mask > FLASH_WRProt_Pages60to63  && WRProt_mask <= FLASH_WRProt_Pages92to95 ){
		local_update = local_update>>16;
		FLASH_updatePage(&local_update, 1, WRP2, SAVE_OPT);}

	else if ( WRProt_mask > FLASH_WRProt_Pages92to95  && WRProt_mask <= FLASH_WRProt_Pages124to127 ){
		//*((volatile u16*)OPT_4_WRP2_WRP3) =~  (u16)(WRProt_mask>>16);
		local_update = local_update>>24;
		FLASH_updatePage(&local_update, 1, WRP3, SAVE_OPT);}
	else {
		FLASH_CR &=~ FLASH_CR_OPTPG;					/*Disabling Option byte programming*/
		return;
	}

	FLASH_reloadPage(OPT_BASE_ADDRESS, SAVE_OPT);		/*Writing RDP byte to 0xFF to enable protection
		 	 	 	 	 	 	 	 	 	 	 	 	 management: 4. reload the save buffer in its flash address*/

	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	FLASH_CR &=~ FLASH_CR_OPTPG;					/*Disabling Option byte programming*/
//	FLASH_SystemReset();							/*applying system reset*/
}

/***************** Select the pages we want to unprotect *****************/
extern void FLASH_OPT_WriteProtection_Disable	(u32 WRProt_mask)
{
	u32 local_update = 		((OPT_3_WRP0_WRP1 & OPT_WRP0_MASK)
						|  	((OPT_3_WRP0_WRP1 & OPT_WRP1_MASK)>>8)
						|  	((OPT_4_WRP2_WRP3 & OPT_WRP2_MASK)<<16)
						|  	((OPT_4_WRP2_WRP3 & OPT_WRP3_MASK)<<8))
						|	 (WRProt_mask);

	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	FLASH_savePage(OPT_BASE_ADDRESS,SAVE_OPT);		/*management: 1. save option bytes*/

	FLASH_OPT_EraseOPT();							/*management: 2. erase option bytes*/

	FLASH_CR |= FLASH_CR_OPTPG;						/*Enabling Option byte programming*/

	//if      ( WRProt_mask >= FLASH_WRProt_Pages0to3   && WRProt_mask <= FLASH_WRProt_Pages60to63 )
	//	*((volatile u16*)OPT_3_WRP0_WRP1) =  (u16)WRProt_mask;
    //
	//else if ( WRProt_mask >= FLASH_WRProt_Pages64to67 && WRProt_mask <= FLASH_WRProt_Pages124to127 )
	//	*((volatile u16*)OPT_4_WRP2_WRP3) =  (u16)(WRProt_mask>>16);
    //
	//else
	//{
	//	FLASH_CR &=~ FLASH_CR_OPTPG;					/*Disabling Option byte programming*/
	//	return;
	//}
	if      ( WRProt_mask >= FLASH_WRProt_Pages0to3   && WRProt_mask <= FLASH_WRProt_Pages28to31 ){
		//*((volatile u16*)OPT_3_WRP0_WRP1) =~  (u16)WRProt_mask;
		//local_update |= *(u32*)OPT_3_WRP0_WRP1;
		FLASH_updatePage(&local_update,       1, WRP0, SAVE_OPT);}

	else if ( WRProt_mask > FLASH_WRProt_Pages28to31  && WRProt_mask <= FLASH_WRProt_Pages60to63 ){
		//local_update |= *(u32*)OPT_3_WRP0_WRP1;
		local_update = local_update>>8;
		FLASH_updatePage(&local_update,  1, WRP1, SAVE_OPT);}

	else if ( WRProt_mask > FLASH_WRProt_Pages60to63  && WRProt_mask <= FLASH_WRProt_Pages92to95 ){
		//local_update |= *(u32*)OPT_4_WRP2_WRP3;
		local_update = local_update>>16;
		FLASH_updatePage(&local_update, 1, WRP2, SAVE_OPT);}

	else if ( WRProt_mask > FLASH_WRProt_Pages92to95  && WRProt_mask <= FLASH_WRProt_Pages124to127 ){
		//*((volatile u16*)OPT_4_WRP2_WRP3) =~  (u16)(WRProt_mask>>16);
		//local_update |= *(u32*)OPT_4_WRP2_WRP3;
		local_update = local_update>>24;
		FLASH_updatePage(&local_update, 1, WRP3, SAVE_OPT);}
	else if(WRProt_mask == FLASH_WRProt_AllPages) {
		FLASH_updatePage(&local_update, 1, WRP0, SAVE_OPT);
		FLASH_updatePage(&local_update, 1, WRP1, SAVE_OPT);
		FLASH_updatePage(&local_update, 1, WRP2, SAVE_OPT);
		FLASH_updatePage(&local_update, 1, WRP3, SAVE_OPT);
	}
	else {
		FLASH_CR &=~ FLASH_CR_OPTPG;					/*Disabling Option byte programming*/
		return;
	}
	FLASH_reloadPage(OPT_BASE_ADDRESS, SAVE_OPT);		/*Writing RDP byte to 0xFF to enable protection
		 	 	 	 	 	 	 	 	 	 	 	 	 management: 4. reload the save buffer in its flash address*/

	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/
	FLASH_CR &=~ FLASH_CR_OPTPG;					/*Disabling Option byte programming*/

	//FLASH_SystemReset();
}



/***************** System reset *****************/
extern void FLASH_SystemReset	(void)
{
	/*System reset will be done by enabling SYSRESETREQ in SCB_AIRCR*/
	/*To write on SCB, we must write 0x5FA with the desired value or else the register won't allow us to write on it (This is already done
	 * in the mask, but I wrote this line here as a reminder)*/
	/*First, we will get value inside SCB register to preserve priorities and save it */
	u32 Local_u32RegisterValue = SCB_AIRCR;
	/*Discard everything else except the priorities by AND with Zeros for the rest of the bits*/
	Local_u32RegisterValue &= 0x700;
	/*OR the priorities value with the reset value*/
	Local_u32RegisterValue |= SYSTEM_RESET_MASK;
	/*Write new value to the register*/
	SCB_AIRCR = Local_u32RegisterValue;
}

/*******************************************************************************************************/
/*************************************** Flash memory management ***************************************/
/*******************************************************************************************************/


/***************** Store Flash memory (page) in RAM *****************/
/* WARNING: MAXIMUM 1K (1 page = 1024 bytes) */
extern ErrorStatus	FLASH_savePage  					(u32 srcAddress, u8 Flash_or_OPT)
{
	u8 error_status = STD_TYPES_ERROR_NOK;
	u16 index;

	if(Flash_or_OPT == SAVE_OPT)
	{
		for(index=0;index<16;index++)
		{
				OPTSaveBuffer[index]=*((u8*)(srcAddress+index)); 	/*Saving*/
		}

		for(index=0;index<16;index++)
		{
				if(OPTSaveBuffer[index] == *((u8*)(srcAddress+index))) error_status = STD_TYPES_ERROR_OK;	/*Verifying*/
				else return STD_TYPES_ERROR_NOK;
		}
	}
	else if(Flash_or_OPT == SAVE_FLASH)
	{
		for(index=0;index<1024;index++)
		{
				FlashSaveBuffer[index]=*((u8*)(srcAddress+index)); 	/*Saving*/
		}

		for(index=0;index<1024;index++)
		{
				if(FlashSaveBuffer[index] == *((u8*)(srcAddress+index))) error_status = STD_TYPES_ERROR_OK;	/*Verifying*/
				else return STD_TYPES_ERROR_NOK;
		}
	}
return error_status;
}



/***************** Update the saved page in RAM and leaving the rest of the data  *****************/
extern ErrorStatus FLASH_updatePage 					(u32* newData,    u16 numberOfBytes, u16 startIndex, u8 Flash_or_OPT)
{
	u8 error_status = STD_TYPES_ERROR_NOK;
	u16 index;

	if(Flash_or_OPT == SAVE_OPT)
	{
		for(index=0;index<numberOfBytes;index++)
		{
			*newData = *newData>>(index*8);
			OPTSaveBuffer[startIndex+index] = *((u8*)newData); /*Updating*/
		}
		for(index=0;index<numberOfBytes;index++)
		{
			 if( *((u8*)(newData+index)) == OPTSaveBuffer[startIndex+index] )error_status = STD_TYPES_ERROR_OK;	/*Verifying*/
			 else return STD_TYPES_ERROR_NOK;
		}
	}
	else if(Flash_or_OPT == SAVE_FLASH)
	{
		for(index=0;index<numberOfBytes;index++)
		{
			*newData = *newData>>(index*8);
			FlashSaveBuffer[startIndex+index] = *((u8*)newData); /*Updating*/
		}
		for(index=0;index<numberOfBytes;index++)
		{
			 if( *((u8*)(newData+index)) == FlashSaveBuffer[startIndex+index] )error_status = STD_TYPES_ERROR_OK;	/*Verifying*/
			 else return STD_TYPES_ERROR_NOK;
		}
	}


return error_status;
}

/***************** Re-writing the updated page from RAM to Flash *****************/
extern ErrorStatus 	FLASH_reloadPage					(u32 destAddress, u8 Flash_or_OPT)
{
	u8 error_status = STD_TYPES_ERROR_NOK;
	u16 index;

	if(Flash_or_OPT == SAVE_OPT)
	{
		FLASH_WriteProgram(OPTSaveBuffer,(u32*)destAddress,16); 	/*Reloading*/

		for(index=0;index<16;index++)
		{
				if( *((u8*)(destAddress+index)) == OPTSaveBuffer[index] ) error_status = STD_TYPES_ERROR_OK;	/*Verifying*/
				else return STD_TYPES_ERROR_NOK;
		}
	}
	else if(Flash_or_OPT == SAVE_FLASH)
	{
		FLASH_WriteProgram(FlashSaveBuffer,(u32*)destAddress,1024);	/*Reloading*/

		for(index=0;index<1024;index++)
		{
				if( *((u8*)(destAddress+index)) == FlashSaveBuffer[index] ) error_status = STD_TYPES_ERROR_OK;	/*Verifying*/
				else return STD_TYPES_ERROR_NOK;
		}
	}

return error_status;
}
