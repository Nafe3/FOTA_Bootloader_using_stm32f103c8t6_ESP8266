/*
 * HUART_program.c
 *
 *  Created on: Mar 27, 2020
 *      Author: Mahmoud
 *      Version: 1.0
 */

#include "HUART_interface.h"
#include "UART_interface.h"
#include "GPIO_interface.h"
#include "RCC_interface.h"
#include "NVIC_interface.h"

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

/*APIs*/
/*Description: This API will be used to initialize the respective
 * UART peripheral
 * Parameters: Peripheral Number (u32), Baudrate (u32), stop bits (u32), parity Bits (u32)
 * Return:Error Status (u8) */
u8 HUART_u8Init(u32 Copy_u32PeripheralNumber, u32 Copy_u32Baudrate, u32 Copy_u32StopBits, u32 Copy_u32ParityBits)
{
	/*This local variable holds the status and will be returned at the end*/
	u8 Local_u8Status = STATUS_NOK;
	/*Configure GPIO pins as alternate function*/
	/* TX1 pin: PA9
	 * RX1: PA10
	 * TX2: PA2
	 * RX2: PA3
	 * TX3: PB10
	 * RX3: PB11
	 * For Asynchronous Mode:
	 * TX should be configured as AF Push Pull
	 * RX should be configured as PULL or floating*/
	if (Copy_u32PeripheralNumber == UART_USART1)
	{


		/*Enable Interrupt of UART peripheral from NVIC Driver*/
		NVIC_u8EnableInterrupt(37);

		/*Enable Clock on UART Peripheral and on Port from RCC Driver*/
		RCC_voidEnablePeripheralClock(RCC_PERIPHERALS_PORTA);
		RCC_voidEnablePeripheralClock(RCC_PERIPHERALS_USART1);



		/*Create object for TX and RX pins and initialize them from GPIO Driver*/
		GPIO_t TX = {GPIO_PORT_A, GPIO_PIN9, GPIO_MODE_OUTPUT_AF_PUSHPULL, GPIO_SPEED_OUTPUT_2};
		GPIO_t RX = {GPIO_PORT_A, GPIO_PIN10,GPIO_MODE_INPUT_PULL , GPIO_SPEED_OUTPUT_2};

		GPIO_voidConfigurePin(&RX);
		GPIO_voidConfigurePin(&TX);
	}
	else if (Copy_u32PeripheralNumber == UART_USART2)
	{
		/*Enable Clock on UART Peripheral and on Port*/
		RCC_voidEnablePeripheralClock(RCC_PERIPHERALS_PORTA);
		RCC_voidEnablePeripheralClock(RCC_PERIPHERALS_USART2);
		/*Create object for TX and RX pins and initialize them*/
		GPIO_t TX = {GPIO_PORT_A, GPIO_PIN2, GPIO_MODE_INPUT_PULL, GPIO_SPEED_OUTPUT_10};
		GPIO_t RX = {GPIO_PORT_A, GPIO_PIN3, GPIO_MODE_OUTPUT_AF_PUSHPULL, GPIO_SPEED_OUTPUT_10};

		GPIO_voidConfigurePin(&RX);
		GPIO_voidConfigurePin(&TX);
	}
	else if (Copy_u32PeripheralNumber == UART_USART3)
	{
		/*Enable Clock on UART Peripheral and on Port*/
		RCC_voidEnablePeripheralClock(RCC_PERIPHERALS_PORTB);
		RCC_voidEnablePeripheralClock(RCC_PERIPHERALS_USART3);
		/*Create object for TX and RX pins and initialize them*/
		GPIO_t TX = {GPIO_PORT_B, GPIO_PIN10, GPIO_MODE_INPUT_PULL, GPIO_SPEED_OUTPUT_10};
		GPIO_t RX = {GPIO_PORT_B, GPIO_PIN11, GPIO_MODE_OUTPUT_AF_PUSHPULL, GPIO_SPEED_OUTPUT_10};

		GPIO_voidConfigurePin(&RX);
		GPIO_voidConfigurePin(&TX);
	}

	/*Check user passed configuration parameters, if any of them is not equal to the one provided, use the default configurations which are
	 * 1. Stop Bits = 1
	 * 2. No of bits = 8 Bits
	 * 3. Parity = Disabled
	 * BaudRate=9600*/
	if (Copy_u32Baudrate != UART_BAUDRATE_9600 && Copy_u32Baudrate != UART_BAUDRATE_4800 && Copy_u32Baudrate != UART_BAUDRATE_115200)
	{
		Copy_u32Baudrate = UART_BAUDRATE_9600;
	}
	if (Copy_u32StopBits != UART_STOP_BIT1 && Copy_u32StopBits != UART_STOP_BIT2)
	{
		Copy_u32StopBits = UART_STOP_BIT1;
	}
	if (Copy_u32ParityBits != UART_PARITY_DISABLED && Copy_u32ParityBits != UART_PARITY_EVEN && Copy_u32ParityBits!= UART_PARITY_ODD)
	{
		Copy_u32ParityBits = UART_PARITY_DISABLED;
	}

	/*Call the UART_configure function from interface and save its return inside the status variable*/
	Local_u8Status = UART_u8Configure(Copy_u32PeripheralNumber, Copy_u32Baudrate, Copy_u32StopBits, Copy_u32ParityBits);

	return Local_u8Status;
}

