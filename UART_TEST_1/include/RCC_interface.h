/*This driver is for controlling STM32  RCC*/

#ifndef RCC_INTERFACE_H_
#define RCC_INTERFACE_H_

/***************************************/
/*Author: Mahmoud Hamdy ****************/
/*Version: 2.3*/
/*Date	: 26/05/2020********************/
/***************************************/

#include "STD_TYPES.h"

/*Changelog from version 2.2:
 * 1) Added API for software Reset
 * 2) Added peripherals other than the ones on APB2
 * 3) Fixed a bug found in PLL configurations
 * 4) Improved macros for better use during autocomplete
 * */

/*Changelog from version 2.1
1) Added features to modify prescalar for AHB, APB1, APB2 and ADC*/

/*Changelog from version 2.0
1) Added UART1 peripheral to list of peripherals
2) Added "RCC_PERIPHERALS" prefix to peripherals for easier access from autocomplete*/

/*Change log from version 1.1
1) Functions now work by passing mask as a parameters
*/
/*Changelog from version 1.0
*1) Changed ENUMs for return status to #define
  2) Added Header Guard
  3) Added RCC Prefix to macros for better use
  4) Added Casting for macros
  5) Added "extern" modifier for functions just in case a compiler ever needs it
  6) Added return type to function names
  7) Added new APIs for controlling MCO and PLL
*/



/*These macros will be used to define return status of SWS function*/
#define RCC_SWS_HSI						(u8)0
#define RCC_SWS_HSE						(u8)1
#define RCC_SWS_PLL						(u8)2

/***********Macros to be used by user************************/
/********Clock Controls********/
#define RCC_ENABLE_HSI					((u32)0x1)
#define RCC_DISABLE_HSI				~((u32)0x1)
#define RCC_ENABLE_HSE					((u32)0x10000)
#define RCC_DISABLE_HSE				~((u32)0x10000)
#define RCC_ENABLE_PLL					((u32)0x1000000)
#define RCC_DISABLE_PLL					~((u32)0x1000000)
#define RCC_HSI_STATUS 				((u32)0x2)			
#define RCC_HSE_STATUS 				((u32)0x20000)		
#define RCC_PLL_STATUS  				((u32)0x2000000)
	
/*SW Controls*/
#define RCC_SW_HSI					((u32)0x0)
#define RCC_SW_HSE					((u32)0x1)
#define RCC_SW_PLL						((u32)0x2)
	
/*MCO Controls*/
#define RCC_MCO_NOCLOCK					((u32)0x03000000)
#define RCC_MCO_HSI							((u32)0x05000000)
#define RCC_MCO_HSE							((u32)0x06000000)
#define RCC_MCO_PLL_DIVIDED_2			((u32)0x07000000)
	
/*PLL Controls*/
/*Source Clock*/
#define RCC_PLL_SOURCE_HSI2			((u32)0xFFFEFFFF)
#define RCC_PLL_SOURCE_HSE				((u32)0x00010000)
/*HSE Divider*/
#define RCC_PLL_HSE_NO_DIVIDER		((u32)0xFFFDFFFF)
#define RCC_PLL_HSE_DIVIDER				((u32)0x00020000)
/*PLL Multiplier*/
#define RCC_PLL_MULTIPLIER_2				((u32)0xFFC3FFFF)
#define RCC_PLL_MULTIPLIER_3				((u32)0x00040000)
#define RCC_PLL_MULTIPLIER_4				((u32)0x00080000)
#define RCC_PLL_MULTIPLIER_5				((u32)0x000C0000)
#define RCC_PLL_MULTIPLIER_6				((u32)0x00100000)
#define RCC_PLL_MULTIPLIER_7				((u32)0x00140000)
#define RCC_PLL_MULTIPLIER_8				((u32)0x00180000)
#define RCC_PLL_MULTIPLIER_9				((u32)0x001C0000)
#define RCC_PLL_MULTIPLIER_10				((u32)0x00200000)
#define RCC_PLL_MULTIPLIER_11				((u32)0x00240000)
#define RCC_PLL_MULTIPLIER_12				((u32)0x00280000)
#define RCC_PLL_MULTIPLIER_13				((u32)0x002C0000)
#define RCC_PLL_MULTIPLIER_14				((u32)0x00300000)
#define RCC_PLL_MULTIPLIER_15				((u32)0x00340000)
#define RCC_PLL_MULTIPLIER_16				((u32)0x00380000)

/*Prescalars*/
#define RCC_AHB_PRESCALAR_1								((u32)~ (0b10000000))
#define RCC_AHB_PRESCALAR_2								((u32)(0b10000000))
#define RCC_AHB_PRESCALAR_4								((u32)(0b10010000))
#define RCC_AHB_PRESCALAR_8								((u32)(0b10100000))
#define RCC_AHB_PRESCALAR_16								((u32)(0b10110000))
#define RCC_AHB_PRESCALAR_64								((u32)(0b11000000))
#define RCC_AHB_PRESCALAR_128							((u32)(0b11010000))
#define RCC_AHB_PRESCALAR_256							((u32)(0b11100000))
#define RCC_AHB_PRESCALAR_512							((u32)(0b11110000))

