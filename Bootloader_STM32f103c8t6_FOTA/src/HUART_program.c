/*
 * HUART_program.c
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

#include "STD_TYPES.h"
#include "GPIO.h"
#include "RCC_interface.h"
#include "NVIC_interface.h"

#include "UART_interface.h"
#include "HUART_interface.h"

 const UART_GPIO_t HUART_USART1 = {

		.TX = {GPIOA, GPIO_PIN_9,GPIO_OUTPUT_SPEED_50MHz ,GPIO_MODE_OUTPUT_ALTERNATE_FUNCTION_PUSH_PULL },
		.RX = {GPIOA, GPIO_PIN_10,GPIO_INPUT_MODE_RESET_STATE ,GPIO_MODE_INPUT_PULLUP_PULLDOWN },
		.Port = RCC_PERIPHERALS_PORTA,
		.PeripheralClockName = RCC_PERIPHERALS_USART1,
		.InterruptPeripheralName = NVIC_USART1,
		.BaseAddress=UART_USART1_BASE_ADDRESS
};
 const UART_GPIO_t HUART_USART2 = {
 		.TX = {GPIOA, GPIO_PIN_2, GPIO_OUTPUT_SPEED_50MHz ,GPIO_MODE_OUTPUT_ALTERNATE_FUNCTION_PUSH_PULL},
 		.RX = {GPIOA, GPIO_PIN_3,GPIO_INPUT_MODE_RESET_STATE ,GPIO_MODE_INPUT_PULLUP_PULLDOWN},
 		.Port = RCC_PERIPHERALS_PORTA,
 		.PeripheralClockName = RCC_PERIPHERALS_USART2,
 		.InterruptPeripheralName = NVIC_USART2,
 		.BaseAddress=UART_USART2_BASE_ADDRESS
 };
 const UART_GPIO_t HUART_USART3 = {
 		.TX = {GPIOB, GPIO_PIN_10, GPIO_OUTPUT_SPEED_50MHz ,GPIO_MODE_OUTPUT_ALTERNATE_FUNCTION_PUSH_PULL},
 		.RX = {GPIOB, GPIO_PIN_11,GPIO_INPUT_MODE_RESET_STATE ,GPIO_MODE_INPUT_PULLUP_PULLDOWN},
 		.Port = RCC_PERIPHERALS_PORTB,
 		.PeripheralClockName = RCC_PERIPHERALS_USART3,
 		.InterruptPeripheralName = NVIC_USART3,
 		.BaseAddress=UART_USART3_BASE_ADDRESS
 };


 /*Description: This static function will be used to make baudrate calculations according to chosen clocks and prescalars
  * Parameters: Desired Baudrate (u16), UART Peripheral (u32)
  * Return: Baudrate value (u16) */
 u16 HUART_u16BaudrateCalculator (u32 Copy_u32DesiredBaudrate, u32 Copy_u32UARTAddress)
 {
 	/*This local variable will be used to determine clock in use*/
 	u8 volatile Local_u8currentClock=0;
 	/*This local value will hold prescalar value of bus*/
 	u8 volatile Local_u8PrescalarValue=1;
 	/*This Local variable will hold multiplier value in case of PLL*/
 	f32 volatile Local_f32Multiplier=1;
 	/*This Local Variable will hold clock speed, in case of HSE and HSI, it will be 8MHz (or the value found in RCC_interface.h generally)*/
 	u32 volatile Local_u32ClockSpeed= RCC_HSE_VALUE;
 	/*This Local Variable will hold divisor value that will be written inside the register*/
 	u16 volatile Local_u16DivisorValue=1;
 	/*This Local Variable will hold fraction value that will be written inside the register*/
 	u8 volatile Local_u8FractionValue=1;
 	/*This local variable will hold the final value that will be written to the register*/
 	u16 volatile Local_u16RegisterValue=0;

 	/*Check for current clock and store it in local variable*/
 	Local_u8currentClock = RCC_u8GetSWSStatus();
 	/*If current clock is PLL, check its multiplier settings*/
 	if (Local_u8currentClock == RCC_SWS_PLL)
 	{
 		/*Get multiplier settings through specific API in RCC register*/
 		Local_f32Multiplier = RCC_f32GetPLLMultiplierValue();
 		/*Calculate new speed*/
 		Local_u32ClockSpeed = Local_u32ClockSpeed * Local_f32Multiplier;
 	}

 	/*Check which UART peripheral is currently in use to determine corressponding bus, and according to it check current prescalar to determine speed*/
 	/*In case of UART1, then we are on APB2*/
 	if (Copy_u32UARTAddress == UART_USART1_BASE_ADDRESS)
 	{
 		Local_u8PrescalarValue = RCC_u16GetPrescalarValue(RCC_GET_APB2_PRESCALAR);
 	}
 	/*Other UART peripherals are found on APB1*/
 	else
 	{
 		Local_u8PrescalarValue = RCC_u16GetPrescalarValue(RCC_GET_APB1_PRESCALAR);
 	}

 	/*Calculate final clock value*/
 	Local_u32ClockSpeed = Local_u32ClockSpeed / Local_u8PrescalarValue;

 	/*Now calculate baudrate according to these 2 equations
 	 * - IntegerDivider = ((Clock on Bus) / (16 * Baud rate))
   	  	  - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5
 	 * */
 	Local_u16DivisorValue = Local_u32ClockSpeed / (16 * Copy_u32DesiredBaudrate);
 	Local_u8FractionValue = ((((f32)Local_u32ClockSpeed / (16*(f32)Copy_u32DesiredBaudrate))) - (Local_u32ClockSpeed / (16 * Copy_u32DesiredBaudrate))) * 16;

 	/*Put the divisor value and fraction value inside one variable*/
 	/*Divisor value will be shifted by one to the left by 4 bits so that fraction value can be concatenated next to it*/
 	Local_u16RegisterValue = (Local_u16DivisorValue <<4) | Local_u8FractionValue;

 	/*Now write final value of baudrate to BRR register*/
 	return Local_u16RegisterValue;
 }


