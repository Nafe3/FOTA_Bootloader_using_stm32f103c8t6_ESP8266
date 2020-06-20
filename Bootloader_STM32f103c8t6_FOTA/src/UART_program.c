/*
 * UART_program.h
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
 * 6) Added TX and RX buffers for every UART peripheral
 * */

/*Changelog from version 1.0:
 * 1) Added functionality to use multiple UART peripherals (and implemented their interrupt handlers)
 * 2) Added new functions to send and receive data sync
 * 3) Fixed a bug where an exception was thrown if there are no callback functions determined
 * */
#include "STD_TYPES.h"
#include "UART_interface.h"
#include "Delay_interface.h"

/*These static variables will hold the call back functions*/
static TXCallback_t TXCallbackFunctionUART1 = NULL;
static TXCallback_t TXCallbackFunctionUART2 = NULL;
static TXCallback_t TXCallbackFunctionUART3 = NULL;
static RXCallback_t RXCallbackFunctionUART1 = NULL;
static RXCallback_t RXCallbackFunctionUART2 = NULL;
static RXCallback_t RXCallbackFunctionUART3 = NULL;


#define STATUS_BUSY				(u8)2
#define STATUS_IDLE				(u8)3

/*This struct will be used to create objects that will hold the data buffer and its status to be handled by the IRQ
 * The IRQ will contain a for loop that will keep looping on the array till the current position matches the size */
typedef struct {
	u8* dataArray;
	u8 size;
	u8 currentPosition;
	u8 bufferState;
} dataBuffer_t;

/*These static objects will hold the send and receive data buffers*/
static dataBuffer_t txBufferUART1 = { NULL, 0, 0, STATUS_IDLE };
static dataBuffer_t rxBufferUART1 = { NULL, 0, 0, STATUS_IDLE };
static dataBuffer_t txBufferUART2 = { NULL, 0, 0, STATUS_IDLE };
static dataBuffer_t rxBufferUART2 = { NULL, 0, 0, STATUS_IDLE };
static dataBuffer_t txBufferUART3 = { NULL, 0, 0, STATUS_IDLE };
static dataBuffer_t rxBufferUART3 = { NULL, 0, 0, STATUS_IDLE };

/*APIs*/
/*Description: This API will be configure the UART with the passed configurations
 * Parameters: Base Address (u32), Baudrate (u16), Stop Bits (u32), Parity Bits (u32)
 * Return:Error Status*/
u8 UART_u8Configure(u32 Copy_u32BaseAddress, u16 Copy_u16Baudrate, u32 Copy_u32StopBits, u32 Copy_u32ParityBits) {

	/*This Local Variable will hold the status to be returned in the end*/
	u8 Local_u8Status = STATUS_NOK;

	/*Enable UART Peripheral*/
	*((u32*) (Copy_u32BaseAddress + UART_CR1 )) |= UART_ENABLE_MASK;

	/*Write Baudrate inside the BRR register*/
	*((u32*) (Copy_u32BaseAddress + UART_BRR )) = Copy_u16Baudrate;

	/*Write corresponding stop bits to their location
	 * 1 stop bit needs to be ANDed
	 * 2 Stop bits needs to be ORed */
	if (Copy_u32StopBits == UART_STOP_BIT1_MASK)
	{
		*((u32*) (Copy_u32BaseAddress + UART_CR2 )) &= Copy_u32StopBits;
	}
	else
	{
		*((u32*) (Copy_u32BaseAddress + UART_CR2 )) |= Copy_u32StopBits;
	}

	/*Write Corresponding Parity bits to their location
	 * Parity Disabled needs to be ANDed
	 * Parity Even or ODD needs to be ORed*/
	if (Copy_u32ParityBits == UART_PARITY_DISABLED_MASK)
	{
		*((u32*) (Copy_u32BaseAddress + UART_CR1 )) &= Copy_u32ParityBits;
		/*Return status as ok since we reached this step successfully*/
		Local_u8Status = STATUS_OK;
	}
	else
	{
		*((u32*) (Copy_u32BaseAddress + UART_CR1 )) |= Copy_u32ParityBits;
		/*Return status as ok since we reached this step successfully*/
		Local_u8Status = STATUS_OK;
	}
	/*Enable Transmitter to send Idle frame*/
	*((u32*) (Copy_u32BaseAddress + UART_CR1 )) |= UART_TX_ENABLE_MASK;
	/*Enable Receiver*/
	*((u32*) (Copy_u32BaseAddress + UART_CR1 )) |= UART_RX_ENABLE_MASK;

	return Local_u8Status;
}/*End of Configure*/