#define RCC_APB1_PRESCALAR_1								((u32)~(0x400))
#define RCC_APB1_PRESCALAR_2								((u32)(0x400))
#define RCC_APB1_PRESCALAR_4								((u32)0x500)
#define RCC_APB1_PRESCALAR_8								((u32)0x600)
#define RCC_APB1_PRESCALAR_16							((u32)0x700)

#define RCC_APB2_PRESCALAR_1								((u32)~(0x2000))
#define RCC_APB2_PRESCALAR_2								((u32)(0x2000))
#define RCC_APB2_PRESCALAR_4								((u32)0x2800)
#define RCC_APB2_PRESCALAR_8								((u32)0x3000)
#define RCC_APB2_PRESCALAR_16							((u32)0x3800)

#define RCC_ADC_PRESCALAR_2								((u32)~(0xC000))
#define RCC_ADC_PRESCALAR_4								((u32)0x4000)
#define RCC_ADC_PRESCALAR_6								((u32)0x8000)
#define RCC_ADC_PRESCALAR_8								((u32)0xC000)

/***************************/
/********Peripherals**********/
/*APB2*/
#define RCC_MASK_PERIPHERALS_APB2		 ((u32) 0x10000000)
#define RCC_PERIPHERALS_PORTA					 ((u32) 0x10000004)
#define RCC_PERIPHERALS_PORTB					 ((u32) 0x10000008)
#define RCC_PERIPHERALS_PORTC					 ((u32) 0x10000010)
#define RCC_PERIPHERALS_PORTD					 ((u32) 0x10000020)
#define RCC_PERIPHERALS_PORTE					 ((u32) 0x10000040)
#define RCC_PERIPHERALS_PORTF					 ((u32) 0x10000080)
#define RCC_PERIPHERALS_PORTG					 ((u32) 0x10000100)
#define RCC_PERIPHERALS_PORTH					 ((u32) 0x10000200)
#define RCC_PERIPHERALS_USART1				 ((u32) 0x10004000)
#define RCC_PERIPHERALS_SPI1	 ((u32) 0x10001000)
#define RCC_PERIPHERALS_TIM1	 ((u32) 0x10000800)
#define RCC_PERIPHERALS_ADC1		 ((u32) 0x10000200)
#define RCC_PERIPHERALS_ADC2		 ((u32) 0x10000400)
/*APB1*/
#define RCC_MASK_PERIPHERALS_APB1		 ((u32) 0xC0000000)
#define RCC_PERIPHERALS_TIM2					 ((u32) 0xC0000001)
#define RCC_PERIPHERALS_TIM3					 ((u32) 0xC0000002)
#define RCC_PERIPHERALS_TIM4					 ((u32) 0xC0000004)
#define RCC_PERIPHERALS_TIM5					 ((u32) 0xC0000008)
#define RCC_PERIPHERALS_TIM6					 ((u32) 0xC0000010)
#define RCC_PERIPHERALS_TIM7					 ((u32) 0xC0000020)
#define RCC_PERIPHERALS_TIM12						((u32) 0xC0000040)
#define RCC_PERIPHERALS_TIM13						((u32) 0xC0000080)
#define RCC_PERIPHERALS_TIM14						((u32) 0xC0000100)
#define RCC_PERIPHERALS_WWDGEN      		((u32) 0xC0000800)
#define RCC_PERIPHERALS_SPI2			       ((u32) 0xC0004000)
#define RCC_PERIPHERALS_SPI3			       ((u32) 0xC0008000)
#define RCC_PERIPHERALS_USART2			      ((u32) 0xC0020000)
#define RCC_PERIPHERALS_USART3			      ((u32) 0xC0040000)
#define RCC_PERIPHERALS_UART4				       ((u32) 0xC0080000)
#define RCC_PERIPHERALS_UART5				       ((u32) 0xC0100000)
#define RCC_PERIPHERALS_I2C1			        ((u32) 0xC0200000)
#define RCC_PERIPHERALS_I2C2			        ((u32) 0xC0400000)
#define RCC_PERIPHERALS_USB			        ((u32) 0xC0800000)
#define RCC_PERIPHERALS_CAN			        ((u32) 0xC2000000)
#define RCC_PERIPHERALS_BKP			        ((u32) 0xC8000000)
#define RCC_PERIPHERALS_PWR			        ((u32) 0xD0000000)
#define RCC_PERIPHERALS_DAC			        ((u32) 0xE0000000)
/*AHB*/
#define RCC_MASK_PERIPHERALS_AHB		 ((u32) 0x20000000)
#define RCC_PERIPHERALS_DMA1		 		((u32) 0x20000001)
#define RCC_PERIPHERALS_DMA2				 ((u32) 0x20000002)
#define RCC_PERIPHERALS_SRAM		 	((u32) 0x20000004)
#define RCC_PERIPHERALS_FLITF		 	((u32) 0x20000010)
#define RCC_PERIPHERALS_CRC	 	((u32) 0x20000040)
#define RCC_PERIPHERALS_FSMC		 ((u32) 0x20000100)
#define RCC_PERIPHERALS_SDIO		 ((u32) 0x20000400)
/****************************************************/
/***********End of Macros to be used by user************************/


