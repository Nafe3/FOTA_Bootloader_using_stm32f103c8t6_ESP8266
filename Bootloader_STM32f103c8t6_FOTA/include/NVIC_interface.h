/*NVIC DRIVER FOR STM32
VERSION: 1.1
AUTHOR: MAHMOUD HAMDY*/


/*
Changelog from version 1.0:
1) Fixed base address for SCB_AIRCR register
2) Fixed minor bug in function determining preemption
*/

/*Peripherals Selections*/
#define NVIC_WATCHDOG							(u8)0
#define NVIC_PVD										(u8)1
#define NVIC_TAMPER								(u8)2
#define NVIC_RTC										(u8)3
#define NVIC_FLASH									(u8)4
#define NVIC_RCC										(u8)5
#define NVIC_EXTI0									(u8)6
#define NVIC_EXTI1									(u8)7
#define NVIC_EXTI2									(u8)8
#define NVIC_EXTI3									(u8)9
#define NVIC_EXTI4									(u8)10
#define NVIC_DMA1_CHANNEL1					(u8)11
#define NVIC_DMA1_CHANNEL2					(u8)12
#define NVIC_DMA1_CHANNEL3					(u8)13
#define NVIC_DMA1_CHANNEL4					(u8)14
#define NVIC_DMA1_CHANNEL5					(u8)15
#define NVIC_DMA1_CHANNEL6					(u8)16
#define NVIC_DMA1_CHANNEL7					(u8)17
#define NVIC_ADC1_2								(u8)18
#define NVIC_USB_HP_CAN_TX					(u8)19
#define NVIC_USB_LP_CAN_RX					(u8)20
#define NVIC_CAN_RX1								(u8)21
#define NVIC_CAN_SCE								(u8)22
#define NVIC_EXTI9_5								(u8)23
#define NVIC_TIM1_BRK							(u8)24
#define NVIC_TIM1_UP								(u8)25
#define NVIC_TIM1_TRG_COM					 (u8)26
#define NVIC_TIM1_CC								 (u8)27
#define NVIC_TIM2									 (u8)28
#define NVIC_TIM3									 (u8)29
#define NVIC_TIM4									 (u8)30
#define NVIC_I2C1_EV								 (u8)31
#define NVIC_I2C1_ER								 (u8)32
#define NVIC_I2C2_EV								 (u8)33
#define NVIC_I2C2_ER								 (u8)34
#define NVIC_SPI1									 (u8)35
#define NVIC_SPI2									 (u8)36
#define NVIC_USART1								 (u8)37
#define NVIC_USART2								 (u8)38
#define NVIC_USART3					 			 (u8)39
#define NVIC_EXTI5_10								 (u8)40
#define NVIC_RTCALARM							 (u8)41
#define NVIC_USBWAKEUP						 (u8)42
#define NVIC_TIM8_BRK_TIM12					 (u8)43	
#define NVIC_TIM8_UP_TIM13					 (u8)44	
#define NVIC_TIM8_TRG_COM_TIM14		 (u8)45
#define NVIC_TIM8_CC								 (u8)46
#define NVIC_ADC3									 (u8)47
#define NVIC_FSMC									 (u8)48
#define NVIC_SDIO									 (u8)49
#define NVIC_TIM5								 	 (u8)50
#define NVIC_SPI3								 	 (u8)51
#define NVIC_UART4								 	 (u8)52
#define NVIC_UART5								 	 (u8)53
#define NVIC_TIM6								 	 (u8)54
#define NVIC_TIM7								 	 (u8)55
#define NVIC_DMA2_CHANNEL1					 (u8)56
#define NVIC_DMA2_CHANNEL2					 (u8)57
#define NVIC_DMA2_CHANNEL3					 (u8)58
#define NVIC_DMA2_CHANNEL4_5				 (u8)59





/*Group Priorities */
#define NVIC_GROUPS_0							NVIC_GROUPS_0_MASK
#define NVIC_GROUPS_2							NVIC_GROUPS_2_MASK
#define NVIC_GROUPS_4							NVIC_GROUPS_4_MASK
#define NVIC_GROUPS_8							NVIC_GROUPS_8_MASK
#define NVIC_GROUPS_16							NVIC_GROUPS_16_MASK