/*Description: This function will be used to trigger sending data. It will enable the interrupt, pass parameters to buffers and the rest will be handled by IRQ
 * Parameters: Desired UART Peripheral (u32), Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status */
u8 UART_voidSendAsync(u32 Copy_u32UARTAddress, u8 *Copy_u8Buffer, u8 Copy_u8Size)
{
	/*This local pointer will point to the proper struct according to chosen peripheral*/
	dataBuffer_t* Local_txBuffer;
	/*This local variable will hold the status of the function whether it succeeded or not*/
	u8 Local_u8Status = STATUS_NOK;

	/*Check which UART peripheral has this call for sending, and according to it, pass the one specified for this peripheral to the local object*/
	if (Copy_u32UARTAddress == UART_USART1_BASE_ADDRESS)
	{
		Local_txBuffer = &txBufferUART1;
	}
	else if (Copy_u32UARTAddress == UART_USART2_BASE_ADDRESS)
	{
		Local_txBuffer = &txBufferUART2;
	}
	else if (Copy_u32UARTAddress == UART_USART3_BASE_ADDRESS)
	{
		Local_txBuffer = &txBufferUART3;
	}

	/*If current status is IDLE, then it means we are ready to send new data and no current interrupt based sending on this UART is on*/
	if (Local_txBuffer->bufferState == STATUS_IDLE)
	{
		/*Save the passed parameters in the txBuffer object*/
		Local_txBuffer->dataArray = Copy_u8Buffer;
		Local_txBuffer->size = Copy_u8Size;

		/*Change status to busy*/
		Local_txBuffer->bufferState = STATUS_BUSY;

		/*Enable Interrupt that will be related to sending data*/
		UART_u8EnableInterrupt(Copy_u32UARTAddress, UART_TX_EMPTY_INTERRUPT_ENABLE_MASK, UART_INTERRUPT_ENABLE_MASK);
		/*Set status as successful*/
		Local_u8Status = STATUS_OK;
	}
	return Local_u8Status;
}/*End of SendAsync*/

/*Description: This function will be used to trigger receiving data. It will only trigger interrupt the rest will be handled by the interrupt request
 * Parameters:Desired UART Peripheral (u32), Pointer to Data Buffer (u8*), size of data buffer (u8)
 * Return: Error Status */
u8 UART_u8ReceiveAsync(u32 Copy_u32UARTAddress, u8 *Copy_u8Buffer, u8 Copy_u8Size)
{
	/*This local pointer will point to the proper struct according to chosen peripheral*/
	dataBuffer_t* Local_rxBuffer;

	/*This local variable holds the status which will be returned at the end*/
	u8 Local_u8Status = STATUS_NOK;

	/*Check which UART peripheral has this call for sending, and according to it, pass the one specified for this peripheral to the local object*/
	if (Copy_u32UARTAddress == UART_USART1_BASE_ADDRESS)
	{
		Local_rxBuffer = &rxBufferUART1;
	}
	else if (Copy_u32UARTAddress == UART_USART2_BASE_ADDRESS)
	{
		Local_rxBuffer = &rxBufferUART2;
	}
	else if (Copy_u32UARTAddress == UART_USART3_BASE_ADDRESS)
	{
		Local_rxBuffer = &rxBufferUART3;
	}

	/*If current status is IDLE, then it means we are ready to receive new data and no current interrupt based receiving on this UART is on*/
	if (Local_rxBuffer->bufferState == STATUS_IDLE)
	{
		/*Save the passed parameters in the txBuffer object*/
		Local_rxBuffer->dataArray = Copy_u8Buffer;
		Local_rxBuffer->size = Copy_u8Size;

		/*Enable Interrupt that will be related to sending data*/
		UART_u8EnableInterrupt(Copy_u32UARTAddress, UART_RX_NOT_EMPTY_INTERRUPT_ENABLE_MASK, UART_INTERRUPT_ENABLE_MASK);

		/*Change flag status to busy so that any new receive request is halted till the end*/
		Local_rxBuffer->bufferState = STATUS_BUSY;
		Local_u8Status = STATUS_OK;
	}
	return Local_u8Status;
}/*End of ReceiveAsync*/

