/***************************************/
/*Author: Mahmoud Hamdy ****************/
/*Version: 2.3*/
/*Date	: 26/05/2020********************/
/***************************************/

/*Changelog from version 2.2:
 * 1) Added API for software Reset
 * 2) Added peripherals other than the ones on APB2
 * 3) Fixed a bug found in PLL configurations
 * 4) Improved macros for better use during autocomplete
 * */

/*Changelog from version 2.1
1) Added features to modify prescalar for AHB, APB1, APB2 and ADC*/

/*Change log from version 1.1
1) Functions now work by passing mask as a parameters instead of the old method of passing any numbers
2) Merged the functions of setting and getting clock status to only two functions
3) Improved description of functions
*/


/*Changelog from version 1.0
* 1) Changed ENUMs for return status to #define
  2) Added RCC prefix to macros for better use
  3) Changed names of parameters to match naming convention
  4) Changed some magic numbers in functions to their respective macros
  5) Added return status to enable/disable peripheral functions
  6) Added return type to function names
  7) Added new APIs for controlling MCO and PLL
*/


/*Libraries Inclusion				   */
/*Library Layer						   */
/*This is the library that will contain the standard types 
that we will use like u8, u16, etc.*/
#include "STD_TYPES.h"

/*Current Library Layer*/
#include "RCC_interface.h"

	
/*Description: This API will either enable or disable the desired clock according to parameters passed
Parameters: Desired Status (u32)
Return: None
*/
void RCC_voidSetClockStatus(u32 Copy_u32DesiredClockStatus)
{
	/*Operations will be first done on this local variable, then the value will be transferred to the register*/
	u32 Local_u32DesiredOutput;
	Local_u32DesiredOutput = RCC_CR;
	/*If the parameter passed by the user in enable, we will OR the value with the variable.*/ 
	if (Copy_u32DesiredClockStatus == RCC_ENABLE_HSI || Copy_u32DesiredClockStatus == RCC_ENABLE_HSE || Copy_u32DesiredClockStatus == RCC_ENABLE_PLL)
	{
		Local_u32DesiredOutput |= Copy_u32DesiredClockStatus;
	}
	/*In case of disabling, it will be AND with it*/
	else if (Copy_u32DesiredClockStatus == RCC_DISABLE_HSI || Copy_u32DesiredClockStatus == RCC_DISABLE_HSE || Copy_u32DesiredClockStatus == RCC_DISABLE_PLL)
	{
		Local_u32DesiredOutput &= Copy_u32DesiredClockStatus;
	}
	/*Transfer the value inside the variable to the register*/
	RCC_CR = Local_u32DesiredOutput;
}/*End of voidSetClockStatus*/
	
/*Description: This API will get the status of a clock whether it is enabled or disabled
Parameters: Desired Clock (u32)
Return: Status of clock*/
u8 RCC_u8GetClockStatus(u32 Copy_u32DesiredClock)
{
	/*The CR value will be ANDed with the ready mask. If it returns 
	a value, it means that it is ready. If it returns zero, then it is not*/
	if (RCC_CR & Copy_u32DesiredClock)
	{
		return STATUS_OK;
	}
	else
	{
		return STATUS_NOK;
	}
}/*End of u8GetClockStatus*/


/*Description: This API will be used by
the user to select HSI as the System Clock 
Parameters: Desired Clock (u32)
Return: None*/
void RCC_voidSWSelectClock(u32 Copy_u8Clock)
{
	/*This local variable will be used to save current Copy_u8State of CFGR
	so that we will do the operation on it and then save its Copy_u8State
	again in the CFGR*/
	u32 Local_u32CFGRValue;

	/*Clear The value retrived from the registers so that it will be ready for values to be written on it after putting it in the local variable*/
	Local_u32CFGRValue = RCC_CFGR;
	Local_u32CFGRValue &= ~RCC_SW_CLEAR_MASK;
	
	/*Enable desired clock by OR the value with the value inside the register*/
	Local_u32CFGRValue |= Copy_u8Clock;
	
	/*Transfer the value to the register*/
	RCC_CFGR=Local_u32CFGRValue;
}/*End of voidSWSelectClock*/

