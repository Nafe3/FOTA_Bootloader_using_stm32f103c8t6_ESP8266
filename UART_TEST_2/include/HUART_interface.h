/*
 * HUART_interface.h
 *
 *  Created on: Mar 27, 2020
 *      Author: Mahmoud Hamdy
 *      Version: 1.0
 */
#ifndef HUART_INTERFACE_H_
#define HUART_INTERFACE_H_

/*Macros*/
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

  PCLK for USART1=8MHz
   For 9600 Baudrate:
  Integer Divider = 52.08 --> 52=34
  Fraction = 0.08*16=1.28 --> C=01
*/

#define UART_BAUDRATE_4800				(u16)(0x3A98)
#define UART_BAUDRATE_9600				(u16)(0x0341)
#define UART_BAUDRATE_115200			(u16)(0x0271)

/*Stop Bits*/
#define UART_STOP_BIT1					(u32)~(0x3000)
#define UART_STOP_BIT2					(u32)(0x2000)

/*Parity Controls*/
#define UART_PARITY_EVEN				(u32)(0x400)
#define UART_PARITY_ODD					(u32)(0x600)
#define UART_PARITY_DISABLED			(u32)~(0x400)

/*Interrupt Enable Controls*/
#define UART_INTERRUPT_PARITY_ERROR		(u32)(0x100)
#define UART_INTERRUPT_TX_EMPTY			(u32)(0x80)
#define UART_INTERRUPT_TX_COMPLETE		(u32)(0x40)
#define UART_INTERRUPT_RX_NOT_EMPTY		(u32)(0x20)
#define UART_INTERRUPT_IDLE				(u32)(0x10)
#define UART_INTERRUPT_ENABLE			(u8)1
#define UART_INTERRUPT_DISABLE			(u8)0


/*Structs*/
typedef struct
{
	GPIO_t TX;
	GPIO_t RX;
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

/*Description: This API will be used to set callback function for TX
 * Parameters:Pointer to TX CallbackFunction
 * Return:Error Status  */
extern u8 HUART_u8SetTXCallBack(TXCallback_t Copy_TXCallbackFunction);
/*Description: This API will be used to set callback function for RX
 * Parameters: Pointer to RX CallbackFunction
 * Return:Error Status */
extern u8 HUART_u8SetRXCallBack(RXCallback_t Copy_RXCallbackFunction);

/*Description: This API will be used to pass data buffer for sending using interrupts.
 * Parameters: Desired UART (struct), Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status (u8)  */
extern u8 HUART_u8SendAsync(UART_GPIO_t Copy_u32PeripheralNumber, u8 *Copy_u8Buffer, u8 Copy_u8Size);
/*Description: This API will be used to pass data buffer for receiving using interrupts.
 * Parameters: Desired UART (struct), Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status (u8)  */
extern u8 HUART_u8ReceiveAsync(UART_GPIO_t Copy_u32PeripheralNumber, u8 *Copy_u8Buffer, u8 Copy_u8Size);

/*Description: This API will be used to pass data buffer for sending using polling.
 * Parameters: Desired UART (struct), Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status (u8)  */
extern u8 HUART_u8SendSync(UART_GPIO_t Copy_u32PeripheralNumber, u8 *Copy_u8Buffer, u8 Copy_u8Size);
/*Description: This API will be used to pass data buffer for receiving using polling.
 * Parameters: Desired UART (struct), Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status (u8)  */
extern u8 HUART_u8ReceiveSync(UART_GPIO_t Copy_u32PeripheralNumber, u8 *Copy_u8Buffer, u8 Copy_u8Size);

#endif /* HUART_INTERFACE_H_ */