/*Description: This function will be used to trigger sending data synchronously without using interrupts
 * Parameters: Desired UART Peripheral (u32), Pointer to Data Buffer (u8*), size of data buffer (u8), required delay in ms (u32)
 * Return: void  */
void UART_voidSendSync(u32 Copy_u32UARTAddress, u8 *Copy_u8Buffer, u8 Copy_u8Size, u32 Copy_u32Time)
{

	/*This local pointer will point to the proper struct according to chosen peripheral*/
	dataBuffer_t* Local_txBuffer;

	/*Check which UART peripheral has this call for sending, and according to it, pass the one specified for this peripheral to the local object*/
	if (Copy_u32UARTAddress == UART_USART1_BASE_ADDRESS)
	{
		Local_txBuffer = &txBufferUART1;
	}
	else if (Copy_u32UARTAddress == UART_USART2_BASE_ADDRESS)
	{
		Local_txBuffer = &txBufferUART2;
	}
	else if (Copy_u32UARTAddress == UART_USART3_BASE_ADDRESS)
	{
		Local_txBuffer = &txBufferUART3;
	}


	/*Save the passed parameters in the txBuffer object*/
	Local_txBuffer->dataArray = Copy_u8Buffer;
	Local_txBuffer->size = Copy_u8Size;

	/*Send data to DR using for loop*/
	for (Local_txBuffer->currentPosition = 0; Local_txBuffer->currentPosition < Local_txBuffer->size; Local_txBuffer->currentPosition++)
	{
		/*Send desired character*/
		*((u32*) (Copy_u32UARTAddress + UART_DR )) = Local_txBuffer->dataArray[Local_txBuffer->currentPosition];
		delay_ms(Copy_u32Time);
	}
	/*Reset everything*/
	Local_txBuffer->dataArray=NULL;
	Local_txBuffer->size = 0;
	Local_txBuffer->currentPosition = 0;

}/*End of SendSync*/

/*Description: This function will be used to trigger receiving data synchronously without using interrupts
 * Parameters: Desired UART Peripheral (u32), Pointer to Data Buffer (u8*), size of data buffer (u8), required delay in ms (u32)
 * Return: Error Status */