/*APIs*/
/*Description: This API will be used to initialize the respective
 * UART peripheral
 * Parameters: Peripheral Number (u32), Baudrate (u16), stop bits (u32), parity Bits (u32)
 * Return:Error Status (u8) */
u8 HUART_u8Init(UART_GPIO_t Copy_u32PeripheralNumber, u32 Copy_u32Baudrate, u32 Copy_u32StopBits, u32 Copy_u32ParityBits)
{
	/*This local variable holds the status and will be returned at the end*/
	u8 Local_u8Status = STATUS_NOK;
	/*This local variable will hold baudrate that will be written to the register*/
	u16 Local_u16Baudrate=0;
	/*Configure GPIO pins*/
		/* TX1 pin: PA9
		 * RX1: PA10
		 * TX2: PA2
		 * RX2: PA3
		 * TX3: PB10
		 * RX3: PB11
		 * For Asynchronous Mode:
		 * TX should be configured as AF Push Pull
		 * RX should be configured as PULL or floating*/
	/*Enable Peripheral Interrupt through NVIC*/
	NVIC_u8EnableInterrupt(Copy_u32PeripheralNumber.InterruptPeripheralName);

	/*Enable Clock on Port and UART peripheral*/
	RCC_voidEnablePeripheralClock(Copy_u32PeripheralNumber.PeripheralClockName);
	RCC_voidEnablePeripheralClock(Copy_u32PeripheralNumber.Port);

	/*Configure GPIO*/
	GPIO_Init(&Copy_u32PeripheralNumber.RX);
	GPIO_Pin_Write(&Copy_u32PeripheralNumber.RX,HIGH);
	GPIO_Init(&Copy_u32PeripheralNumber.TX);


	/*Calculate baudrate value that will be written in the register according to user input*/
	Local_u16Baudrate = HUART_u16BaudrateCalculator(Copy_u32Baudrate, Copy_u32PeripheralNumber.BaseAddress);

	/*Check user passed configuration parameters, if any of them is not equal to the one provided, use the default configurations which are
	 * 1. Stop Bits = 1
	 * 2. No of bits = 8 Bits
	 * 3. Parity = Disabled
	 * */
	if (Copy_u32StopBits != UART_STOP_BIT1 && Copy_u32StopBits != UART_STOP_BIT2)
	{
		Copy_u32StopBits = UART_STOP_BIT1;
	}
	if (Copy_u32ParityBits != UART_PARITY_DISABLED && Copy_u32ParityBits != UART_PARITY_EVEN && Copy_u32ParityBits!= UART_PARITY_ODD)
	{
		Copy_u32ParityBits = UART_PARITY_DISABLED;
	}

	/*Call the UART_configure function from interface and save its return inside the status variable*/
	Local_u8Status = UART_u8Configure(Copy_u32PeripheralNumber.BaseAddress, Local_u16Baudrate, Copy_u32StopBits, Copy_u32ParityBits);
	return Local_u8Status;
}

