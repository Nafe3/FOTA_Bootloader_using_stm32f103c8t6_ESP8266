/*NVIC DRIVER FOR STM32
VERSION: 1.1
AUTHOR: MAHMOUD HAMDY*/

/*
Changelog from version 1.0:
1) Fixed base address for SCB_AIRCR register
2) Fixed minor bug in function determining preemption
*/


#include "STD_TYPES.h"
#include "NVIC_interface.h"

/*Macros*/
#define NVIC_PERIPHERALS_PER_REGISTER								(u8)32
#define NVIC_PERIPHERALS_PER_PRIORITY_REGISTER				(u8)4
#define NVIC_SET_VALUE															(u8)0b1
#define NVIC_RESET_VALUE														(u8)0b1
#define NVIC_BASE_SHIFT_VALUE													(u8)4
#define NVIC_BASE_BYTE_VALUE													(u8)8


#define NVIC_IPR0_MASK															(u8)0
#define NVIC_IPR1_MASK															(u8)1
#define NVIC_IPR2_MASK															(u8)2
#define NVIC_IPR3_MASK															(u8)3
#define NVIC_IPR4_MASK															(u8)4
#define NVIC_IPR5_MASK															(u8)5
#define NVIC_IPR6_MASK															(u8)6
#define NVIC_IPR7_MASK															(u8)7
#define NVIC_IPR8_MASK															(u8)8
#define NVIC_IPR9_MASK															(u8)9
#define NVIC_IPR10_MASK															(u8)10
#define NVIC_IPR11_MASK															(u8)11
#define NVIC_IPR12_MASK															(u8)12
#define NVIC_IPR13_MASK															(u8)13
#define NVIC_IPR14_MASK															(u8)14

/*This value must be written with the desired value to AIRCR, otherwise the processor ignores the operation*/
#define NVIC_VECTKEY_MASK														(u32)0x5FA00000

/*This Macro will be used to shift Group masks to their location in the register (which is 2 bytes)*/
#define NVIC_GROUPS_SHIFT_VALUE													(u8)8


/*APIs*/
/*Description: This API will be used to enable interrupt on a certain peripheral
Parameters:The number of desired peripheral (u8)
Return:Error Status (u8)*/
u8 NVIC_u8EnableInterrupt(u8 Copy_u8PeripheralNumber)
{
	/*Check chosen peripheral number and according to it choose the corresponding register*/
	if (Copy_u8PeripheralNumber<NVIC_PERIPHERALS_PER_REGISTER)
	{
		/*Write inside the register the value that causes the enable shifter by the number of the peripheral */
		*(NVIC_ISER0) |= (NVIC_SET_VALUE<<Copy_u8PeripheralNumber);
		return STATUS_OK;
	}
	/*Since peripheral number is greater than the number of peripherals per register, check whether it is in the second or the third register*/
	else if (Copy_u8PeripheralNumber>=NVIC_PERIPHERALS_PER_REGISTER && Copy_u8PeripheralNumber<(NVIC_PERIPHERALS_PER_REGISTER*2) )
	{
		/*Subtract the number of peripheral by the number of peripherals per register so that we can use it in the shift operation*/
		Copy_u8PeripheralNumber = Copy_u8PeripheralNumber-NVIC_PERIPHERALS_PER_REGISTER;
		/*Write inside the register the value that causes the enable shifter by the number of the peripheral */
		*(NVIC_ISER1) |= (NVIC_SET_VALUE<<Copy_u8PeripheralNumber);
		return STATUS_OK;
	}
	
		/*Since peripheral number is greater than the number of peripherals per register, check whether it is in the second or the third register*/
	else if (Copy_u8PeripheralNumber>=NVIC_PERIPHERALS_PER_REGISTER && Copy_u8PeripheralNumber<(NVIC_PERIPHERALS_PER_REGISTER*3) )
	{
		/*Subtract the number of peripheral by the number of peripherals per register multiplied by two so that we can use it in the shift operation*/
		Copy_u8PeripheralNumber = Copy_u8PeripheralNumber-(NVIC_PERIPHERALS_PER_REGISTER*2);
		/*Write inside the register the value that causes the enable shifter by the number of the peripheral */
		*(NVIC_ISER2) |= (NVIC_SET_VALUE<<Copy_u8PeripheralNumber);
		return STATUS_OK;
	}
	
	/*If nothing of the above was executed, return Status Not Ok*/
	return STATUS_NOK;
}