u8 UART_u8ReceiveSync(u32 Copy_u32UARTAddress, u8 *Copy_u8Buffer, u8 Copy_u8Size, u32 Copy_u32Time)
{
	/*This local variable holds the status which will be returned at the end*/
	u8 Local_u8Status = STATUS_NOK;

	/*This local pointer will point to the proper struct according to chosen peripheral*/
	dataBuffer_t* Local_rxBuffer;

	/*Check which UART peripheral has this call for sending, and according to it, pass the one specified for this peripheral to the local object*/
	if (Copy_u32UARTAddress == UART_USART1_BASE_ADDRESS)
	{
		Local_rxBuffer = &rxBufferUART1;
	}
	else if (Copy_u32UARTAddress == UART_USART2_BASE_ADDRESS)
	{
		Local_rxBuffer = &rxBufferUART2;
	}
	else if (Copy_u32UARTAddress == UART_USART3_BASE_ADDRESS)
	{
		Local_rxBuffer = &rxBufferUART3;
	}

	/*Save the passed parameters in the txBuffer object*/
	Local_rxBuffer->dataArray = Copy_u8Buffer;
	Local_rxBuffer->size = Copy_u8Size;
	Local_rxBuffer->currentPosition = 0;

	/*As long as there are data in the data register, and we haven't reached end of buffer yet, keep receiving*/
	while ((Local_rxBuffer->currentPosition < Local_rxBuffer->size))
	{
		/*Start receiving data and increment current position variable*/
		/*Since Parity is added to the data inside data register, make sure that you remove it in case it is found to prevent corrupt data*/
		if (((*((u32*) (Copy_u32UARTAddress + UART_CR1 ))) & UART_PARITY_EVEN_MASK ) || ((*((u32*) (Copy_u32UARTAddress + UART_CR1 ))) & UART_PARITY_EVEN_MASK ))
		{
			Local_rxBuffer->dataArray[Local_rxBuffer->currentPosition] = (*((u32*) (Copy_u32UARTAddress + UART_DR )) & UART_PARITY_CANCELLATION_MASK );
			//trace_printf("%c\n", rxBuffer.dataArray[rxBuffer.currentPosition]);
		} else
		{
			Local_rxBuffer->dataArray[Local_rxBuffer->currentPosition] = (*((u32*) (Copy_u32UARTAddress + UART_DR )));
			//trace_printf("%c\n", rxBuffer.dataArray[rxBuffer.currentPosition]);
		}
		/*Increment position*/
		Local_rxBuffer->currentPosition++;
		delay_ms(Copy_u32Time);
	}

	/*Reset everything*/
	Local_rxBuffer->dataArray=NULL;
	Local_rxBuffer->size = 0;
	Local_rxBuffer->currentPosition = 0;

	Local_u8Status = STATUS_OK;
	return Local_u8Status;
}/*End of ReceiveSync*/

/*Description: This API will be used to enable or disable desired interrupt
 * Parameters: Desired UART Peripheral(u32), Desired Interrupt (u32), Desired Status (u8)
 * Return:Error Status */
u8 UART_u8EnableInterrupt(u32 Copy_u32UARTAddress, u32 Copy_u32DesiredInterrupt, u8 Copy_u8DesiredStatus) {

	/*This local variable holds the status*/
	u8 Local_u8Status = STATUS_NOK;
	/*In case of enabling, the mask is ORed*/
	if (Copy_u8DesiredStatus == UART_INTERRUPT_ENABLE_MASK)
	{
		*((u32*) (Copy_u32UARTAddress + UART_CR1 )) |= Copy_u32DesiredInterrupt;
		Local_u8Status = STATUS_OK;
	}
	/*In case of Disabling, AND with complement*/
	else
	{
		*((u32*) (Copy_u32UARTAddress + UART_CR1 )) &= ~Copy_u32DesiredInterrupt;
		Local_u8Status = STATUS_OK;
	}
	return Local_u8Status;
}/*End of Enable Interrupt*/

/*Description: This function will save the passed callback by the user to the static variable
 * Parameters: Pointer to TX CallbackFunction, Desired UART Peripheral (u32)
 * Return:Error Status */
u8 UART_u8SetTXCallBack(TXCallback_t Copy_TXCallbackFunction, u32 Copy_u32DesiredUART) {
	if (Copy_u32DesiredUART == UART_USART1_BASE_ADDRESS)
	{
		TXCallbackFunctionUART1 = Copy_TXCallbackFunction;
	}
	else if (Copy_u32DesiredUART == UART_USART2_BASE_ADDRESS)
	{
		TXCallbackFunctionUART2 = Copy_TXCallbackFunction;
	}
	if (Copy_u32DesiredUART == UART_USART3_BASE_ADDRESS)
	{
		TXCallbackFunctionUART3 = Copy_TXCallbackFunction;
	}
	return STATUS_OK ;
}/*End of SetRXCallBack*/

/* Description: This function will save the passed callback by the user to the static variable
 * Parameters: Pointer to RX CallbackFunction, Desired UART Peripheral (u32)
 * Return:Error Status */