/*Description: This API will be used to set callback function for TX
 * Parameters:Pointer to TX CallbackFunction
 * Return:Error Status  */
u8 HUART_u8SetTXCallBack(TXCallback_t Copy_TXCallbackFunction)
{
	/*This local variable will hold the function status and will be returned at the end of the function*/
	u8 Local_u8ErrorStatus=STATUS_NOK;
	/*If callback Function passed is not pointing to NULL, assign it to the static variable*/
	if (Copy_TXCallbackFunction)
	{
		Local_u8ErrorStatus = UART_u8SetTXCallBack(Copy_TXCallbackFunction);

	}
	return Local_u8ErrorStatus;
}/*End of SetTXCallback*/

/*Description: This API will be used to set callback function for RX
 * Parameters: Pointer to RX CallbackFunction
 * Return:Error Status */
u8 HUART_u8SetRXCallBack(RXCallback_t Copy_RXCallbackFunction)
{
	/*This local variable will hold the function status and will be returned at the end of the function*/
	u8 Local_u8ErrorStatus=STATUS_NOK;
	/*If callback Function passed is not pointing to NULL, assign it to the static variable*/
	if (Copy_RXCallbackFunction)
	{
		Local_u8ErrorStatus = UART_u8SetRXCallBack(Copy_RXCallbackFunction);
	}
	return Local_u8ErrorStatus;
}/*End of SetRXCallBack*/

/*Description: This API will be used to enable or disable desired interrupt by checking the input of
 * the user and either passing it or negating it
 * Parameters: Desired Interrupt (u32), desired status (u8)
 * Return:Error Status */
u8 HUART_u8EnableInterrupt(u32 Copy_u32DesiredInterrupt, u8 Copy_u8DesiredStatus)
{
	/*This local variable holds status that will be returned at the end*/
	u8 Local_u8Status = STATUS_NOK;
	/*Check that user has entered a proper interrupt choice*/
	if (Copy_u32DesiredInterrupt == UART_INTERRUPT_PARITY_ERROR || Copy_u32DesiredInterrupt == UART_INTERRUPT_IDLE || Copy_u32DesiredInterrupt == UART_INTERRUPT_RX_NOT_EMPTY || Copy_u32DesiredInterrupt == UART_INTERRUPT_TX_COMPLETE || Copy_u32DesiredInterrupt==UART_INTERRUPT_TX_EMPTY)
	{
		/*Check that user entered a proper status (whether enable or disable)*/
		if (Copy_u8DesiredStatus == UART_INTERRUPT_ENABLE || Copy_u8DesiredStatus == UART_INTERRUPT_DISABLE)
		{
			/*Now that all user choices were right, call enable interrupt function from UART driver and return its status to the status variable*/
			Local_u8Status =UART_u8EnableInterrupt(Copy_u32DesiredInterrupt, Copy_u8DesiredStatus);
		}
	}
	/*If the entered wrong choice, exit returning that status is not ok*/
	return Local_u8Status;

}

/*Description: This API will be used to pass data buffer for sending.
 * It will check that the data buffer and size passed by user are proper and if everything is right, it will call the UART_send function
 * Parameters: Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status (u8)  */
u8 HUART_u8Send(u8 *Copy_u8Buffer, u8 Copy_u8Size)
{
	/*This local variable will hold the status that will be returned at the end*/
	u8 Local_u8Status = STATUS_NOK;
	/*Check that data buffer exists and that the size is not zero*/
	if (Copy_u8Buffer && Copy_u8Size!=0)
	{
		/*Call Send Function*/
		UART_voidSend(Copy_u8Buffer, Copy_u8Size);
		Local_u8Status = STATUS_OK;
	}
	return Local_u8Status;
}

/*Description: This API will be used to pass data buffer for receiving
 * It will check that the data buffer and size passed by user are proper and if everything is right, it will call the UART_receive function
 * Parameters: Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status (u8)  */
 u8 HUART_u8Receive(u8 *Copy_u8Buffer, u8 Copy_u8Size)
{
	 /*This local variable will hold the status that will be returned at the end*/
	 	u8 Local_u8Status = STATUS_NOK;
	 	/*Check that data buffer exists and that the size is not zero*/
	 	if (Copy_u8Buffer && Copy_u8Size!=0)
	 	{
	 		/*Call Send Function*/
	 		Local_u8Status = UART_u8Receive(Copy_u8Buffer, Copy_u8Size);
	 	}
	 	return Local_u8Status;
}