/*Description: This API will be used to disable interrupt on a certain peripheral
Parameters:The number of desired peripheral (u8)
Return:Error Status (u8)*/
u8 NVIC_u8DisableInterrupt(u8 Copy_u8PeripheralNumber)
{
	/*Check chosen peripheral number and according to it choose the corresponding register*/
	if (Copy_u8PeripheralNumber<NVIC_PERIPHERALS_PER_REGISTER)
	{
		/*Write inside the register the value that causes the enable shifter by the number of the peripheral */
		*(NVIC_ICER0) |= (NVIC_SET_VALUE<<Copy_u8PeripheralNumber);
		return STATUS_OK;
	}
	/*Since peripheral number is greater than the number of peripherals per register, check whether it is in the second or the third register*/
	else if (Copy_u8PeripheralNumber>=NVIC_PERIPHERALS_PER_REGISTER && Copy_u8PeripheralNumber<(NVIC_PERIPHERALS_PER_REGISTER*2) )
	{
		/*Subtract the number of peripheral by the number of peripherals per register so that we can use it in the shift operation*/
		Copy_u8PeripheralNumber = Copy_u8PeripheralNumber-NVIC_PERIPHERALS_PER_REGISTER;
		/*Write inside the register the value that causes the enable shifter by the number of the peripheral */
		*(NVIC_ICER1) |= (NVIC_SET_VALUE<<Copy_u8PeripheralNumber);
		return STATUS_OK;
	}
	
		/*Since peripheral number is greater than the number of peripherals per register, check whether it is in the second or the third register*/
	else if (Copy_u8PeripheralNumber>=NVIC_PERIPHERALS_PER_REGISTER && Copy_u8PeripheralNumber<(NVIC_PERIPHERALS_PER_REGISTER*3) )
	{
		/*Subtract the number of peripheral by the number of peripherals per register multiplied by two so that we can use it in the shift operation*/
		Copy_u8PeripheralNumber = Copy_u8PeripheralNumber-(NVIC_PERIPHERALS_PER_REGISTER*2);
		/*Write inside the register the value that causes the enable shifter by the number of the peripheral */
		*(NVIC_ICER2) |= (NVIC_SET_VALUE<<Copy_u8PeripheralNumber);
		return STATUS_OK;
	}
	
	/*If nothing of the above was executed, return Status Not Ok*/
	return STATUS_NOK;
}

