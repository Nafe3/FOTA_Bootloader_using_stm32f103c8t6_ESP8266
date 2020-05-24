#include "STD_TYPES.h"
#include "RCC_private.h"
#include "RCC.h"


/*Description: user chooses the state(on/off) of a certain peripheral and certain bus*/
void RCC_voidEnablePeripheral(u8 bus,u32 peripheral,u8 state)
{
	switch(bus)
	{
		case AHB:
			if(state)RCC_AHBENR |= peripheral;	//if state == ON, setting the chosen peripherals
			else	 RCC_AHBENR &=~peripheral;	//if state == OFF, clearing the chosen peripheral bits
			break;
		case APB2:
			if(state)RCC_APB2ENR |= peripheral;	//if state == ON, setting the chosen peripherals
			else	 RCC_APB2ENR &=~peripheral; //if state == OFF, clearing the chosen peripheral bits
			break;
		case APB1:
			if(state)RCC_APB1ENR |= peripheral; //if state == ON, setting the chosen peripherals
			else 	 RCC_APB1ENR &=~peripheral; //if state == OFF, clearing the chosen peripheral bits
			break;
	}
}
/*choose a clock to set as system clock*/
void RCC_voidSelectSysClk(u8 clk)
{
	switch(clk)
	{
		case HSI:
			if  ((RCC_CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_HSI)break;//reading System Clock Switch Status
			else
				{
					RCC_CR 	 |= RCC_CR_HSION;//Setting HSION bit
					while((RCC_CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY);//wait until HSI is ready
					RCC_CFGR &=~RCC_CFGR_SW;//clear
					RCC_CFGR |= RCC_CFGR_SW_HSI;//switch system clock to HSI
				}
			break;
		case HSE:
			if  ((RCC_CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_HSE)break;//reading System Clock Switch Status
			else
				{
					RCC_CR 	 |= RCC_CR_HSEON;//Setting HSEON bit
					while((RCC_CR & RCC_CR_HSERDY) != RCC_CR_HSERDY);//wait until HSE is ready
					RCC_CFGR &=~RCC_CFGR_SW;//clear
					RCC_CFGR |= RCC_CFGR_SW_HSE;//switch system clock to HSE
				}
			break;
		case PLL:
			if   ((RCC_CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL)break;//reading System Clock Switch Status
			else
				{
					RCC_CR	 |= RCC_CR_PLLON;//Setting PLLON bit
					while((RCC_CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);//wait until PLL is ready
					RCC_CFGR &=~RCC_CFGR_SW;//clear
					RCC_CFGR |= RCC_CFGR_SW_PLL;//switch system clock to HSE
				}
			break;
	}
}
/*switch any clock on or off*/
void RCC_voidClkControl(u8 clk,u8 state)
{
	switch(clk)
	{
		case HSI:
			if     (state==ON)	RCC_CR |= RCC_CR_HSION;
			else if(state==OFF) RCC_CR &=~RCC_CR_HSION;
			break;
		case HSE:
			if     (state==ON)	RCC_CR |= RCC_CR_HSEON;
			else if(state==OFF) RCC_CR &=~RCC_CR_HSEON;
			break;
		case PLL:
			if     (state==ON)	RCC_CR |= RCC_CR_PLLON;
			else if(state==OFF) RCC_CR &=~RCC_CR_PLLON;
			break;
	}
}
/*configure PLL using its masks*/
void RCC_voidPLLConfig(u32 PLLMULmask,u32 PPLXTPREmask,u32 PLLSRCmask)
{
	/*If PLL is system clock */
		//Switch system clock to HSI
		//Turn off PLL
		//Apply PLL configurations(mask)
		//Turn on PLL
		//Wait PLL until it is ready
		//Switch system clock to PLL
	//else ->
			//Turn off PLL
			//Apply PLL configurations(mask)
			//Turn on PLL
			//Wait PLL until it is ready
	u32 tempReg;
	if   ((RCC_CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL)//if PLL is system clock
	{
		RCC_CR	 &=~ RCC_CR_HSION;								//clear
		RCC_CR 	 |=  RCC_CR_HSION;								//Setting HSION bit
		while((RCC_CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY);		//Wait until HSI is ready
		tempReg = 	RCC_CFGR;									//because we cannot clear SW bits since 00 will switch to HSI and we dont want to do this
		tempReg &=~ RCC_CFGR_SW_HSI;							//so we clear the temporary register
		tempReg |=  RCC_CFGR_SW_HSI;
		RCC_CFGR =  tempReg;									//Switch system clock to HSI
		RCC_CR  &=~ RCC_CR_PLLON;   							//Turn off PLL
		//need to clear before assignment
		RCC_CFGR &=~ (PLLMULmask | PPLXTPREmask | PLLSRCmask);  //clear
		RCC_CFGR |=  (PLLMULmask | PPLXTPREmask | PLLSRCmask);	//Apply PLL configurations(mask)
		RCC_CR	 |= RCC_CR_PLLON;								//Turn on PLL
		while((RCC_CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);		//Wait until PLL is ready
		RCC_CFGR |= RCC_CFGR_SW_PLL;							//Switch system clock to PLL
	}
	else
	{
		RCC_CR  &=~ RCC_CR_PLLON;   							//Turn off PLL
		//need to clear before assignment
		RCC_CFGR &=~ (PLLMULmask | PPLXTPREmask | PLLSRCmask);  //clear
		RCC_CFGR |=  (PLLMULmask | PPLXTPREmask | PLLSRCmask);	//Apply PLL configurations(mask)
		RCC_CR	 |= RCC_CR_PLLON;								//Turn on PLL
		while((RCC_CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);		//Wait until PLL is ready
	}
}
/*configure Prescalers using its masks*/
void RCC_voidPreConfig(u32 PREtype,u32 PREmask)
{
	switch(PREtype)
	{
		case USBPRE:
			RCC_CFGR &=~ RCC_CFGR_USBPRE;//clear the bits
			RCC_CFGR |=  PREmask;
			break;
		case ADCPRE:
			RCC_CFGR &=~ RCC_CFGR_ADCPRE;//clear the bits
			RCC_CFGR |=  PREmask;
			break;
		case PPRE2:
			RCC_CFGR &=~ RCC_CFGR_PPRE2;//clear the bits
			RCC_CFGR |=  PREmask;
			break;
		case PPRE1:
			RCC_CFGR &=~ RCC_CFGR_PPRE1;//clear the bits
			RCC_CFGR |=  PREmask;
			break;
		case HPRE:
			RCC_CFGR &=~ RCC_CFGR_HPRE;//clear the bits
			RCC_CFGR |=  PREmask;
			break;
	}
}
/*configure MCO using its masks*/
void RCC_voidMCOConfig(u32 MCOmask)
{
	RCC_CFGR &=~ RCC_CFGR_MCO;//clear MCO bits
	RCC_CFGR |=  MCOmask;
}