/*Description: This API will be used by
the user to get current working system clock
Input: None
Return: The clock currently selected*/
u8 RCC_u8GetSWSStatus(void)
{
	/*The CR value will be ANDed with the ready mask. If it returns 
	a value, it means that it is ready. If it returns zero, then it is not*/
	if (!(RCC_CFGR & RCC_SWS_HSI_MASK))
	{
		return RCC_SWS_HSI;
	}
	
	else if (RCC_CFGR & RCC_SWS_HSE_MASK)
	{
		return RCC_SWS_HSE;
	}
	else if (RCC_CFGR & RCC_SWS_PLL_MASK)
	{
		return RCC_SWS_PLL;
	}
}/*End of u8Get_SWS_Status*/

/*Description: This API will be used by
the user to enable ports clock
INPUT: Desired Peripheral Name (u32)
Return: None*/
void RCC_voidEnablePeripheralClock(u32 Copy_u32Peripheral)
{
	/*Check where this peripheral is located and activate it on corressponding register*/
	if ((Copy_u32Peripheral & RCC_MASK_PERIPHERALS_APB2) == RCC_MASK_PERIPHERALS_APB2)
	{
		/*Remove Prefix*/
		Copy_u32Peripheral &= ~RCC_MASK_PERIPHERALS_APB2;
		/*Write the enable mask passed to the specific register of the peripheral*/
		RCC_APB2ENR |= Copy_u32Peripheral;
	}
	else if ((Copy_u32Peripheral & RCC_MASK_PERIPHERALS_APB1) == RCC_MASK_PERIPHERALS_APB1)
	{
		/*Remove Prefix*/
		Copy_u32Peripheral &= ~RCC_MASK_PERIPHERALS_APB1;
		/*Write the enable mask passed to the specific register of the peripheral*/
		RCC_APB1ENR |= Copy_u32Peripheral;
	}
	else if ((Copy_u32Peripheral & RCC_MASK_PERIPHERALS_AHB) == RCC_MASK_PERIPHERALS_AHB)
	{
		/*Remove Prefix*/
		Copy_u32Peripheral &= ~RCC_MASK_PERIPHERALS_AHB;
		/*Write the enable mask passed to the specific register of the peripheral*/
		RCC_AHBENR |= Copy_u32Peripheral;
	}
}/*End of RCC_voidEnable_Peripheral_Clock*/

/*Description: This API will be used by
the user to disable ports clock
INPUT: Desired Peripheral Name(u32)
Return: None*/
void RCC_voidDisablePeripheralClock(u32 Copy_u32Peripheral)
{
	/*Check where this peripheral is located and activate it on corressponding register*/
	if ((Copy_u32Peripheral & RCC_MASK_PERIPHERALS_APB2) == RCC_MASK_PERIPHERALS_APB2)
	{
		/*Remove Prefix*/
		Copy_u32Peripheral &= ~RCC_MASK_PERIPHERALS_APB2;
		/*Write the enable mask passed to the specific register of the peripheral*/
		RCC_APB2ENR &= ~Copy_u32Peripheral;
	}
	else if ((Copy_u32Peripheral & RCC_MASK_PERIPHERALS_APB1) == RCC_MASK_PERIPHERALS_APB1)
	{
		/*Remove Prefix*/
		Copy_u32Peripheral &= ~RCC_MASK_PERIPHERALS_APB1;
		/*Write the enable mask passed to the specific register of the peripheral*/
		RCC_APB1ENR &= ~Copy_u32Peripheral;
	}
	else if ((Copy_u32Peripheral & RCC_MASK_PERIPHERALS_AHB) == RCC_MASK_PERIPHERALS_AHB)
	{
		/*Remove Prefix*/
		Copy_u32Peripheral &= ~RCC_MASK_PERIPHERALS_AHB;
		/*Write the enable mask passed to the specific register of the peripheral*/
		RCC_AHBENR &= ~Copy_u32Peripheral;
	}
}/*End of RCC_u32Disable_Peripheral_Clock*/

	
/*Description: This API will be used to determine configuration
of MCO
Input: Desired clock (u32)
Return: None
*/
void RCC_voidMCOClock (u32 Copy_u32Clock)
{
	/*Operations will be done first on local variable, then the value will be transferred to the register
	This is done because sometimes hardware restrictions can make conflicts*/
	u32 Local_u32CFGRValue;
	Local_u32CFGRValue = RCC_CFGR;
	
	/*Clear Values inside the register*/
	Local_u32CFGRValue &= RCC_MCO_CLEAR;

	/*According to user input, write the desired value in the local variable
	then transfer it to the register`*/
	Local_u32CFGRValue |= Copy_u32Clock;
	RCC_CFGR = Local_u32CFGRValue;
}/*End of voidMCO_Clock */

