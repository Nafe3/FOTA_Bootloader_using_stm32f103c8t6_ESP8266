/*
 * Flash.c
 *
 *  Created on: May 5, 2020
 *      Author: moham
 */

#include "STD_TYPES.h"
#include "Flash.h"

/*Flash memory interface registers addresses*/
#define FLASH_INTERFACE_BASE_ADDRESS		0x40022000
#define FLASH_ACR							*((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x000))/*Flash Access Control Register*/
#define FLASH_KEYR							*((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x004))/**/
#define FLASH_OPTKEYR						*((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x008))/**/
#define FLASH_SR                            *((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x00C))/**/
#define FLASH_CR                            *((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x010))/**/
#define FLASH_AR                            *((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x014))/**/
#define FLASH_OBR                           *((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x01C))/**/
#define FLASH_WRPR                          *((volatile u32*)(FLASH_INTERFACE_BASE_ADDRESS+0x020))/**/


/*Bit definitions for flash memory interface registers*/
/*FLASH_ACR*/
#define FLASH_ACR_LATENCY					0x00000007
#define FLASH_ACR_HLFCYA					0x00000008 /*Flash half cycle access enable*/
#define FLASH_ACR_PRFTBE					0x00000010 /*Prefetch buffer enable*/
#define FLASH_ACR_PRFTBS					0x00000020 /*Prefetch buffer status*/


/*FLASH_SR*/
#define FLASH_SR_BSY						0x00000001 /*Busy*/
#define FLASH_SR_PGERR						0x00000004 /*Programming error*/
#define FLASH_SR_WRPRTERR					0x00000010 /*Write protection error*/
#define FLASH_SR_EOP						0x00000020 /*End of operation*/

/*FLASH_CR*/
#define FLASH_CR_PG							0x00000001 /*Flash Programming chosen*/
#define FLASH_CR_PER						0x00000002 /*Page erase*/
#define FLASH_CR_MER						0x00000004 /*Mass erase*/
#define FLASH_CR_OPTPG						0x00000010 /*Option byte Programming*/
#define FLASH_CR_OPTER						0x00000020 /*Option byte Erase*/
#define FLASH_CR_STRT						0x00000040 /*Start erase operation*/
#define FLASH_CR_LOCK						0x00000080 /*LOCK*/
#define FLASH_CR_OPTWRE						0x00000100 /*Option bytes write enable*/
#define FLASH_CR_ERRIE						0x00000200 /*Error interrupt enable*/
#define FLASH_CR_EOPIE						0x00000800 /*End of operation interrupt enable*/

/*Unlock keys*/
#define KEY1  0x45670123
#define KEY2  0xCDEF89AB


/*Locks FPEC block*/
extern void Flash_voidLock  	  (void)
{
	FLASH_CR |= FLASH_CR_LOCK;
}

/*Unlocks FPEC block to enable programming and erasing operations*/
extern void Flash_voidUnlock      (void)
{
	FLASH_KEYR = KEY1;
	FLASH_KEYR = KEY2;
	while(FLASH_CR & FLASH_CR_LOCK);
}

/*Writing 32bit (4 bytes) to a selected flash memory address*/
/*
The main Flash memory programming sequence in standard mode is as follows:
● Check that no main Flash memory operation is ongoing by checking the BSY bit in the
  FLASH_SR register.
● Set the PG bit in the FLASH_CR register.
● Perform the data write (half-word) at the desired address.
● Wait for the BSY bit to be reset.
● Read the programmed value and verify.
*/
extern void Flash_voidWriteWord   (void* destAddress, u32 data)
{
	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	FLASH_CR |= FLASH_CR_PG;						/*Flash Programming enabled*/

	*((u16*)destAddress)= (u16)(data & 0x0000FFFF);
	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	*((u16*)destAddress+1)= (u16)(data >> 16);
	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	FLASH_CR &=~ FLASH_CR_PG;						/*Flash Programming disabled*/
}

/*Writing a certain number of bytes to a selected flash memory address*/
extern void Flash_voidWriteProgram(void* srcAddress, void* destAddress, u8 numberOfBytes)
{
	u16 index;
	while(FLASH_SR & FLASH_SR_BSY);					/*wait for busy bit to be cleared*/

	FLASH_CR |= FLASH_CR_PG;						/*Flash Programming enabled*/

	for(index=0; index<(numberOfBytes/2); index++)
	{
		*((u16*)destAddress + index)= *((u16*)srcAddress + index);
		while(FLASH_SR & FLASH_SR_BSY);				/*wait for busy bit to be cleared*/

	}

	FLASH_CR &=~ FLASH_CR_PG;/*Flash Programming disabled*/
}

/*Erasing a page (1KB) at flash memory address*/
/*
the procedure below should be followed:
● Check that no Flash memory operation is ongoing by checking the BSY bit in the
FLASH_CR register
● Set the PER bit in the FLASH_CR register
● Program the FLASH_AR register to select a page to erase
● Set the STRT bit in the FLASH_CR register
● Wait for the BSY bit to be reset
● Read the erased page and verify
*/
extern void Flash_voidePageErase   (u32 pageAddress)
{
	FLASH_CR |= FLASH_CR_PER;		/*page erase enable*/
	FLASH_AR  = pageAddress; 		/*passing the required destination address*/
	FLASH_CR |= FLASH_CR_STRT;		/*start erase operation*/
	while(FLASH_SR & FLASH_SR_BSY);	/*wait for busy bit to be cleared*/
	FLASH_CR &=~ FLASH_CR_PER;		/*page erase disable*/
}

/*Erasing all flash memory*/
/*
The following sequence is
recommended:
● Check that no Flash memory operation is ongoing by checking the BSY bit in the
FLASH_SR register
● Set the MER bit in the FLASH_CR register
● Set the STRT bit in the FLASH_CR register
● Wait for the BSY bit to be reset
● Read all the pages and verify
 */
extern void Flash_voidMassErase    (void)
{
	FLASH_CR |=  FLASH_CR_MER;		/*Mass erase enabled*/
	FLASH_CR |=  FLASH_CR_STRT;		/*start erase operation*/
	while(FLASH_SR & FLASH_SR_BSY);	/*wait for busy bit to be cleared*/
	FLASH_CR &=~ FLASH_CR_MER;		/*Mass erase disabled*/
}

