/*Description: This API will be used to set pending flag of a certain peripheral
Parameters:The number of desired peripheral (u8)
Return:Error Status (u8)*/
u8 NVIC_u8SetPendingFlag(u8 Copy_u8PeripheralNumber)
{
	/*Check chosen peripheral number and according to it choose the corresponding register*/
	if (Copy_u8PeripheralNumber<NVIC_PERIPHERALS_PER_REGISTER)
	{
		/*Write inside the register the value that causes the enable shifter by the number of the peripheral */
		*(NVIC_ISPR0) |= (NVIC_SET_VALUE<<Copy_u8PeripheralNumber);
		return STATUS_OK;
	}
	/*Since peripheral number is greater than the number of peripherals per register, check whether it is in the second or the third register*/
	else if (Copy_u8PeripheralNumber>=NVIC_PERIPHERALS_PER_REGISTER && Copy_u8PeripheralNumber<(NVIC_PERIPHERALS_PER_REGISTER*2) )
	{
		/*Subtract the number of peripheral by the number of peripherals per register so that we can use it in the shift operation*/
		Copy_u8PeripheralNumber = Copy_u8PeripheralNumber-NVIC_PERIPHERALS_PER_REGISTER;
		/*Write inside the register the value that causes the enable shifter by the number of the peripheral */
		*(NVIC_ISPR1) |= (NVIC_SET_VALUE<<Copy_u8PeripheralNumber);
		return STATUS_OK;
	}

	/*Since peripheral number is greater than the number of peripherals per register, check whether it is in the second or the third register*/
	else if (Copy_u8PeripheralNumber>=NVIC_PERIPHERALS_PER_REGISTER && Copy_u8PeripheralNumber<(NVIC_PERIPHERALS_PER_REGISTER*3) )
	{
		/*Subtract the number of peripheral by the number of peripherals per register multiplied by two so that we can use it in the shift operation*/
		Copy_u8PeripheralNumber = Copy_u8PeripheralNumber-(NVIC_PERIPHERALS_PER_REGISTER*2);
		/*Write inside the register the value that causes the enable shifter by the number of the peripheral */
		*(NVIC_ISPR2) |= (NVIC_SET_VALUE<<Copy_u8PeripheralNumber);
		return STATUS_OK;
	}

	/*If nothing of the above was executed, return Status Not Ok*/
	return STATUS_NOK;
}

/*Description: This API will be used to reset pending flag of a certain peripheral
Parameters:The number of desired peripheral (u8)
Return:Error Status (u8)*/
u8 NVIC_u8ResetPendingFlag(u8 Copy_u8PeripheralNumber)
{
	/*Check chosen peripheral number and according to it choose the corresponding register*/
	if (Copy_u8PeripheralNumber<NVIC_PERIPHERALS_PER_REGISTER)
	{
		/*Write inside the register the value that causes the enable shifter by the number of the peripheral */
		*(NVIC_ICPR0) |= (NVIC_SET_VALUE<<Copy_u8PeripheralNumber);
		return STATUS_OK;
	}
	/*Since peripheral number is greater than the number of peripherals per register, check whether it is in the second or the third register*/
	else if (Copy_u8PeripheralNumber>=NVIC_PERIPHERALS_PER_REGISTER && Copy_u8PeripheralNumber<(NVIC_PERIPHERALS_PER_REGISTER*2) )
	{
		/*Subtract the number of peripheral by the number of peripherals per register so that we can use it in the shift operation*/
		Copy_u8PeripheralNumber = Copy_u8PeripheralNumber-NVIC_PERIPHERALS_PER_REGISTER;
		/*Write inside the register the value that causes the enable shifter by the number of the peripheral */
		*(NVIC_ICPR1) |= (NVIC_SET_VALUE<<Copy_u8PeripheralNumber);
		return STATUS_OK;
	}

	/*Since peripheral number is greater than the number of peripherals per register, check whether it is in the second or the third register*/
	else if (Copy_u8PeripheralNumber>=NVIC_PERIPHERALS_PER_REGISTER && Copy_u8PeripheralNumber<(NVIC_PERIPHERALS_PER_REGISTER*3) )
	{
		/*Subtract the number of peripheral by the number of peripherals per register multiplied by two so that we can use it in the shift operation*/
		Copy_u8PeripheralNumber = Copy_u8PeripheralNumber-(NVIC_PERIPHERALS_PER_REGISTER*2);
		/*Write inside the register the value that causes the enable shifter by the number of the peripheral */
		*(NVIC_ICPR2) |= (NVIC_SET_VALUE<<Copy_u8PeripheralNumber);
		return STATUS_OK;
	}

	/*If nothing of the above was executed, return Status Not Ok*/
	return STATUS_NOK;
}

