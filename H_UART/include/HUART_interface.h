/*
 * HUART_interface.h
 *
 *  Created on: Mar 27, 2020
 *      Author: Mahmoud Hamdy
 *      Version: 1.0
 */
#ifndef HUART_INTERFACE_H_
#define HUART_INTERFACE_H_

#include "UART_interface.h"
/*Macros*/
/*USART Peripherals*/
#define UART_USART1						UART_USART1_BASE_ADDRESS
#define UART_USART2						UART_USART2_BASE_ADDRESS
#define UART_USART3						UART_USART3_BASE_ADDRESS
#define UART_UART4						UART_UART4_BASE_ADDRESS
#define UART_UART5						UART_UART5_BASE_ADDRESS
/*Baud Rate*/
/*- IntegerDivider = ((Clock on Bus) / (16 * Baud rate))
  - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5

  PCLK for USART1=72MHz
  For 9600 Baudrate:
  Integer Divider = 468.75 --> 468=0x1D4
  Fraction = 0.75*16=12 --> C

  For 115200 Baudrate:
  Integer Divider = 39.0625 -->39=0x27
  Fraction=.0625*16=1 --> 1

  For 4800 Baudrate:
  Integer Divider = 937.5 --> 3A9
  Fraction= 0.5 *16=8 -->8
*/

#define UART_BAUDRATE_4800				(u16)(0x3A98)
#define UART_BAUDRATE_9600				(u16)(0x1D4C)
#define UART_BAUDRATE_115200			(u16)(0x0271)

/*Stop Bits*/
#define UART_STOP_BIT1					UART_STOP_BIT1_MASK
#define UART_STOP_BIT2					UART_STOP_BIT2_MASK

/*Parity Controls*/
#define UART_PARITY_EVEN				UART_PARITY_EVEN_MASK
#define UART_PARITY_ODD					UART_PARITY_ODD_MASK
#define UART_PARITY_DISABLED			UART_PARITY_DISABLED_MASK

/*Interrupt Enable Controls*/
#define UART_INTERRUPT_PARITY_ERROR		UART_PARITY_ERROR_INTERRUPT_ENABLE_MASK
#define UART_INTERRUPT_TX_EMPTY			UART_TX_EMPTY_INTERRUPT_ENABLE_MASK
#define UART_INTERRUPT_TX_COMPLETE		UART_TX_COMPLETE_INTERRUPT_ENABLE_MASK
#define UART_INTERRUPT_RX_NOT_EMPTY		UART_RX_NOT_EMPTY_INTERRUPT_ENABLE_MASK
#define UART_INTERRUPT_IDLE				UART_IDLE_INTERRUPT_ENABLE_MASK
#define UART_INTERRUPT_ENABLE			UART_INTERRUPT_ENABLE_MASK
#define UART_INTERRUPT_DISABLE			UART_INTERRUPT_DISABLE_MASK



/*APIs*/
/*Description: This API will be used to initialize the respective
 * UART peripheral
 * Parameters: Peripheral Number (u32), Baudrate (u32), stop bits (u32), parity Bits (u32)
 * Return:Error Status (u8) */
extern u8 HUART_u8Init(u32 Copy_u32PeripheralNumber, u32 Copy_u32Baudrate, u32 Copy_u32StopBits, u32 Copy_u32ParityBits);
/*Description: This API will be used to set callback function for TX
 * Parameters:Pointer to TX CallbackFunction
 * Return:Error Status  */
extern u8 HUART_u8SetTXCallBack(TXCallback_t Copy_TXCallbackFunction);
/*Description: This API will be used to set callback function for RX
 * Parameters: Pointer to RX CallbackFunction
 * Return:Error Status */
extern u8 HUART_u8SetRXCallBack(RXCallback_t Copy_RXCallbackFunction);
/*Description: This API will be used to enable or disable desired interrupt by checking the input of
 * the user and either passing it or negating it
 * Parameters: Desired Interrupt (u32), desired status (u8)
 * Return:Error Status */
extern u8 HUART_u8EnableInterrupt(u32 Copy_u32DesiredInterrupt, u8 Copy_u8DesiredStatus);
/*Description: This API will be used to pass data buffer for sending.
 * Parameters: Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status (u8)  */
extern u8 HUART_u8Send(u8 *Copy_u8Buffer, u8 Copy_u8Size);
/*Description: This API will be used to pass data buffer for receiving.
 * Parameters: Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status (u8)  */
extern u8 HUART_u8Receive(u8 *Copy_u8Buffer, u8 Copy_u8Size);

#endif /* HUART_INTERFACE_H_ */