/*Description: This API will be used to determine configuration for the PLL clock
Inputs: 1) Source Clock
			2) HSE Divider (Choose no divider in case HSE was not chosen)
			3) PLL Multiplier
Return: None*/
void RCC_voidPLLConfig (u32 Copy_u32SourceClock, u32 Copy_u32HSEDivider, u32 Copy_u32PLLMultiplier)
{
	/*Since configurations cannot be performed if PLL is enabled, the first thing to do will 
	be disabling the PLL and choosing another SW if it is the currently active SW*/
	u8 Local_u8ActiveSW;
	/*This variable will hold the value in CFGR and operations will be done on it*/
	u32 Local_u32CFGRValue;
	/*The return of this function indicates the current active clock*/
	Local_u8ActiveSW = RCC_u8GetSWSStatus();
	/*If current active system clock is PLL, activate HSI and choose it as system clock so that we can be able to stop it*/
	if (Local_u8ActiveSW == RCC_SWS_PLL)
	{
		RCC_voidSetClockStatus(RCC_ENABLE_HSI);
		RCC_voidSWSelectClock(RCC_SW_HSI);
	}
	/*Disable PLL*/
	RCC_voidSetClockStatus(RCC_DISABLE_PLL);
	
	/*Transfer register value in local variable just in case of hardware constraints
	and delete the value inside PLL*/
	Local_u32CFGRValue = RCC_CFGR;
	Local_u32CFGRValue &= RCC_PLL_CLEAR;
	
	/*Choose PLL source according to user input*/
	if (Copy_u32SourceClock == RCC_PLL_SOURCE_HSI2)
	{
		Local_u32CFGRValue &= Copy_u32SourceClock;
	}
	else if (Copy_u32SourceClock == RCC_PLL_SOURCE_HSE)
	{
		Local_u32CFGRValue |= Copy_u32SourceClock;
	}
	
	/*Choose HSE divider*/
	if (Copy_u32HSEDivider == RCC_PLL_HSE_NO_DIVIDER)
	{
		Local_u32CFGRValue &= Copy_u32HSEDivider;
	}
	else if (Copy_u32HSEDivider == RCC_PLL_HSE_DIVIDER)
	{
		Local_u32CFGRValue |= Copy_u32HSEDivider;
	}
	
	/*Choose PLL Multiplier*/
	if (Copy_u32PLLMultiplier == RCC_PLL_MULTIPLIER_2)
	{
		Local_u32CFGRValue &= Copy_u32PLLMultiplier;
	}
	else
	{
		Local_u32CFGRValue |= Copy_u32PLLMultiplier;
	}
	/*Put value inside register*/
	RCC_CFGR = Local_u32CFGRValue;
}

/*Description: This API will be used to change prescalar on a specific bus
 * Parameters: Desired prescalar configuration
 * Return: void
 * */
void RCC_voidChangeBusPrescalar (u32 Copy_u32DesiredConfiguration)
{
	if (Copy_u32DesiredConfiguration == RCC_AHB_PRESCALAR_1 || Copy_u32DesiredConfiguration == RCC_APB1_PRESCALAR_1 || Copy_u32DesiredConfiguration == RCC_APB2_PRESCALAR_1 ||Copy_u32DesiredConfiguration ==RCC_ADC_PRESCALAR_2)
	{
		RCC_CFGR &= Copy_u32DesiredConfiguration;
	}
	else
	{
		RCC_CFGR |= Copy_u32DesiredConfiguration;
	}
}

/*Description: This API will be used to perform system reset
 * Parameters: None
 * Return: void*/
void RCC_voidSystemReset (void)
{
	/*System reset will be done by enabling SYSRESETREQ in SCB_AIRCR*/
	/*To write on SCB, we must write 0x5FA with the desired value or else the register won't allow us to write on it (This is already done
	 * in the mask, but I wrote this line here as a reminder)*/
	/*First, we will get value inside SCB register to preserve priorities and save it */
	u32 Local_u32RegisterValue = RCC_SCB_AIRCR;
	/*Discard everything else except the priorities by AND with Zeros for the rest of the bits*/
	Local_u32RegisterValue &= 0x700;
	/*OR the priorities value with the reset value*/
	Local_u32RegisterValue |= RCC_RESET_MASK;
	/*Write new value to the register*/
	RCC_SCB_AIRCR = Local_u32RegisterValue;
}