/*Description: This API will be used to set callback function for TX to specific peripheral
 * Parameters:Pointer to TX CallbackFunction, desired UART (u32)
 * Return:Error Status  */
u8 HUART_u8SetTXCallBack(TXCallback_t Copy_TXCallbackFunction, u32 Copy_u32DesiredUART)
{
	/*This local variable will hold the function status and will be returned at the end of the function*/
	u8 Local_u8ErrorStatus=STATUS_NOK;
	/*If callback Function passed is not pointing to NULL, assign it to the static variable*/
	if (Copy_TXCallbackFunction)
	{
		Local_u8ErrorStatus = UART_u8SetTXCallBack(Copy_TXCallbackFunction, Copy_u32DesiredUART);

	}
	return Local_u8ErrorStatus;
}/*End of SetTXCallback*/

/*Description: This API will be used to set callback function for RX for specific peripheral
 * Parameters: Pointer to RX CallbackFunction, Desired UART peripheral (u32)
 * Return:Error Status */
u8 HUART_u8SetRXCallBack(RXCallback_t Copy_RXCallbackFunction, u32 Copy_u32DesiredUART)
{
	/*This local variable will hold the function status and will be returned at the end of the function*/
	u8 Local_u8ErrorStatus=STATUS_NOK;
	/*If callback Function passed is not pointing to NULL, assign it to the static variable*/
	if (Copy_RXCallbackFunction)
	{
		Local_u8ErrorStatus = UART_u8SetRXCallBack(Copy_RXCallbackFunction, Copy_u32DesiredUART);
	}
	return Local_u8ErrorStatus;
}/*End of SetRXCallBack*/

/*Description: This API will be used to enable or disable desired interrupt by checking the input of
 * the user and either passing it or negating it
 * Parameters: Desired UART peripheral(struct), Desired Interrupt (u32), desired status (u8)
 * Return:Error Status */
u8 HUART_u8EnableInterrupt(UART_GPIO_t Copy_u32PeripheralNumber, u32 Copy_u32DesiredInterrupt, u8 Copy_u8DesiredStatus)
{
	/*This local variable holds status that will be returned at the end*/
	u8 Local_u8Status = STATUS_NOK;
	/*Check that user has entered a proper interrupt choice*/
	if (Copy_u32DesiredInterrupt == UART_INTERRUPT_PARITY_ERROR || Copy_u32DesiredInterrupt == UART_INTERRUPT_IDLE)
	{
		/*Check that user entered a proper status (whether enable or disable)*/
		if (Copy_u8DesiredStatus == UART_INTERRUPT_ENABLE || Copy_u8DesiredStatus == UART_INTERRUPT_DISABLE)
		{
			/*Now that all user choices were right, call enable interrupt function from UART driver and return its status to the status variable*/
			Local_u8Status =UART_u8EnableInterrupt(Copy_u32PeripheralNumber.BaseAddress, Copy_u32DesiredInterrupt, Copy_u8DesiredStatus);
		}
	}

	/*If the entered wrong choice, exit returning that status is not ok*/
	return Local_u8Status;

}

/*Description: This API will be used to pass data buffer for sending using interrupts.
 * It will check that the data buffer and size passed by user are proper and if everything is right, it will call the UART_send function
 * Parameters: Desired UART (struct), Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status (u8)  */
