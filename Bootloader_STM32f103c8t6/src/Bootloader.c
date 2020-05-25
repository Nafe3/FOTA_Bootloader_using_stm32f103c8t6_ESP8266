/*
 * Bootloader.c
 *
 *  Created on: May 24, 2020
 *      Author: Mohamed Nafea
 */

#include "STD_TYPES.h"
#include "Bootloader.h"
#include "Debug.h"
//#include "../system/include/cmsis/"

#define  SCB_BASE_ADDRESS       		0xE000E008
#define  SCB_VTOR		 				*((volatile u32*)(SCB_BASE_ADDRESS+0x008))

#define  FLASH_BOOTLDR_BASE_ADDRESS		0x08000000
#define  FLASH_USR_APP_BASE_ADDRESS		0x08008000

/*Reads the command packet which comes from the host application*/
extern void bootloader_voidUARTReadData (void)
{
	printmsg("BL_DEBUG_MSG: Button is pressed .. going to BL mode\n\r");
}
extern void bootloader_voidJumpToUserApp(void)
{
	printmsg("BL_DEBUG_MSG: Button is not pressed .. executing user app\n\r");

	//just a function to hold the address of the reset handlerof the user app.
	void (*app_reset_handler)(void);

	/*1. configure the MSP by reading the value from the base address of the FLASH sector
	 * that contains the user app*/
	u32 msp_value = *(volatile u32*)FLASH_USR_APP_BASE_ADDRESS;  //getting the user app flash sector
	printmsg("BL_DEBUG_MSG: MSP value = %#x\n",msp_value);

	//This function comes from CMSIS
	//__set_MSP(msp_value);										//forcing sp to go to the user app flash sector
	asm volatile ("MSR msp, %0\n" : : "r" (msp_value) : "sp");
	//SCB_VTOR = FLASH_BOOTLDR_BASE_ADDRESS;

	/* 2. Now fetch the reset handler address of the user application
	 * from the location FLASH_USR_APP_BASE_ADDRESS
	 * Here, we gave the address of the reset handler to a pointer to function, so that
	 * this pointer to function (app_reset_handler)_ can trigger the user application's reset handler
	 * which jumps to the user's (main) function through (_start) function in (startup.c)
	 * */
	app_reset_handler = (void*)(*((volatile u32*)(FLASH_USR_APP_BASE_ADDRESS+0x04)));

	/*3. Jump to reset handler of the user application*/
	app_reset_handler();
}

