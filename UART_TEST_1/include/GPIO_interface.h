/*****************************************************/
/*Author  : Mahmoud Hamdy							*/
/*Version : V2.0		 							*/
/*Date	  :2020-03-01								*/
/*****************************************************/

/*Changelog from previous version
1) Changed the way the user passes arguments to functions, where now these arguments are macros that represent the base address of the GPIO
2) Modified Macros to match the way the functions will work now with
3) Added new functionalities
*/
#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_

/*Libraries*/
#include "STD_TYPES.h"

/*Macros to be used by the user*/
/***********************************************************************/
/*Ports*/
#define GPIO_PORT_A						GPIO_PORTA_BASE_ADDRESS
#define GPIO_PORT_B						GPIO_PORTB_BASE_ADDRESS
#define GPIO_PORT_C						GPIO_PORTC_BASE_ADDRESS
#define GPIO_PORT_D						GPIO_PORTD_BASE_ADDRESS
#define GPIO_PORT_E						GPIO_PORTE_BASE_ADDRESS
#define GPIO_PORT_F						GPIO_PORTF_BASE_ADDRESS
#define GPIO_PORT_G						GPIO_PORTG_BASE_ADDRESS
#define GPIO_PORT_H						GPIO_PORTH_BASE_ADDRESS
/*Pins*/
#define GPIO_PIN0							(u8)0
#define GPIO_PIN1							(u8)1
#define GPIO_PIN2							(u8)2
#define GPIO_PIN3							(u8)3
#define GPIO_PIN4							(u8)4
#define GPIO_PIN5							(u8)5
#define GPIO_PIN6							(u8)6
#define GPIO_PIN7							(u8)7
#define GPIO_PIN8							(u8)8
#define GPIO_PIN9							(u8)9
#define GPIO_PIN10							(u8)10
#define GPIO_PIN11							(u8)11
#define GPIO_PIN12							(u8)12
#define GPIO_PIN13							(u8)13
#define GPIO_PIN14							(u8)14
#define GPIO_PIN15							(u8)15

/*Modes*/
/*Speed*/
#define GPIO_SPEED_INPUT										GPIO_SPEED_INPUT_MASK
#define GPIO_SPEED_OUTPUT_10								GPIO_SPEED_OUTPUT_10_MASK
#define GPIO_SPEED_OUTPUT_2								GPIO_SPEED_OUTPUT_2_MASK
#define GPIO_SPEED_OUTPUT_50								GPIO_SPEED_OUTPUT_50_MASK
/*Modes*/
#define GPIO_MODE_INPUT_ANALOG							GPIO_MODE_INPUT_ANALOG_MASK
#define GPIO_MODE_INPUT_FLOATING						GPIO_MODE_INPUT_FLOATING_MASK
#define GPIO_MODE_INPUT_PULL								GPIO_MODE_INPUT_PULL_MASK

/*OUTPUT*/
#define GPIO_MODE_OUTPUT_PUSHPULL					GPIO_MODE_OUTPUT_PUSHPULL_MASK
#define GPIO_MODE_OUTPUT_OPENDRAIN					GPIO_MODE_OUTPUT_OPENDRAIN_MASK
#define GPIO_MODE_OUTPUT_AF_PUSHPULL				GPIO_MODE_OUTPUT_AF_PUSHPULL_MASK
#define GPIO_MODE_OUTPUT_AF_OPENDRAIN			GPIO_MODE_OUTPUT_AF_OPENDRAIN_MASK
	
/*Set/Reset*/
#define GPIO_SET_PIN												(u8)1
#define GPIO_RESET_PIN											(u8)0
/***********************************************************************/

/*WARNING: Don't change the next macros and struct unless you are using a different microcontroller*/
/***********************************************************************/
/*Base Addresses of port registers*/
#define GPIO_PORTA_BASE_ADDRESS 					(u32)0x40010800
#define GPIO_PORTB_BASE_ADDRESS 					(u32)0x40010C00
#define GPIO_PORTC_BASE_ADDRESS 					(u32)0x40011000
#define GPIO_PORTD_BASE_ADDRESS 					(u32)0x40014000
#define GPIO_PORTE_BASE_ADDRESS 					(u32)0x40018000
#define GPIO_PORTF_BASE_ADDRESS 					(u32)0x40011C00
#define GPIO_PORTG_BASE_ADDRESS 					(u32)0x40012000
#define GPIO_PORTH_BASE_ADDRESS 					(u32)0x40012400

/*Speed Masks*/
#define GPIO_SPEED_INPUT_MASK							(u32)0b1100
#define GPIO_SPEED_OUTPUT_10_MASK					(u32)0b0001
#define GPIO_SPEED_OUTPUT_2_MASK						(u32)0b0010
#define GPIO_SPEED_OUTPUT_50_MASK					(u32)0b0011
	
/*Modes*/
/*INPUT*/
#define GPIO_MODE_INPUT_ANALOG_MASK							(u32)0b0011
#define GPIO_MODE_INPUT_FLOATING_MASK						(u32)0b0100
#define GPIO_MODE_INPUT_PULL_MASK							(u32)0b1000

/*OUTPUT*/
#define GPIO_MODE_OUTPUT_PUSHPULL_MASK					(u32)0b0011
#define GPIO_MODE_OUTPUT_OPENDRAIN_MASK					(u32)0b0100
#define GPIO_MODE_OUTPUT_AF_PUSHPULL_MASK				(u32)0b1000
#define GPIO_MODE_OUTPUT_AF_OPENDRAIN_MASK				(u32)0b1100

/*Create New Struct Type which represents GPIO pin and its parameters*/
typedef struct{
u32 port;
u8 pin;
u8 speed;
u8 mode;
}GPIO_t;
/***********************************************************************/

/*APIs*/
/*Description: This API will be used to configure specific pin on a specific port
Input: Pointer to desired GPIO struct
Return: None*/
extern void GPIO_voidConfigurePin(GPIO_t* DesiredGPIO);

/*Description: This API will be used to get value of a specific pin
Input: Pointer to desired GPIO struct
Return: Pin Status (The value inside IDR) as (u16)
*/
extern u16 GPIO_u16GetPinStatus (GPIO_t* DesiredGPIO);

/*Description: This API will be used to write desired value on a pin through the ODR register
Parameters: Desired pin (pointer to struct), Desired value to be written (u8)
Return: None*/
extern void GPIO_voidWriteOutput (GPIO_t* DesiredGPIO, u8 Copy_u8DesiredValue);
#endif
