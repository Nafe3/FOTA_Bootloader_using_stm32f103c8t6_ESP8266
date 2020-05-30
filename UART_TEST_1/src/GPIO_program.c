/*****************************************************/
/*Author  : Mahmoud Hamdy							               */
/*Version : V02			 							                   */
/*Date	  :2020-03-01								*/
/*****************************************************/

/*Changelog from previous version
1) Changed the way the user passes arguments to functions, where now these arguments are macros that represent the base address of the GPIO
2) Modified Macros to match the way the functions will work now with
3) Added new functionalities
*/
#include "GPIO_interface.h"

/*PORT Registers Offsets*/
#define GPIO_CRL										(u32)0x00
#define GPIO_CRH										(u32)0x04
#define GPIO_IDR										(u32)0x08
#define GPIO_ODR										(u32)0x0C
#define GPIO_BSRR									(u32)0x10
#define GPIO_BRR										(u32)0x14
#define GPIO_LCK										(u32)0x18

	
/*Shift Mask*/
/*This shift mask will be used in shifting desired pin in register. It will be multiplied by pin number*/
#define GPIO_SHIFT_MASK										(u8)4

/*Pin clear Mask*/
/*This clear mask will be used by shifting it to the desired pin inside the register, then 
getting its NOT and AND with desired register*/
#define GPIO_CLEAR_MASK											(u32)0xF
	
/*Pins Read/write Mask*/
/*This mask will be used with the following registers
IDR: Reading a specific Pin
BSRR: Setting a specific pin
BRR: Resetting a specific pin*/
#define GPIO_RW_MASK													(u32)0b1


#define GPIO_PINS_PER_REGISTER									(u8)8

/*Functions*/
/*Description: This API will be used to configure specific pin on a specific port
Input: Pointer to desired GPIO struct
Return: None*/
void GPIO_voidConfigurePin(GPIO_t* DesiredGPIO)
{
	/*This variable will hold the value that we want to write inside the register*/
	u32 Local_u32DesiredConfiguration;

	/*This pointer will hold the addresses for the port to be written on*/
	u32 *Local_u32PortRegister;
	
	/*This variable will hold pin number after subtracing it from number of ports in register
	in case the pin will be put in the CRH instead of CRL
	If it will be put in CRL, it will be as it is*/
	u8 Local_u8PinValue;
	
	/*If pin number is less than pin #8, then we will write in CRL*/
	if ((DesiredGPIO->pin)<GPIO_PIN8)
	{
		
		Local_u32PortRegister = (u32)((DesiredGPIO->port) + GPIO_CRL);
		Local_u8PinValue = (u8)(DesiredGPIO->pin);
	}
	/*If pin number is greater than pin #8, then we will write in CRH and subract the number that will be 
	written in CRH by number of pins per register*/
	else
	{
		Local_u32PortRegister = (u32)((DesiredGPIO->port) + GPIO_CRH);
		Local_u8PinValue = (u8)((DesiredGPIO->pin) - GPIO_PINS_PER_REGISTER);
	}
	/*We will reset desired pin bits so that it won't conflict with new configurations*/
	*(Local_u32PortRegister) &= ~ (GPIO_CLEAR_MASK<<(Local_u8PinValue * GPIO_SHIFT_MASK));

	/*Write speed configuration into local variable*/
	Local_u32DesiredConfiguration = (DesiredGPIO->speed);

	/*Check desired mode and OR it with the variable
	In case the mode is Input Analog or Output push pull, they will need to be AND with the variable instead*/
	if (((DesiredGPIO->mode)==GPIO_MODE_INPUT_ANALOG) ||((DesiredGPIO->mode)==GPIO_MODE_OUTPUT_PUSHPULL))
	{
		Local_u32DesiredConfiguration &= (DesiredGPIO->mode);
	}
	else
	{
		Local_u32DesiredConfiguration |= (DesiredGPIO->mode);
	}
	
	/*Write configuration inside desired register by shifting it by (pin number  x shift mask)*/
	*(Local_u32PortRegister) |= (Local_u32DesiredConfiguration<<(Local_u8PinValue*GPIO_SHIFT_MASK));
}/*End of GPIO_voidConfigurePin*/

/*Description: This API will be used to get value of a specific pin through the IDR register
Input: Desired port (u8), desired pin (u8)
Return: Pin Status (The value inside IDR)
*/
u16 GPIO_u16GetPinStatus (GPIO_t* DesiredGPIO)
{
	/*This pointer will hold the addresses for the port register that we need*/
	u32 *Local_u32IDRRegister = (u32)((DesiredGPIO->port) + GPIO_IDR);
	/*Read desired pin by AND with the RW mask after shifting it to the desired location and return the result*/
	return (*(Local_u32IDRRegister) & (GPIO_RW_MASK<<(DesiredGPIO->pin)));
}/*End of GPIO_u32GetPinStatus*/

/*Description: This API will be used to write desired value on a pin through either BSR or BSRR register
BSR will set the desired pin
BSRR will reset the desired pin
Parameters: Desired pin (pointer to struct), Desired value to be written (u8)
Return: None*/
void GPIO_voidWriteOutput (GPIO_t* DesiredGPIO, u8 Copy_u8DesiredValue)
{
	/*This pointer will hold the register that we want to write on whether it is BSRR or BRR*/\
	u32 *Local_u32DesiredRegister;
	/*If user wants to set bit, then we will write to BSR, in case of reset, we will write to BRR*/
	if (Copy_u8DesiredValue == GPIO_SET_PIN)
	{
		Local_u32DesiredRegister = (u32)((DesiredGPIO->port) + GPIO_BSRR);
	}
	else if (Copy_u8DesiredValue == GPIO_RESET_PIN)
	{
		Local_u32DesiredRegister = (u32)((DesiredGPIO->port) + GPIO_BRR);
	}
	/*Write the desired value to the register*/
	*(Local_u32DesiredRegister) |= (GPIO_RW_MASK <<(DesiredGPIO->pin));
}/*END of GPIO_voidWriteOutput */

