/*
 * UART_program.c
 *
 *  Created on: Mar 27, 2020
 *      Author: Mahmoud
 *      Version: 1.0
 */

#include "UART_interface.h"

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


/*These static variables will hold the call back functions*/
static TXCallback_t TXCallbackFunction=NULL;
static RXCallback_t RXCallbackFunction=NULL;

/*This static variable will hold the base address of the activated UART*/
static u32 UART_BaseAddress = NULL;

#define STATUS_BUSY				(u8)2
#define STATUS_IDLE				(u8)3

/*This struct will be used to create objects that will hold the data buffer and its status to be handled by the IRQ
 * The IRQ will contain a for loop that will keep looping on the array till the current position matches the size */
typedef struct{
	u8* dataArray;
	u8 size;
	u8 currentPosition;
	u8 bufferState;
}dataBuffer_t;

/*These static objects will hold the send and receive data buffers*/
static dataBuffer_t txBuffer = {NULL, 0,0, STATUS_IDLE};
static dataBuffer_t rxBuffer = {NULL, 0,0, STATUS_IDLE};

/*APIs*/


/*Description: This API will be configure the UART with the passed configurations
 * Parameters: Base Address (u32), Baudrate (u32), Stop Bits (u32), Parity Bits (u32)
 * Return:Error Status*/
u8 UART_u8Configure (u32 Copy_u32BaseAddress, u32 Copy_u32Baudrate, u32 Copy_u32StopBits, u32 Copy_u32ParityBits)
{

	/*This Local Variable will hold the status to be returned in the end*/
	u8 Local_u8Status = STATUS_NOK;

	/*Enable UART Peripheral*/
	*((u32*)(Copy_u32BaseAddress+UART_CR1)) |= UART_ENABLE_MASK;

	/*Write Baudrate inside the BRR register*/
	*((u32*)(Copy_u32BaseAddress+UART_BRR)) = Copy_u32Baudrate;

	/*Write corresponding stop bits to their location
	 * 1 stop bit needs to be ANDed
	 * 2 Stop bits needs to be ORed */
	if (Copy_u32StopBits == UART_STOP_BIT1_MASK)
	{
		*((u32*)(Copy_u32BaseAddress+UART_CR2)) &= Copy_u32StopBits;
	}
	else
	{
		*((u32*)(Copy_u32BaseAddress+UART_CR2)) |= Copy_u32StopBits;
	}

	/*Write Corresponding Parity bits to their location
	 * Parity Disabled needs to be ANDed
	 * Parity Even or ODD needs to be ORed*/
	if (Copy_u32ParityBits == UART_PARITY_DISABLED_MASK)
	{
		*((u32*)(Copy_u32BaseAddress+UART_CR1)) &= Copy_u32ParityBits;
		/*Return status as ok since we reached this step successfully*/
		Local_u8Status = STATUS_OK;
	}
	else
	{
		*((u32*)(Copy_u32BaseAddress+UART_CR1)) |= Copy_u32ParityBits;
		/*Return status as ok since we reached this step successfully*/
		Local_u8Status = STATUS_OK;
	}
	/*Enable Transmitter to send Idle frame*/
	*((u32*)(Copy_u32BaseAddress+UART_CR1)) |= UART_TX_ENABLE_MASK;
	/*Enable Receiver*/
	*((u32*)(Copy_u32BaseAddress+UART_CR1)) |= UART_RX_ENABLE_MASK;
	/*Save Base Address to be used later in functions*/
	UART_BaseAddress = Copy_u32BaseAddress;

	return Local_u8Status;
}

/*Description: This function will be used to trigger sending data. It will send only the first bit of the buffer and the rest will be handled by the interrupt request
 * Parameters: Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: void  */
void UART_voidSend(u8 *Copy_u8Buffer, u8 Copy_u8Size)
{


	/*Save the passed parameters in the txBuffer object*/
	txBuffer.dataArray = Copy_u8Buffer;
	txBuffer.size = Copy_u8Size;
	/*Send the first char of the data array and increment the current position. And the rest will be handled by the interrupt*/
	/*Clear Current value in register*/
	*((u32*)(UART_BaseAddress+UART_DR)) = 0x0;
	*((u32*)(UART_BaseAddress+UART_DR)) = txBuffer.dataArray[0];
			//txBuffer.dataArray[0];

	txBuffer.currentPosition++;
}
/*Description: This function will be used to trigger receiving data. It will receive only the first bit of the buffer and the rest will be handled by the interrupt request
 * Parameters:Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status */
