/*
 * UART_interface.h
 *
 *  Created on: Mar 27, 2020
 *      Author: Mahmoud Hamdy
 *      Version: 1.0
 */

#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

#include "STD_TYPES.h"
/*********************************************************************************/
/************Warning: Don't change anything in this section***********************/
/*Base Addresses*/
#define UART_USART1_BASE_ADDRESS			(u32)(0x40013800)
#define UART_USART2_BASE_ADDRESS			(u32)(0x40004400)
#define UART_USART3_BASE_ADDRESS			(u32)(0x40004800)
#define UART_UART4_BASE_ADDRESS				(u32)(0x40004C00)
#define UART_UART5_BASE_ADDRESS				(u32)(0x40005000)
/*Offsets*/
#define UART_SR								(u32)(0x00)
#define UART_DR								(u32)(0x04)
#define UART_BRR							(u32)(0x08)
#define UART_CR1							(u32)(0x0C)
#define UART_CR2							(u32)(0x10)
#define UART_CR3							(u32)(0x14)
#define UART_GTPR							(u32)(0x18)
/*Read Only MASKS (should be ANDed to get state)*/
/*SR Options*/
#define UART_CTS_FLAG_MASK							(u32)(0b1000000000)
#define UART_LIN_BREAK_MASK							(u32)(0b100000000)
#define UART_TX_EMPTY_MASK							(u32)(0b10000000)
#define UART_TX_COMPLETE_MASK						(u32)(0b1000000)
#define UART_RX_NOT_EMPTY_MASK						(u32)(0b100000)
#define UART_IDLE_DETECTED_MASK						(u32)(0b10000)
#define UART_OVERRUN_ERROR_MASK						(u32)(0b1000)
#define UART_NOISE_ERROR_MASK						(u32)(0b100)
#define UART_FRAMING_ERROR_MASK						(u32)(0b10)
#define UART_PARITY_ERROR_MASK						(u32)(0b1)
/*CR1 Options*/
#define UART_ENABLE_MASK							(u32)(0x2000)
#define UART_9BITS_MASK								(u32)(0x1000)
#define UART_WAKEUP_ADDRESS_MASK					(u32)(0x800)
#define UART_PARITY_EVEN_MASK						(u32)(0x400)
#define UART_PARITY_ODD_MASK						(u32)(0x600)
#define UART_PARITY_DISABLED_MASK					(u32)~(0x400)
#define UART_PARITY_ERROR_INTERRUPT_ENABLE_MASK		(u32)(0x100)
#define UART_TX_EMPTY_INTERRUPT_ENABLE_MASK			(u32)(0x80)
#define UART_TX_COMPLETE_INTERRUPT_ENABLE_MASK		(u32)(0x40)
#define UART_RX_NOT_EMPTY_INTERRUPT_ENABLE_MASK		(u32)(0x20)
#define UART_IDLE_INTERRUPT_ENABLE_MASK				(u32)(0x10)
#define UART_TX_ENABLE_MASK							(u32)(0x8)
#define UART_RX_ENABLE_MASK							(u32)(0x4)
#define UART_RX_MUTE_MASK							(u32)(0x2)
#define UART_BREAK_CHAR_ENABLE_MASK					(u32)(0x1)
/*CR2 Options*/
#define UART_STOP_BIT1_MASK							(u32)~(0x3000)
#define UART_STOP_BIT2_MASK							(u32)(0x2000)
/*CR3 Options*/
#define UART_DMA_TX_ENABLE_MASK						(u32)(0x80)
#define UART_DMA_RX_ENABLE_MASK						(u32)(0x40)
#define UART_ERROR_INT_ENABLE_MASK					(u32)(0x1)

#define UART_INTERRUPT_ENABLE_MASK					(u8)1
#define UART_INTERRUPT_DISABLE_MASK					(u8)0
#define UART_PARITY_CANCELLATION_MASK				(u8)~(0b10000000)

/*Custom Types*/
typedef void(*TXCallback_t)(void);
typedef void(*RXCallback_t)(void);

/********************************************************************************/

/*APIs*/
/*Description: This API will be configure the UART with the passed configurations
 * Parameters: Base Address (u32), Baudrate (u32), Stop Bits (u32), Parity Bits (u32)
 * Return:Error Status*/
extern u8 UART_u8Configure (u32 Copy_u32BaseAddress, u32 Copy_u32Baudrate, u32 Copy_u32StopBits, u32 Copy_u32ParityBits);
/*Description: This function will be used to trigger sending data. It will send only the first byte of the buffer and the rest will be handled by the interrupt request
 * Parameters: Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: void  */
extern void UART_voidSend(u8 *Copy_u8Buffer, u8 Copy_u8Size);
/*Description: This API will be used to receive data
 * Parameters:
 * Return: Error Status */
extern u8 UART_u8Receive(u8 *Copy_u8Buffer, u8 Copy_u8Size);
/*Description: This API will be used to enable or disable desired interrupt
 * Parameters: Desired Interrupt (u32), Desired Status (u8)
 * Return:Error Status */
extern u8 UART_u8EnableInterrupt(u32 Copy_u32DesiredInterrupt, u8 Copy_u8DesiredStatus);

/*Description: This function will save the passed callback by the user to the static variable
 * Parameters: Pointer to TX CallbackFunction
 * Return:Error Status */
extern u8 UART_u8SetTXCallBack(TXCallback_t Copy_TXCallbackFunction);

/*Description: This function will save the passed callback by the user to the static variable
 * Parameters: Pointer to RX CallbackFunction
 * Return:Error Status */
extern u8 UART_u8SetRXCallBack(RXCallback_t Copy_RXCallbackFunction);

#endif /* UART_INTERFACE_H_ */
