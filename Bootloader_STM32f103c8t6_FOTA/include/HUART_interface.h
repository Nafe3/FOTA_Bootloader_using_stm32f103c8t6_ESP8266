/*
 * HUART_interface.h
 *
 *  Created on: June 4, 2020
 *      Author: Mahmoud Hamdy
 *      Version: 2.0
 */

/*Changelog from version 1.1:
 * 1) Removed Deparcated Macros (related to baudrate)
 * 2) Changed the flags for the IRQ because there were bugs in it
 * 3) Changed the way asynchronous Functions work by making it handle enabling interrupts instead of being handled by the user
 * 4) Changed IRQ implementations
 * 5) Added options for callback functions for each peripheral
 * */

/*Changelog from version 1.0:
 * 1) Added functionality to use multiple UART peripherals (and implemented their interrupt handlers)
 * 2) Added new functions to send and receive data sync
 * 3) Fixed a bug where an exception was thrown if there are no callback functions determined
 * */
 
#ifndef HUART_INTERFACE_H_
#define HUART_INTERFACE_H_

#include "GPIO.h"

/*Macros*/

/*Stop Bits*/
#define UART_STOP_BIT1					(u32)~(0x3000)
#define UART_STOP_BIT2					(u32)(0x2000)

/*Parity Controls*/
#define UART_PARITY_EVEN				(u32)(0x400)
#define UART_PARITY_ODD					(u32)(0x600)
#define UART_PARITY_DISABLED			(u32)~(0x400)

/*Interrupt Enable Controls*/
#define UART_INTERRUPT_PARITY_ERROR		(u32)(0x100)
#define UART_INTERRUPT_IDLE				(u32)(0x10)
#define UART_INTERRUPT_ENABLE			(u8)1
#define UART_INTERRUPT_DISABLE			(u8)0

/*Callback functions pointers*/
typedef void(*TXCallback_t)(void);
typedef void(*RXCallback_t)(void);



/*Structs*/
typedef struct
{
	GPIO_Pin_t TX;
	GPIO_Pin_t RX;
	u32 Port;
	u32 PeripheralClockName;
	u8 InterruptPeripheralName;
	u32 BaseAddress;
}UART_GPIO_t;

/*USART Objects*/
/*These are the Objects that the user should choose from when enabling the UART peripheral*/
extern const UART_GPIO_t HUART_USART1;
extern const UART_GPIO_t HUART_USART2;
extern const UART_GPIO_t HUART_USART3;

/*APIs*/
/*Description: This API will be used to initialize the respective
 * UART peripheral
 * Parameters: Peripheral Number (u32), Baudrate (u32), stop bits (u32), parity Bits (u32)
 * Return:Error Status (u8) */
extern u8 HUART_u8Init(UART_GPIO_t Copy_u32PeripheralNumber, u32 Copy_u32Baudrate, u32 Copy_u32StopBits, u32 Copy_u32ParityBits);
/*Description: This API will be used to enable or disable desired interrupt by checking the input of
 * the user and either passing it or negating it
 * Parameters: Desired UART peripheral(struct), Desired Interrupt (u32), desired status (u8)
 * Return:Error Status */
extern u8 HUART_u8EnableInterrupt(UART_GPIO_t Copy_u32PeripheralNumber, u32 Copy_u32DesiredInterrupt, u8 Copy_u8DesiredStatus);

/*Description: This API will be used to set callback function for TX to specific peripheral
 * Parameters:Pointer to TX CallbackFunction, desired UART (u32)
 * Return:Error Status  */
extern u8 HUART_u8SetTXCallBack(TXCallback_t Copy_TXCallbackFunction, u32 Copy_u32DesiredUART);
/*Description: This API will be used to set callback function for RX for specific peripheral
 * Parameters: Pointer to RX CallbackFunction, Desired UART peripheral (u32)
 * Return:Error Status */
extern u8 HUART_u8SetRXCallBack(RXCallback_t Copy_RXCallbackFunction, u32 Copy_u32DesiredUART);

/*Description: This API will be used to pass data buffer for sending using interrupts.
 * Parameters: Desired UART (struct), Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status (u8)  */
extern u8 HUART_u8SendAsync(UART_GPIO_t Copy_u32PeripheralNumber, u8 *Copy_u8Buffer, u8 Copy_u8Size);
/*Description: This API will be used to pass data buffer for receiving using interrupts.
 * Parameters: Desired UART (struct), Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status (u8)  */
extern u8 HUART_u8ReceiveAsync(UART_GPIO_t Copy_u32PeripheralNumber, u8 *Copy_u8Buffer, u8 Copy_u8Size);

/*Description: This API will be used to pass data buffer for sending using polling.
 * Parameters: Desired UART (struct), Pointer to Data Buffer (u8*), size of data buffer (u8), desired time between frames (u32)
 * Return: Error Status (u8)  */
extern u8 HUART_u8SendSync(UART_GPIO_t Copy_u32PeripheralNumber, u8 *Copy_u8Buffer, u8 Copy_u8Size, u32 Copy_u32Time);
/*Description: This API will be used to pass data buffer for receiving using polling.
 * Parameters: Desired UART (struct), Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status (u8)  */
extern u8 HUART_u8ReceiveSync(UART_GPIO_t Copy_u32PeripheralNumber, u8 *Copy_u8Buffer, u8 Copy_u8Size, u32 Copy_u32Time);

/*Description: This function can be used to terminate async receiving (Warning!: Don't use it unless you know what you are doing)
 * Parameters: Desired UART Peripheral address (u32)
 * return: None*/
extern void HUART_voidTerminateReceiving (u32 Copy_u32DesiredUARTBaseAddress);

/*Description: This function can be used to terminate async sending (Warning!: Don't use it unless you know what you are doing)
 * Parameters: Desired UART Peripheral address (u32)
 * return: None*/
extern void HUART_voidTerminateSending (u32 Copy_u32DesiredUARTBaseAddress);


#endif /* HUART_INTERFACE_H_ */