u8 UART_u8Receive(u8 *Copy_u8Buffer, u8 Copy_u8Size)
{
	/*This local variable holds the status which will be returned at the end*/
	u8 Local_u8Status = STATUS_NOK;
	/*If current status is IDLE, then it means we are ready to receive new data*/
	if (rxBuffer.bufferState == STATUS_IDLE)
	{
		/*Save the passed parameters in the txBuffer object*/
		rxBuffer.dataArray = Copy_u8Buffer;
		rxBuffer.size = Copy_u8Size;

		/*Start receiving data and increment current position variable*/
		if (((*((u32*)(UART_BaseAddress+UART_SR)))&UART_PARITY_EVEN_MASK) || ((*((u32*)(UART_BaseAddress+UART_SR)))&UART_PARITY_EVEN_MASK))
		{
			rxBuffer.dataArray[0] = (*((u32*)(UART_BaseAddress+UART_DR)) & UART_PARITY_CANCELLATION_MASK);
			trace_printf("%d", rxBuffer.dataArray[0]);
		}
		else
		{
			rxBuffer.dataArray[0] = (*((u32*)(UART_BaseAddress+UART_DR)));
			trace_printf("%d", rxBuffer.dataArray[0]);
		}
		rxBuffer.currentPosition++;


		/*Change flag status to busy so that any new receive request is halted till the end*/
		rxBuffer.bufferState = STATUS_BUSY;
		Local_u8Status = STATUS_OK;
	}

	return Local_u8Status;
}

/*Description: This API will be used to enable or disable desired interrupt
 * Parameters: Desired Interrupt (u32), Desired Status (u8)
 * Return:Error Status */
u8 UART_u8EnableInterrupt(u32 Copy_u32DesiredInterrupt, u8 Copy_u8DesiredStatus)
{
	/*This local variable holds the status*/
	u8 Local_u8Status= STATUS_NOK;
	/*In case of enabling, the mask is ORed*/
	if (Copy_u8DesiredStatus == UART_INTERRUPT_ENABLE_MASK)
	{
		*((u32*)(UART_BaseAddress+UART_CR1)) |= Copy_u32DesiredInterrupt;
		Local_u8Status=STATUS_OK;

	}
	/*In case of Disabling, AND with complement*/
	else
	{
		*((u32*)(UART_BaseAddress+UART_CR1)) &= ~Copy_u32DesiredInterrupt;
		Local_u8Status=STATUS_OK;
	}
	return Local_u8Status;
}

/*Description: This function will save the passed callback by the user to the static variable
 * Parameters: Pointer to TX CallbackFunction
 * Return:Error Status */
u8 UART_u8SetTXCallBack(TXCallback_t Copy_TXCallbackFunction)
{
	TXCallbackFunction = Copy_TXCallbackFunction;
	return STATUS_OK;
}/*End of SetRXCallBack*

/*Description: This function will save the passed callback by the user to the static variable
 * Parameters: Pointer to RX CallbackFunction
 * Return:Error Status */
u8 UART_u8SetRXCallBack(RXCallback_t Copy_RXCallbackFunction)
{
	RXCallbackFunction = Copy_RXCallbackFunction;
	return STATUS_OK;
}/*End of SetRXCallBack*/

/*Interrupt Handler Implementation*/
void USART1_IRQHandler(void)
{
	/*Check which flag fired the interrupt request*/
	u32 volatile Local_u32RXFlag = *((u32*)(UART_BaseAddress+UART_DR)) & 0xFFFFFFFF;
	u32 volatile Local_u32TXFlag = *((u32*)(UART_BaseAddress+UART_SR)) & UART_TX_COMPLETE_MASK;


	/*If value of local variable representing RXNE is not zero, then it means it is the one that got fired*/
		if (Local_u32RXFlag)
		{
			/*Check that if status is busy, if so, then it means we are continuing off of an ongoing receiving operation and we should continue*/
			if (rxBuffer.bufferState == STATUS_BUSY)
			{
				/*Save current data in next position in array and increment current position*/
				rxBuffer.dataArray[rxBuffer.currentPosition] = (*((u32*)(UART_BaseAddress+UART_DR)))& UART_PARITY_CANCELLATION_MASK;
				trace_printf("%d",rxBuffer.dataArray[rxBuffer.currentPosition]);
				rxBuffer.currentPosition++;

				/*Check that we haven't reached the end of the array*/
				if (rxBuffer.currentPosition==rxBuffer.size)
				{
					/*If we reached the last position, then reset all of the variables and mark status as IDLE*/
					rxBuffer.dataArray = NULL;
					rxBuffer.currentPosition = 0;
					rxBuffer.size=0;
					rxBuffer.bufferState = STATUS_IDLE;
					/*Callback function*/
					RXCallbackFunction();
				}
			}
		}
	/*If value of local variable representing TXE is not zero, then it means it is the one that got fired*/
	if (Local_u32TXFlag)
	{
		/*Check if we have reached the last character, if we haven't reached it yet, send the next char*/
		if (txBuffer.currentPosition != (txBuffer.size))
		{
			/*Clear Current value in register*/
			*((u32*)(UART_BaseAddress+UART_DR)) = 0x0;
			/*Send the current char of the data array and increment the current position*/
			*((u32*)(UART_BaseAddress+UART_DR)) |= txBuffer.dataArray[txBuffer.currentPosition];
			txBuffer.currentPosition++;
		}
		/*If we reached the last character, then reset all the variables*/
		else
		{
			txBuffer.currentPosition = 0;
			txBuffer.size = 0;
			txBuffer.dataArray = NULL;

			/*Reset Flags*/
			*((u32*)(UART_BaseAddress+UART_SR)) &= ~ UART_TX_COMPLETE_MASK;
			/*Callback function*/
			TXCallbackFunction();
		}
	}

}