u8 UART_u8SetRXCallBack(RXCallback_t Copy_RXCallbackFunction, u32 Copy_u32DesiredUART)
{
	if (Copy_u32DesiredUART == UART_USART1_BASE_ADDRESS)
	{
		RXCallbackFunctionUART1 = Copy_RXCallbackFunction;
	}
	else if (Copy_u32DesiredUART == UART_USART2_BASE_ADDRESS)
	{
		RXCallbackFunctionUART2 = Copy_RXCallbackFunction;
	}
	else if (Copy_u32DesiredUART == UART_USART3_BASE_ADDRESS)
	{
		RXCallbackFunctionUART3 = Copy_RXCallbackFunction;
	}
	return STATUS_OK ;
}/*End of SetRXCallBack*/

/*Description: This function can be used to terminate async receiving (Warning!: Don't use it unless you know what you are doing)
 * Parameters: Desired UART Peripheral address (u32)
 * return: None*/
void UART_voidTerminateReceiving (u32 Copy_u32DesiredUARTBaseAddress)
{
	/*This local pointer will point to the proper struct according to chosen peripheral*/
	dataBuffer_t* Local_rxBuffer;

	/*This local variable holds the status which will be returned at the end*/
	u8 Local_u8Status = STATUS_NOK;

	/*Check which UART peripheral has this call for sending, and according to it, pass the one specified for this peripheral to the local object*/
	if (Copy_u32DesiredUARTBaseAddress== UART_USART1_BASE_ADDRESS)
	{
		Local_rxBuffer = &rxBufferUART1;
	}
	else if (Copy_u32DesiredUARTBaseAddress == UART_USART2_BASE_ADDRESS)
	{
		Local_rxBuffer = &rxBufferUART2;
	}
	else if (Copy_u32DesiredUARTBaseAddress== UART_USART3_BASE_ADDRESS)
	{
		Local_rxBuffer = &rxBufferUART3;
	}

	/*Reset all parameters*/
	Local_rxBuffer->dataArray = NULL;
	Local_rxBuffer->currentPosition = 0;
	Local_rxBuffer->size = 0;
	Local_rxBuffer->bufferState = STATUS_IDLE;

	/*Disable Interrupt*/
	UART_u8EnableInterrupt(Copy_u32DesiredUARTBaseAddress, UART_RX_NOT_EMPTY_INTERRUPT_ENABLE_MASK, UART_INTERRUPT_DISABLE_MASK);
}

/*Description: This function can be used to terminate async sending (Warning!: Don't use it unless you know what you are doing)
 * Parameters: Desired UART Peripheral address (u32)
 * return: None*/
void UART_voidTerminateSending (u32 Copy_u32DesiredUARTBaseAddress)
{
	/*This local pointer will point to the proper struct according to chosen peripheral*/
	dataBuffer_t* Local_txBuffer;

	/*This local variable holds the status which will be returned at the end*/
	u8 Local_u8Status = STATUS_NOK;

	/*Check which UART peripheral has this call for sending, and according to it, pass the one specified for this peripheral to the local object*/
	if (Copy_u32DesiredUARTBaseAddress== UART_USART1_BASE_ADDRESS)
	{
		Local_txBuffer = &txBufferUART1;
	}
	else if (Copy_u32DesiredUARTBaseAddress == UART_USART2_BASE_ADDRESS)
	{
		Local_txBuffer = &txBufferUART2;
	}
	else if (Copy_u32DesiredUARTBaseAddress== UART_USART3_BASE_ADDRESS)
	{
		Local_txBuffer = &txBufferUART3;
	}

	/*Reset all parameters*/
	Local_txBuffer->dataArray = NULL;
	Local_txBuffer->currentPosition = 0;
	Local_txBuffer->size = 0;
	Local_txBuffer->bufferState = STATUS_IDLE;

	/*Reset Flags*/
	*((u32*) (Copy_u32DesiredUARTBaseAddress + UART_SR )) &= ~ UART_TX_COMPLETE_MASK;

	/*Disable Interrupt*/
	UART_u8EnableInterrupt(Copy_u32DesiredUARTBaseAddress, UART_TX_EMPTY_INTERRUPT_ENABLE_MASK, UART_INTERRUPT_DISABLE_MASK);
}