u8 HUART_u8SendAsync(UART_GPIO_t Copy_u32PeripheralNumber, u8 *Copy_u8Buffer, u8 Copy_u8Size)
{
	/*This local variable will hold the status that will be returned at the end*/
	u8 Local_u8Status = STATUS_NOK;
	/*Check that data buffer exists and that the size is not zero*/
	if (Copy_u8Buffer && Copy_u8Size!=0)
	{
		/*Call Send Function*/
		UART_voidSendAsync(Copy_u32PeripheralNumber.BaseAddress, Copy_u8Buffer, Copy_u8Size);
		Local_u8Status = STATUS_OK;
	}
	return Local_u8Status;
}

/*Description: This API will be used to pass data buffer for receiving using interrupts.
 * It will check that the data buffer and size passed by user are proper and if everything is right, it will call the UART_receive function
 * Parameters: Desired UART (struct), Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status (u8)  */
 u8 HUART_u8ReceiveAsync(UART_GPIO_t Copy_u32PeripheralNumber, u8 *Copy_u8Buffer, u8 Copy_u8Size)
{
	 /*This local variable will hold the status that will be returned at the end*/
	 	u8 Local_u8Status = STATUS_NOK;
	 	/*Check that data buffer exists and that the size is not zero*/
	 	if (Copy_u8Buffer && Copy_u8Size!=0)
	 	{
	 		/*Call Send Function*/
	 		Local_u8Status = UART_u8ReceiveAsync(Copy_u32PeripheralNumber.BaseAddress ,Copy_u8Buffer, Copy_u8Size);
	 	}
	 	return Local_u8Status;
}

 /*Description: This API will be used to pass data buffer for sending using polling.
  * Parameters: Desired UART (struct), Pointer to Data Buffer (u8*), size of data buffer (u8), desired time between frames (u32)
  * Return: Error Status (u8)  */
u8 HUART_u8SendSync(UART_GPIO_t Copy_u32PeripheralNumber, u8 *Copy_u8Buffer, u8 Copy_u8Size, u32 Copy_u32Time)
{
	/*This local variable will hold the status that will be returned at the end*/
	u8 Local_u8Status = STATUS_NOK;
	/*Check that data buffer exists and that the size is not zero*/
	if (Copy_u8Buffer && Copy_u8Size!=0)
	{
		/*Call Send Function*/
		UART_voidSendSync(Copy_u32PeripheralNumber.BaseAddress, Copy_u8Buffer, Copy_u8Size, Copy_u32Time);
		Local_u8Status = STATUS_OK;
	}
	return Local_u8Status;
}
 /*Description: This API will be used to pass data buffer for receiving using polling.
  * Parameters: Desired UART (struct), Pointer to Data Buffer (u8*), size of data buffer (u8)
  * Return: Error Status (u8)  */
u8 HUART_u8ReceiveSync(UART_GPIO_t Copy_u32PeripheralNumber, u8 *Copy_u8Buffer, u8 Copy_u8Size, u32 Copy_u32Time)
{
	 /*This local variable will hold the status that will be returned at the end*/
	 	u8 Local_u8Status = STATUS_NOK;
	 	/*Check that data buffer exists and that the size is not zero*/
	 	if (Copy_u8Buffer && Copy_u8Size!=0)
	 	{
	 		/*Call Send Function*/
	 		Local_u8Status = UART_u8ReceiveSync(Copy_u32PeripheralNumber.BaseAddress ,Copy_u8Buffer, Copy_u8Size, Copy_u32Time);
	 	}
	 	return Local_u8Status;
}

/*Description: This function can be used to terminate async receiving (Warning!: Don't use it unless you know what you are doing)
 * Parameters: Desired UART Peripheral address (u32)
 * return: None*/
void HUART_voidTerminateReceiving (u32 Copy_u32DesiredUARTBaseAddress)
{
	/*Call Function from driver directly*/
	UART_voidTerminateReceiving (Copy_u32DesiredUARTBaseAddress);
}

/*Description: This function can be used to terminate async sending (Warning!: Don't use it unless you know what you are doing)
 * Parameters: Desired UART Peripheral address (u32)
 * return: None*/
void HUART_voidTerminateSending (u32 Copy_u32DesiredUARTBaseAddress)
{
	/*Call function from driver directly*/
	UART_voidTerminateSending (Copy_u32DesiredUARTBaseAddress);
}


