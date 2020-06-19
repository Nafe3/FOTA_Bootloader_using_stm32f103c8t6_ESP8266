/*
 * UART_interface.h
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

#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_

/*Callback functions pointers*/
typedef void(*TXCallback_t)(void);
typedef void(*RXCallback_t)(void);
/*********************************************************************************/
/************Warning: Don't change anything in this section***********************/
/*Base Addresses*/
#define UART_USART1_BASE_ADDRESS			(u32 volatile)(0x40013800)
#define UART_USART2_BASE_ADDRESS			(u32 volatile)(0x40004400)
#define UART_USART3_BASE_ADDRESS			(u32 volatile)(0x40004800)
#define UART_UART4_BASE_ADDRESS				(u32 volatile)(0x40004C00)
#define UART_UART5_BASE_ADDRESS				(u32 volatile)(0x40005000)
/*Offsets*/
#define UART_SR								(u32 volatile)(0x00)
#define UART_DR								(u32 volatile)(0x04)
#define UART_BRR							(u32 volatile)(0x08)
#define UART_CR1							(u32 volatile)(0x0C)
#define UART_CR2							(u32 volatile)(0x10)
#define UART_CR3							(u32 volatile)(0x14)
#define UART_GTPR							(u32 volatile)(0x18)
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


/********************************************************************************/

/*APIs*/
/*Description: This API will be configure the UART with the passed configurations
 * Parameters: Base Address (u32), Baudrate (u32), Stop Bits (u32), Parity Bits (u32)
 * Return:Error Status*/
extern u8 UART_u8Configure (u32 Copy_u32BaseAddress, u16 Copy_u16Baudrate, u32 Copy_u32StopBits, u32 Copy_u32ParityBits);

/*Description: This function will be used to trigger sending data. It will send only the first bit of the buffer and the rest will be handled by the interrupt request
 * Parameters: Desired UART Peripheral (u32), Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status  */
extern u8 UART_voidSendAsync(u32 Copy_u32UARTAddress, u8 *Copy_u8Buffer, u8 Copy_u8Size);
/*Description: This API will be used to receive data using interrupts
 * Parameters:Desired UART Peripheral (u32), Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status */
extern u8 UART_u8ReceiveAsync(u32 Copy_u32UARTAddress, u8 *Copy_u8Buffer, u8 Copy_u8Size);

/*Description: This function will save the passed callback by the user to the static variable
 * Parameters: Pointer to TX CallbackFunction, Desired UART Peripheral (u32)
 * Return:Error Status */
extern u8 UART_u8SetTXCallBack(TXCallback_t Copy_TXCallbackFunction, u32 Copy_u32DesiredUART);
/* Description: This function will save the passed callback by the user to the static variable
 * Parameters: Pointer to RX CallbackFunction, Desired UART Peripheral (u32)
 * Return:Error Status */
extern u8 UART_u8SetRXCallBack(RXCallback_t Copy_RXCallbackFunction, u32 Copy_u32DesiredUART);

/*Description: This function will be used to trigger sending data synchronously without using interrupts
 * Parameters: Desired UART Peripheral (u32), Pointer to Data Buffer (u8*), size of data buffer (u8), required delay in ms (u32)
 * Return: void  */
extern void UART_voidSendSync(u32 Copy_u32UARTAddress, u8 *Copy_u8Buffer, u8 Copy_u8Size, u32 Copy_u32Time);
/*Description: This function will be used to trigger receiving data synchronously without using interrupts
 * Parameters: Desired UART Peripheral (u32), Pointer to Data Buffer (u8*), size of data buffer (u8), required delay in ms (u32)
 * Return: Error Status */
extern u8 UART_u8ReceiveSync(u32 Copy_u32UARTAddress, u8 *Copy_u8Buffer, u8 Copy_u8Size, u32 Copy_u32Time);

/*Description: This API will be used to enable or disable desired interrupt
 * Parameters: Desired UART Peripheral(u32), Desired Interrupt (u32), Desired Status (u8)
 * Return:Error Status */
extern u8 UART_u8EnableInterrupt(u32 Copy_u32UARTAddress, u32 Copy_u32DesiredInterrupt, u8 Copy_u8DesiredStatus);

/*Description: This API can be used to terminate async receiving (Warning!: Don't use it unless you know what you are doing)
 * Parameters: Desired UART Peripheral address (u32)
 * return: None*/
extern void UART_voidTerminateReceiving (u32 Copy_u32DesiredUARTBaseAddress);

/*Description: This function can be used to terminate async receiving (Warning!: Don't use it unless you know what you are doing)
 * Parameters: Desired UART Peripheral address (u32)
 * return: None*/
extern void UART_voidTerminateSending (u32 Copy_u32DesiredUARTBaseAddress);

#endif /* UART_INTERFACE_H_ */