/*Interrupt Handler Implementation*/
void USART1_IRQHandler(void) {
	/*Check which flag fired the interrupt request*/
	u32 volatile Local_u32RXFlag = *((u32*) (UART_USART1_BASE_ADDRESS + UART_DR )) & 0xFFFFFFFF;
	u32 volatile Local_u32TXFlag = *((u32*) (UART_USART1_BASE_ADDRESS + UART_SR )) & UART_TX_EMPTY_MASK;

	/*This is a local variable that will check whether the data that we are currently receiving is really data or empty data from register*/
	u8 Local_u8DRValue = 0;


	/*If value of local variable representing data inside data register is not zero, then it means it is the one that got fired*/
	if (Local_u32RXFlag)
	{
		/*Check that if status is busy, if so, then it means we are continuing off of an ongoing receiving operation and we should continue*/
		if (rxBufferUART1.bufferState == STATUS_BUSY)
		{
			/*Save current data in next position in local variable and check whether this is really data or empty value*/
			/*Start receiving data and increment current position variable*/
			if (((*((u32*) (UART_USART1_BASE_ADDRESS + UART_CR1 ))) & UART_PARITY_EVEN_MASK ) || ((*((u32*) (UART_USART1_BASE_ADDRESS + UART_CR1 ))) & UART_PARITY_EVEN_MASK ))
			{
				Local_u8DRValue = (*((u32*) (UART_USART1_BASE_ADDRESS + UART_DR ))) & UART_PARITY_CANCELLATION_MASK;
				//trace_printf("%c\n", rxBufferUART1.dataArray[0]);
			}
			else
			{
				Local_u8DRValue = (*((u32*) (UART_USART1_BASE_ADDRESS + UART_DR )));
				//trace_printf("%c\n", rxBufferUART1.dataArray[0]);
			}
			/*If data in local variable now is not equal to 0 (NULL), then it is really data and put it in the array*/
			if (Local_u8DRValue != 0)
			{
				rxBufferUART1.dataArray[rxBufferUART1.currentPosition] = Local_u8DRValue;
				//trace_printf("%d\n", rxBufferUART1.dataArray[rxBufferUART1.currentPosition]);
				/*Increment position we are currently pointing to in the array*/
				rxBufferUART1.currentPosition++;
			}

			/*Check that we haven't reached the end of the array*/
			if (rxBufferUART1.currentPosition == rxBufferUART1.size)
			{
				/*Call terminate receiving function which will reset all flags and disable interrupt*/
				UART_voidTerminateReceiving(UART_USART1_BASE_ADDRESS);
				/*If there is a Callback function, then call it*/
				if (RXCallbackFunctionUART1 != NULL)
				{
					RXCallbackFunctionUART1();
				}
			}
		}
	}

	/*If value of local variable representing TXE is not zero, then it means it is the one that got fired*/
	if (Local_u32TXFlag)
	{
		/*Check that we came here because of a previous function call using status flag*/
		if (txBufferUART1.bufferState == STATUS_BUSY)
		{
			/*Check if we have reached the last character, if we haven't reached it yet, send the next char*/
			if (txBufferUART1.currentPosition != (txBufferUART1.size))
			{
				/*Send the current char of the data array and increment the current position*/
				*((u32*) (UART_USART1_BASE_ADDRESS + UART_DR )) = txBufferUART1.dataArray[txBufferUART1.currentPosition];
				//trace_printf("Sending %c", txBufferUART1.dataArray[txBufferUART1.currentPosition]);
				txBufferUART1.currentPosition++;
			}
			/*If we reached the last character, then reset all the variables*/
			else
			{
				/*Call terminate sending function which should reset all flags*/
				UART_voidTerminateSending(UART_USART1_BASE_ADDRESS);

				/*If there is a Callback function, then call it*/
				if (TXCallbackFunctionUART1 != NULL)
				{
					TXCallbackFunctionUART1();
				}
			}
		}
	}
}/*End of USART1_IRQHandler*/