/*WARNING: DON'T CHANGE OR MODIFY THE NEXT PART*/
/*********************************************/
/****************REGISTERS********************/
#define NVIC_BASE_ADDRESS						(u32)0xE000E100
/*Set Enable Registers*/
#define NVIC_ISER0										((u32*)(NVIC_BASE_ADDRESS+0x0))
#define NVIC_ISER1										((u32*)(NVIC_BASE_ADDRESS+0x4))
#define NVIC_ISER2										((u32*)(NVIC_BASE_ADDRESS+0x8))
#define NVIC_ISER3										((u32*)(NVIC_BASE_ADDRESS+0xC))
#define NVIC_ISER4										((u32*)(NVIC_BASE_ADDRESS+0x10))
#define NVIC_ISER5										((u32*)(NVIC_BASE_ADDRESS+0x14))
#define NVIC_ISER6										((u32*)(NVIC_BASE_ADDRESS+0x18))
#define NVIC_ISER7										((u32*)(NVIC_BASE_ADDRESS+0x1C))
/*Clear Enable Registers*/
#define NVIC_ICER0										((u32*)(NVIC_BASE_ADDRESS+0x80))
#define NVIC_ICER1										((u32*)(NVIC_BASE_ADDRESS+0x84))
#define NVIC_ICER2										((u32*)(NVIC_BASE_ADDRESS+0x88))
#define NVIC_ICER3										((u32*)(NVIC_BASE_ADDRESS+0x8C))
#define NVIC_ICER4										((u32*)(NVIC_BASE_ADDRESS+0x90))
#define NVIC_ICER5										((u32*)(NVIC_BASE_ADDRESS+0x94))
#define NVIC_ICER6										((u32*)(NVIC_BASE_ADDRESS+0x98))
#define NVIC_ICER7										((u32*)(NVIC_BASE_ADDRESS+0x9C))
/*Set Pending Registers*/
#define NVIC_ISPR0										((u32*)(NVIC_BASE_ADDRESS+0x100))
#define NVIC_ISPR1										((u32*)(NVIC_BASE_ADDRESS+0x104))
#define NVIC_ISPR2										((u32*)(NVIC_BASE_ADDRESS+0x108))
#define NVIC_ISPR3										((u32*)(NVIC_BASE_ADDRESS+0x10C))
#define NVIC_ISPR4										((u32*)(NVIC_BASE_ADDRESS+0x110))
#define NVIC_ISPR5										((u32*)(NVIC_BASE_ADDRESS+0x114))
#define NVIC_ISPR6										((u32*)(NVIC_BASE_ADDRESS+0x118))
#define NVIC_ISPR7										((u32*)(NVIC_BASE_ADDRESS+0x11C))
/*Clear Pending Registers*/
#define NVIC_ICPR0										((u32*)(NVIC_BASE_ADDRESS+0x180))
#define NVIC_ICPR1										((u32*)(NVIC_BASE_ADDRESS+0x184))
#define NVIC_ICPR2										((u32*)(NVIC_BASE_ADDRESS+0x188))
#define NVIC_ICPR3										((u32*)(NVIC_BASE_ADDRESS+0x18C))
#define NVIC_ICPR4										((u32*)(NVIC_BASE_ADDRESS+0x190))
#define NVIC_ICPR5										((u32*)(NVIC_BASE_ADDRESS+0x194))
#define NVIC_ICPR6										((u32*)(NVIC_BASE_ADDRESS+0x198))
#define NVIC_ICPR7										((u32*)(NVIC_BASE_ADDRESS+0x19C))
/*Active Bit Registers (Read Only)*/
#define NVIC_IABR0										((u32*)(NVIC_BASE_ADDRESS+0x200))
#define NVIC_IABR1										((u32*)(NVIC_BASE_ADDRESS+0x204))
#define NVIC_IABR2										((u32*)(NVIC_BASE_ADDRESS+0x208))
#define NVIC_IABR3										((u32*)(NVIC_BASE_ADDRESS+0x20C))
#define NVIC_IABR4										((u32*)(NVIC_BASE_ADDRESS+0x210))
#define NVIC_IABR5										((u32*)(NVIC_BASE_ADDRESS+0x214))
#define NVIC_IABR6										((u32*)(NVIC_BASE_ADDRESS+0x218))
#define NVIC_IABR7										((u32*)(NVIC_BASE_ADDRESS+0x21C))
/*Set priority registers*/
#define NVIC_IPR0										((u32*)(NVIC_BASE_ADDRESS+0x300))
#define NVIC_IPR1										((u32*)(NVIC_BASE_ADDRESS+0x304))
#define NVIC_IPR2										((u32*)(NVIC_BASE_ADDRESS+0x308))
#define NVIC_IPR3										((u32*)(NVIC_BASE_ADDRESS+0x30C))
#define NVIC_IPR4										((u32*)(NVIC_BASE_ADDRESS+0x310))
#define NVIC_IPR5										((u32*)(NVIC_BASE_ADDRESS+0x314))
#define NVIC_IPR6										((u32*)(NVIC_BASE_ADDRESS+0x318))
#define NVIC_IPR7										((u32*)(NVIC_BASE_ADDRESS+0x31C))
#define NVIC_IPR8										((u32*)(NVIC_BASE_ADDRESS+0x320))
#define NVIC_IPR9										((u32*)(NVIC_BASE_ADDRESS+0x324))
#define NVIC_IPR10										((u32*)(NVIC_BASE_ADDRESS+0x328))
#define NVIC_IPR11										((u32*)(NVIC_BASE_ADDRESS+0x32C))
#define NVIC_IPR12										((u32*)(NVIC_BASE_ADDRESS+0x330))
#define NVIC_IPR13										((u32*)(NVIC_BASE_ADDRESS+0x334))
#define NVIC_IPR14										((u32*)(NVIC_BASE_ADDRESS+0x338))
/*Software Trigger Interrupt Registers*/
#define NVIC_STIR										((u32*)(NVIC_BASE_ADDRESS+0xE00))
/*Set grouping and preemption register*/
#define NVIC_SCB_AIRCR									((u32*)(0xE000ED00+0x0C))

