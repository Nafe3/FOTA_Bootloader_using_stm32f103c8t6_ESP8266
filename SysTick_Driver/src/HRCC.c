/*
 * HRCC.c
 *
 *  Created on: Mar 31, 2020
 *      Author: moham
 */
#include "STD_TYPES.h"
#include "RCC.h"
#include "RCC_private.h"
#include "HRCC.h"
#include "..\system\include\diag\Trace.h"

/*This function shall return the current system clock*/
/*It should be used in other Handlers or in Application*/
u32 HRCC_getSystemClk(void){

	u32 CurrClk;
	u8 SysClk = RCC_CFGR & RCC_CFGR_SWS >> 2 ; //read SWS 2 bits from RCC_CFGR register
	switch(SysClk)
	{
		case hsi:
			CurrClk = 8000000;
			trace_printf("System clock is HSI");
			break;
		case hse:
			CurrClk = 8000000;
			trace_printf("System clock is HSE");
			break;
		case pll:
			CurrClk = pll;
			trace_printf("System clock is PLL");
			/*Should do more read operations to find out the selected frequency*/
			/*Double check the clock tree in the reference manual
			 *Read ---PLLSRC--- to see which is chosen  (HSI/2) or output from ---PLLXTPRE---*/
			/*if PLLXTPRE is chosen then you have to make another read operation to check for
			 * (HSE or HSE/2)*/
			/*	0000: PLL input clock x 2
				0001: PLL input clock x 3
				0010: PLL input clock x 4
				0011: PLL input clock x 5
				0100: PLL input clock x 6
				0101: PLL input clock x 7
				0110: PLL input clock x 8
				0111: PLL input clock x 9
				1000: PLL input clock x 10
				1001: PLL input clock x 11
				1010: PLL input clock x 12
				1011: PLL input clock x 13
				1100: PLL input clock x 14
				1101: PLL input clock x 15
				1110: PLL input clock x 16
				1111: PLL input clock x 16*/
			break;
	}
	return CurrClk;
}