/*Description: This API will be used to set priority of a certain peripheral
Parameters:The number of desired peripheral (u8), Desired Priority (u8)
Return:Error Status (u8)*/
u8 NVIC_u8SetPriority(u8 Copy_u8PeripheralNumber, u8 Copy_u8PeripheralPriority)
{
	/*This variable will hold the priority register number which contains the desired peripheral*/
	u8 Local_u8PriorityRegisterNumber;
	/*This variable will hold the number of peripheral inside its corressponding register*/
	u8 Local_u8PeripheralNumberInsideRegister;
	/*This variable will hold the value by which the priority will be shifted with to write on corressponding peripheral*/
	u8 Local_u8ShiftValue;
	
	/*From the number of peripheral, check which register contains the peripheral where each register contains 4 peripherals*/
	Local_u8PriorityRegisterNumber = (Copy_u8PeripheralNumber/NVIC_PERIPHERALS_PER_PRIORITY_REGISTER);
	/*Find number of peripheral inside its corressponding register*/
	Local_u8PeripheralNumberInsideRegister = (Copy_u8PeripheralNumber - (Local_u8PriorityRegisterNumber * NVIC_PERIPHERALS_PER_PRIORITY_REGISTER));

	/*Calculate the value by which the shift will be done by moving by the value of base shift (which is 4
	 * because the least significant 4 bits are closed
	 * and add to it the offset of one byte multiplied by number of peripheral inside register*/
	Local_u8ShiftValue = NVIC_BASE_SHIFT_VALUE + (NVIC_BASE_BYTE_VALUE*Local_u8PeripheralNumberInsideRegister);

	/*According to the number saved in the local variable, go to the corresponding register and write priority inside it*/
	if (Local_u8PriorityRegisterNumber == NVIC_IPR0_MASK)
	{
		*(NVIC_IPR0) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	else if (Local_u8PriorityRegisterNumber == NVIC_IPR1_MASK)
	{
		*(NVIC_IPR1) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	else if (Local_u8PriorityRegisterNumber == NVIC_IPR2_MASK)
	{
		*(NVIC_IPR2) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	else if (Local_u8PriorityRegisterNumber == NVIC_IPR3_MASK)
	{
		*(NVIC_IPR3) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	else if (Local_u8PriorityRegisterNumber == NVIC_IPR4_MASK)
	{
		*(NVIC_IPR4) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	else if (Local_u8PriorityRegisterNumber == NVIC_IPR5_MASK)
	{
		*(NVIC_IPR5) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	else if (Local_u8PriorityRegisterNumber == NVIC_IPR6_MASK)
	{
		*(NVIC_IPR6) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	else if (Local_u8PriorityRegisterNumber == NVIC_IPR7_MASK)
	{
		*(NVIC_IPR7) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	else if (Local_u8PriorityRegisterNumber == NVIC_IPR8_MASK)
	{
		*(NVIC_IPR8) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	else if (Local_u8PriorityRegisterNumber == NVIC_IPR9_MASK)
	{
		*(NVIC_IPR9) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	else if (Local_u8PriorityRegisterNumber == NVIC_IPR10_MASK)
	{
		*(NVIC_IPR10) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	else if (Local_u8PriorityRegisterNumber == NVIC_IPR11_MASK)
	{
		*(NVIC_IPR11) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	else if (Local_u8PriorityRegisterNumber == NVIC_IPR12_MASK)
	{
		*(NVIC_IPR12) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	else if (Local_u8PriorityRegisterNumber == NVIC_IPR13_MASK)
	{
		*(NVIC_IPR13) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	else if (Local_u8PriorityRegisterNumber == NVIC_IPR14_MASK)
	{
		*(NVIC_IPR14) |= (Copy_u8PeripheralPriority<<Local_u8ShiftValue);
		return STATUS_OK;
	}
	/*If the program didn't exit correctly, return that status is not OK*/
	return STATUS_NOK;
}

/*Description: This API will be used to know if a current interrupt is active or not
Parameters: Desired peripheral (u8), pointer to variable that will hold status
Return: Error Status (u8) */
u8 NVIC_u8GetActiveStatus(u8 Copy_u8PeripheralNumber, u32* Copy_u32StatusVariable)
{

	/*Check chosen peripheral number and according to it choose the corresponding register*/
	if (Copy_u8PeripheralNumber < NVIC_PERIPHERALS_PER_REGISTER) {
		/*Check the status of desired peripheral by ANDing the register value with 1 shifter by peripheral Number */
		*Copy_u32StatusVariable = (*(NVIC_ISER0))& (u32)(NVIC_SET_VALUE << Copy_u8PeripheralNumber);
		return STATUS_OK;
	}
	/*Since peripheral number is greater than the number of peripherals per register, check whether it is in the second or the third register*/
	else if (Copy_u8PeripheralNumber >= NVIC_PERIPHERALS_PER_REGISTER && Copy_u8PeripheralNumber < (NVIC_PERIPHERALS_PER_REGISTER * 2)) {
		/*Subtract the number of peripheral by the number of peripherals per register so that we can use it in the shift operation*/
		Copy_u8PeripheralNumber = Copy_u8PeripheralNumber - NVIC_PERIPHERALS_PER_REGISTER;
		/*Check the status of desired peripheral by ANDing the register value with 1 shifter by peripheral Number */
		*Copy_u32StatusVariable = (*(NVIC_ISER1))&(u32)(NVIC_SET_VALUE << Copy_u8PeripheralNumber);
		return STATUS_OK ;
	}

	/*Since peripheral number is greater than the number of peripherals per register, check whether it is in the second or the third register*/
	else if (Copy_u8PeripheralNumber >= NVIC_PERIPHERALS_PER_REGISTER && Copy_u8PeripheralNumber < (NVIC_PERIPHERALS_PER_REGISTER * 3)) {
		/*Subtract the number of peripheral by the number of peripherals per register multiplied by two so that we can use it in the shift operation*/
		Copy_u8PeripheralNumber = Copy_u8PeripheralNumber - (NVIC_PERIPHERALS_PER_REGISTER * 2);
		/*Check the status of desired peripheral by ANDing the register value with 1 shifter by peripheral Number */
		*Copy_u32StatusVariable = (*(NVIC_ISER2))&(u32)(NVIC_SET_VALUE << Copy_u8PeripheralNumber);
		return STATUS_OK ;
	}


	/*If nothing of the above was executed, return Status Not Ok*/
	return STATUS_NOK ;
}

/*Description: This API will be used to trigger a software interrupt on a certain peripheral
Parameters: Desired Peripheral (u8)
Return: Error Status (u8)*/
u8 NVIC_u8SoftwareInterruptTrigger(u8 Copy_u8IRQNumber)
{
	u8 Local_u8Status = STATUS_NOK;
	/*Write the number of peripheral inside the register */
	*(NVIC_STIR) |= Copy_u8IRQNumber;
	Local_u8Status = STATUS_OK;
	return Local_u8Status;
}

/*Description: This API will be used to select number of groups priority
 * Parameters: Number of groups (u8)
 * Return: Error Status*/
u8 NVIC_u8SelectGroupsNumber (u8 Copy_u8GroupsNumber)
{
	u32 Local_u32DesiredValue = (NVIC_VECTKEY_MASK | Copy_u8GroupsNumber<<NVIC_GROUPS_SHIFT_VALUE);
	/*Write desired value inside the SCB register*/
	*(NVIC_SCB_AIRCR) = Local_u32DesiredValue;
	return STATUS_OK;
}

/*Description: This API will be used to disable all interrupts except NMI and hard faults
 * Parameters: none
 * Return: Error Status (u8)*/
u8 NVIC_u8SetPRIMASK(void)
{
	/*Write assembly instruction to the PRIMASK register with a value of "1"
	 * This asm instruction was taken directly from M3 Guide*/
	asm volatile ("MSR primask, %0" : : "r" (1) : "memory");
	return STATUS_OK;
}

/*Description: This API will be used to enable all interrupts
 * Parameters: none
 * Return: Error Status (u8)*/
u8 NVIC_u8ResetPRIMASK(void)
{
	/*Write assembly instruction to the PRIMASK register with a value of "0"
	 * This asm instruction was taken directly from M3 Guide*/
	asm volatile ("MSR primask, %0" : : "r" (0) : "memory");
	return STATUS_OK;
}

/*Description: This API will be used to get the status of PRIMASK whether set or reset
 * Parameters: pointer to variable to hold status (u32*)
 * Return: Error Status (u8)*/
u8 NVIC_u8GetPRIMASKStatus(u32* Copy_u32Status)
{
	u32 Local_u32HolderVariable;
	asm volatile ("MRS %0, primask" : "=r" (Local_u32HolderVariable) );
	*Copy_u32Status = Local_u32HolderVariable;
	return STATUS_OK;
}

/*Description: This API will be used to disable all interrupts starting from a certain level
 * Parameters: Priority wanted to be the base (u8)
 * Return: Error Status (u8)*/
u8 NVIC_u8SetBASEPRI(u8 Copy_u8BasePriority)
{
	/*Write assembly instruction to the BASEPRI register with a value of "1"
	 * This asm instruction was taken directly from M3 Guide*/
	asm volatile ("MSR basepri, %0" : : "r" (Copy_u8BasePriority) : "memory");
	return STATUS_OK;
}

/*Description: This API will be used to enable all interrupts
 * Parameters: none
 * Return: Error Status (u8)*/
u8 NVIC_u8ResetBASEPRI(void)
{
	/*Write assembly instruction to the BASEPRI register with a value of "0"
	 * This asm instruction was taken directly from M3 Guide*/
	asm volatile ("MSR basepri, %0" : : "r" (0) : "memory");
	return STATUS_OK;
}

/*Description: This API will be used to get the status of BASEPRI whether set or reset
 * Parameters: pointer to variable to hold status (u32*)
 * Return: Error Status (u8)*/
u8 NVIC_u8GetBASEPRIStatus(u32* Copy_u32Status)
{
	u32 Local_u32HolderVariable;
	asm volatile ("MRS %0, basepri" : "=r" (Local_u32HolderVariable) );
	*Copy_u32Status = Local_u32HolderVariable;
	return STATUS_OK;
}

/*Description: This API will be used to disable all interrupts except NMI
 * Parameters: none
 * Return: Error Status (u8)*/
u8 NVIC_u8SetFAULTMASK(void)
{
	/*Write assembly instruction to the PRIMASK register with a value of "1"
	 * This asm instruction was taken directly from M3 Guide*/
	asm volatile ("MSR faultmask, %0" : : "r" (1) : "memory");
	return STATUS_OK;
}

/*Description: This API will be used to enable all interrupts
 * Parameters: none
 * Return: Error Status (u8)*/
u8 NVIC_u8ResetFAULTMASK(void)
{
	/*Write assembly instruction to the PRIMASK register with a value of "0"
	 * This asm instruction was taken directly from M3 Guide*/
	asm volatile ("MSR faultmask, %0" : : "r" (0) : "memory");
	return STATUS_OK;
}

/*Description: This API will be used to get the status of PRIMASK whether set or reset
 * Parameters: pointer to variable to hold status (u32*)
 * Return: Error Status (u8)*/
u8 NVIC_u8GetFAULTMASKStatus(u32* Copy_u32Status)
{
	u32 Local_u32HolderVariable;
	asm volatile ("MRS %0, faultmask" : "=r" (Local_u32HolderVariable) );
	*Copy_u32Status = Local_u32HolderVariable;
	return STATUS_OK;
}