/*Interrupt Handler Implementation*/
void USART2_IRQHandler(void) {
	/*Check which flag fired the interrupt request*/
	u32 volatile Local_u32RXFlag = *((u32*) (UART_USART2_BASE_ADDRESS + UART_DR )) & 0xFFFFFFFF;
	u32 volatile Local_u32TXFlag = *((u32*) (UART_USART2_BASE_ADDRESS + UART_SR )) & UART_TX_EMPTY_MASK;

	/*This is a local variable that will check whether the data that we are currently receiving is really data or empty data from register*/
	u8 Local_u8DRValue = 0;

	/*If value of local variable representing data inside data register is not zero, then it means it is the one that got fired*/
	if (Local_u32RXFlag)
	{
		/*Check that if status is busy, if so, then it means we are continuing off of an ongoing receiving operation and we should continue*/
		if (rxBufferUART2.bufferState == STATUS_BUSY)
		{
			/*Save current data in next position in local variable and check whether this is really data or empty value*/
			/*Start receiving data and increment current position variable*/
			if (((*((u32*) (UART_USART2_BASE_ADDRESS + UART_CR1 ))) & UART_PARITY_EVEN_MASK ) || ((*((u32*) (UART_USART2_BASE_ADDRESS + UART_CR1 ))) & UART_PARITY_EVEN_MASK ))
			{
				Local_u8DRValue = (*((u32*) (UART_USART2_BASE_ADDRESS + UART_DR ))) & UART_PARITY_CANCELLATION_MASK;
				//trace_printf("%c\n", rxBufferUART2.dataArray[0]);
			}
			else
			{
				Local_u8DRValue = (*((u32*) (UART_USART2_BASE_ADDRESS + UART_DR )));
				//trace_printf("%c\n", rxBufferUART2.dataArray[0]);
			}
			/*If data in local variable now is not equal to 0 (NULL), then it is really data and put it in the array*/
			if (Local_u8DRValue != 0)
			{
				rxBufferUART2.dataArray[rxBufferUART2.currentPosition] = Local_u8DRValue;
				//trace_printf("%d\n", rxBufferUART2.dataArray[rxBufferUART2.currentPosition]);
				/*Increment position we are currently pointing to in the array*/
				rxBufferUART2.currentPosition++;
			}

			/*Check that we haven't reached the end of the array*/
			if (rxBufferUART2.currentPosition == rxBufferUART2.size)
			{
				/*Call terminate receiving function which will reset all flags and disable interrupt*/
				UART_voidTerminateReceiving(UART_USART2_BASE_ADDRESS);
				/*If there is a Callback function, then call it*/
				if (RXCallbackFunctionUART2 != NULL)
				{
					RXCallbackFunctionUART2();
				}
			}
		}
	}

	/*If value of local variable representing TXE is not zero, then it means it is the one that got fired*/
	if (Local_u32TXFlag)
	{
		/*Check that we came here because of a previous function call using status flag*/
		if (txBufferUART2.bufferState == STATUS_BUSY)
		{
			/*Check if we have reached the last character, if we haven't reached it yet, send the next char*/
			if (txBufferUART2.currentPosition != (txBufferUART2.size))
			{
				/*Send the current char of the data array and increment the current position*/
				*((u32*) (UART_USART2_BASE_ADDRESS + UART_DR )) = txBufferUART2.dataArray[txBufferUART2.currentPosition];
				//trace_printf("Sending %c", txBufferUART2.dataArray[txBufferUART2.currentPosition]);
				txBufferUART2.currentPosition++;
			}
			/*If we reached the last character, then reset all the variables*/
			else
			{
				/*Call terminate sending function which should reset all flags*/
				UART_voidTerminateSending(UART_USART2_BASE_ADDRESS);

				/*If there is a Callback function, then call it*/
				if (TXCallbackFunctionUART2 != NULL)
				{
					TXCallbackFunctionUART2();
				}
			}
		}
	}
}/*End of USART2_IRQHandler*/