/********************************************************************/
/*****************************Registers*****************************/
/*Base address for RCC which will be used to reach peripherals by adding offset to it*/
#define 		RCC_BASE_ADDRESS 										(u32)0x40021000
/*Base Address for the System Control Block (SCB), which we will use to perform reset*/
#define 		RCC_SCB_BASE_ADDRESS									(u32)0xE000ED00

/*Offset Addresses for peripherals*/
#define 		RCC_CR 													*((u32 volatile*)(RCC_BASE_ADDRESS+0x00))
#define 		RCC_CFGR 												*((u32 volatile*)(RCC_BASE_ADDRESS+0x04))
#define 		RCC_CIR 												*((u32 volatile*)(RCC_BASE_ADDRESS+0x08))
#define 		RCC_APB2RSTR											*((u32 volatile*)(RCC_BASE_ADDRESS+0x0C))
#define 		RCC_APB1RSTR											*((u32 volatile*)(RCC_BASE_ADDRESS+0x10))
#define 		RCC_AHBENR												*((u32 volatile*)(RCC_BASE_ADDRESS+0x14))
#define 		RCC_APB2ENR												*((u32 volatile*)(RCC_BASE_ADDRESS+0x18))
#define 		RCC_APB1ENR												*((u32 volatile*)(RCC_BASE_ADDRESS+0x1C))
#define 		RCC_BDCR												*((u32 volatile*)(RCC_BASE_ADDRESS+0x20))
#define 		RCC_CSR													*((u32 volatile*)(RCC_BASE_ADDRESS+0x24))
#define 		RCC_SCB_AIRCR											*((u32 volatile*)(RCC_SCB_BASE_ADDRESS+0x0C))
/******************************************************************/

/*******************************Masks******************************/
/****************************CR Masks******************************/
#define RCC_CR_CLEAR_MASK								((u32)0x0)													
/****************************CRGR Masks****************************/					
#define RCC_SW_CLEAR_MASK								((u32)0x3)

#define RCC_SWS_HSI_MASK								((u32)0b1100)
#define RCC_SWS_HSE_MASK								((u32)0x4)
#define RCC_SWS_PLL_MASK								((u32)0x8)

#define RCC_MCO_CLEAR										((u32)0xF0FFFFFF)


#define RCC_PLL_CLEAR									((u32)0xFFC0FFFF)
				

/******************************************************************/
/**********************Reset Mask**********************************/
#define 		RCC_RESET_MASK						((u32)(0x05FA0004))
/******************************************************************/


/*Description: This API will either enable or disable the desired clock according to parameters passed
Parameters: Desired Status (u32)
Return: None
*/
extern void RCC_voidSetClockStatus(u32 Copy_u32DesiredClockStatus);

/*Description: This API will get the status of a clock whether it is enabled or disabled
Parameters: Desired Clock (u32)
Return: Status of clock*/
extern u8 RCC_u8GetClockStatus(u32 Copy_u32DesiredClock);

/*Description: This API will be used by
the user to select HSI as the System Clock 
Parameters: Desired Clock (u32)
Return: None*/
extern void RCC_voidSWSelectClock(u32 Copy_u32Clock);

/*Description: This API will be used by
the user to get current working clock, it 
will not take input from user and returns
the state as u8								*/
extern u8 RCC_u8GetSWSStatus(void);

/*Description: This API will be used by
the user to enable ports clock
INPUT: Desired Peripheral Name (u32)
Return: None*/
extern void RCC_voidEnablePeripheralClock(u32 Copy_u32Peripheral);

/*Description: This API will be used by
the user to disable ports clock
INPUT: Desired Peripheral Name(u32)
Return: None*/
extern void RCC_voidDisablePeripheralClock(u32 Copy_u32Peripheral);


/*Description: This API will be used to determine configuration
of MCO
Input: Desired clock (u32)
Return: None
*/
extern void RCC_voidMCOClock (u32 Copy_u32Clock);

/*Description: This API will be used to determine configuration for the PLL clock
Inputs: 1) Source Clock
			2) HSE Divider (Choose no divider in case HSE was not chosen)
			3) PLL Multiplier
Return: void*/
extern void RCC_voidPLLConfig (u32 Copy_u32SourceClock, u32 Copy_u32HSEDivider, u32 Copy_u32PLLMultiplier);

/*Description: This API will be used to change prescalar on a specific bus
 * Parameters: Desired prescalar configuration
 * Return: void
 * */
extern void RCC_voidChangeBusPrescalar (u32 Copy_u32DesiredConfiguration);

/*Description: This API will be used to perform system reset
 * Parameters: None
 * Return: void*/
extern void RCC_voidSystemReset (void);


#endif