/*Group Priorities Masks*/
#define NVIC_GROUPS_0_MASK															(u8)0b111
#define NVIC_GROUPS_2_MASK															(u8)0b110
#define NVIC_GROUPS_4_MASK															(u8)0b101
#define NVIC_GROUPS_8_MASK															(u8)0b100
#define NVIC_GROUPS_16_MASK															(u8)0b011
/****************************************************/
	
/*APIs*/
/*Description: This API will be used to enable interrupt on a certain peripheral
Parameters:The number of desired peripheral (u8)
Return:Error Status (u8)*/
extern u8 NVIC_u8EnableInterrupt(u8 Copy_u8PeripheralNumber);

/*Description: This API will be used to disable interrupt on a certain peripheral
Parameters:The number of desired peripheral (u8)
Return:Error Status (u8)*/
extern u8 NVIC_u8DisableInterrupt(u8 Copy_u8PeripheralNumber);

/*Description: This API will be used to set pending flag of a certain peripheral
Parameters:The number of desired peripheral (u8)
Return:Error Status (u8)*/
extern u8 NVIC_u8SetPendingFlag(u8 Copy_u8PeripheralNumber);

/*Description: This API will be used to reset pending flag of a certain peripheral
Parameters:The number of desired peripheral (u8)
Return:Error Status (u8)*/
extern u8 NVIC_u8ResetPendingFlag(u8 Copy_u8PeripheralNumber);

/*Description: This API will be used to know if a current interrupt is active or not
Parameters: Desired peripheral (u8), pointer to variable that will hold status
Return: Error Status (u8) */
extern u8 NVIC_u8GetActiveStatus(u8 Copy_u8PeripheralNumber, u32* Copy_u32StatusVariable);

/*Description: This API will be used to set priority of a certain peripheral
Parameters:The number of desired peripheral (u8)
Return:Error Status (u8)*/
extern u8 NVIC_u8SetPriority(u8 Copy_u8PeripheralNumber, u8 Copy_u8PeripheralPriority);

/*Description: This API will be used to trigger a software interrupt on a certain peripheral
Parameters: Desired Peripheral (u8)
Return: Error Status (u8)*/
extern u8 NVIC_u8SoftwareInterruptTrigger(u8 Copy_u8PeripheralNumber);

/*Description: This API will be used to select number of groups priority
 * Parameters: Number of groups (u8)
 * Return: Error Status*/
extern u8 NVIC_u8SelectGroupsNumber (u8 Copy_u8GroupsNumber);

/*Description: This API will be used to disable all interrupts except NMI and hard faults
 * Parameters: none
 * Return: Error Status (u8)*/
extern u8 NVIC_u8SetPRIMASK(void);

/*Description: This API will be used to enable all interrupts
 * Parameters: none
 * Return: Error Status (u8)*/
extern u8 NVIC_u8ResetPRIMASK(void);

/*Description: This API will be used to get the status of PRIMASK whether set or reset
 * Parameters: pointer to variable to hold status (u32*)
 * Return: Error Status (u8)*/
extern u8 NVIC_u8GetPRIMASKStatus(u32* Copy_u32Status);

/*Description: This API will be used to disable all interrupts starting from a certain level
 * Parameters: none
 * Return: Error Status (u8)*/
extern u8 NVIC_u8SetBASEPRI(u8 Copy_u8BasePriority);

/*Description: This API will be used to enable all interrupts
 * Parameters: none
 * Return: Error Status (u8)*/
extern u8 NVIC_u8ResetBASEPRI(void);

/*Description: This API will be used to get the status of BASEPRI whether set or reset
 * Parameters: pointer to variable to hold status (u32*)
 * Return: Error Status (u8)*/
extern u8 NVIC_u8GetBASEPRIStatus(u32* Copy_u32Status);

/*Description: This API will be used to disable all interrupts except NMI
 * Parameters: none
 * Return: Error Status (u8)*/
extern u8 NVIC_u8SetFAULTMASK(void);

/*Description: This API will be used to enable all interrupts
 * Parameters: none
 * Return: Error Status (u8)*/
extern u8 NVIC_u8ResetFAULTMASK(void);

/*Description: This API will be used to get the status of PRIMASK whether set or reset
 * Parameters: pointer to variable to hold status (u32*)
 * Return: Error Status (u8)*/
extern u8 NVIC_u8GetFAULTMASKStatus(u32* Copy_u32Status);