/*Interrupt Handler Implementation*/
void USART3_IRQHandler(void) {
	/*Check which flag fired the interrupt request*/
	u32 volatile Local_u32RXFlag = *((u32*) (UART_USART3_BASE_ADDRESS + UART_DR )) & 0xFFFFFFFF;
	u32 volatile Local_u32TXFlag = *((u32*) (UART_USART3_BASE_ADDRESS + UART_SR )) & UART_TX_EMPTY_MASK;

	/*This is a local variable that will check whether the data that we are currently receiving is really data or empty data from register*/
	u8 Local_u8DRValue = 0;

	/*If value of local variable representing data inside data register is not zero, then it means it is the one that got fired*/
	if (Local_u32RXFlag)
	{
		/*Check that if status is busy, if so, then it means we are continuing off of an ongoing receiving operation and we should continue*/
		if (rxBufferUART3.bufferState == STATUS_BUSY)
		{
			/*Save current data in next position in local variable and check whether this is really data or empty value*/
			/*Start receiving data and increment current position variable*/
			if (((*((u32*) (UART_USART3_BASE_ADDRESS + UART_CR1 ))) & UART_PARITY_EVEN_MASK ) || ((*((u32*) (UART_USART3_BASE_ADDRESS + UART_CR1 ))) & UART_PARITY_EVEN_MASK ))
			{
				Local_u8DRValue = (*((u32*) (UART_USART3_BASE_ADDRESS + UART_DR ))) & UART_PARITY_CANCELLATION_MASK;
				//trace_printf("%c\n", rxBufferUART3.dataArray[0]);
			}
			else
			{
				Local_u8DRValue = (*((u32*) (UART_USART3_BASE_ADDRESS + UART_DR )));
				//trace_printf("%c\n", rxBufferUART3.dataArray[0]);
			}
			/*If data in local variable now is not equal to 0 (NULL), then it is really data and put it in the array*/
			if (Local_u8DRValue != 0)
			{
				rxBufferUART3.dataArray[rxBufferUART3.currentPosition] = Local_u8DRValue;
				//trace_printf("%d\n", rxBufferUART3.dataArray[rxBufferUART3.currentPosition]);
				/*Increment position we are currently pointing to in the array*/
				rxBufferUART3.currentPosition++;
			}

			/*Check that we haven't reached the end of the array*/
			if (rxBufferUART3.currentPosition == rxBufferUART3.size)
			{
				/*Call terminate receiving function which will reset all flags and disable interrupt*/
				UART_voidTerminateReceiving(UART_USART3_BASE_ADDRESS);
				/*If there is a Callback function, then call it*/
				if (RXCallbackFunctionUART3 != NULL)
				{
					RXCallbackFunctionUART3();
				}
			}
		}
	}

	/*If value of local variable representing TXE is not zero, then it means it is the one that got fired*/
	if (Local_u32TXFlag)
	{
		/*Check that we came here because of a previous function call using status flag*/
		if (txBufferUART3.bufferState == STATUS_BUSY)
		{
			/*Check if we have reached the last character, if we haven't reached it yet, send the next char*/
			if (txBufferUART3.currentPosition != (txBufferUART3.size))
			{
				/*Send the current char of the data array and increment the current position*/
				*((u32*) (UART_USART3_BASE_ADDRESS + UART_DR )) = txBufferUART3.dataArray[txBufferUART3.currentPosition];
				//trace_printf("Sending %c", txBufferUART3.dataArray[txBufferUART3.currentPosition]);
				txBufferUART3.currentPosition++;
			}
			/*If we reached the last character, then reset all the variables*/
			else
			{
				/*Call terminate sending function which should reset all flags*/
				UART_voidTerminateSending(UART_USART3_BASE_ADDRESS);

				/*If there is a Callback function, then call it*/
				if (TXCallbackFunctionUART3 != NULL)
				{
					TXCallbackFunctionUART3();
				}
			}
		}
	}
}/*End of USART3_IRQHandler*/
