

/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "STD_TYPES.h"
#include "GPIO_interface.h"
#include "RCC_interface.h"
#include "HUART_interface.h"
#include "Delay_interface.h"



// ----------------------------------------------------------------------------
//
// Standalone STM32F1 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

/*Prototypes for callback functions*/
//void txDone (void);
//void rxDone (void);

/*This is the variable that holds the data to be sent*/
u8 Local_u8DataTest[] = "Mahmoud";

int
main(int argc, char* argv[])
{

	/*This is the variable that will act as buffer for the data received*/
	u8 Local_u8Receive[5]={'0'};

	/*Activate system clock and select it
	We will use HSI as system clock for now*/
	//RCC_voidSWSelectClock(RCC_SW_HSI);
	RCC_f32GetPLLMultiplierValue();
	RCC_voidSetClockStatus(RCC_ENABLE_HSE);
	RCC_voidSWSelectClock(RCC_SW_HSE);
	//RCC_voidSetClockStatus(RCC_DISABLE_HSE);
	//RCC_voidSetClockStatus(RCC_DISABLE_PLL);
//	RCC_u16GetPrescalarValue(RCC_GET_AHB_PRESCALAR);
//	RCC_voidChangeBusPrescalar(RCC_AHB_PRESCALAR_16);
//	RCC_u16GetPrescalarValue(RCC_GET_AHB_PRESCALAR);
	//RCC_voidChangeBusPrescalar(RCC_APB2_PRESCALAR_8);
//	RCC_u16GetPrescalarValue(RCC_GET_APB2_PRESCALAR);


	/*Pass callback functions that will be called when sending or receiving is done*/
	//HUART_u8SetRXCallBack(rxDone);
	//HUART_u8SetTXCallBack(txDone);

	/*Initialize UART Peripheral with desired options*/
	HUART_u8Init(HUART_USART1, 9600, UART_STOP_BIT1, UART_PARITY_DISABLED);

	/*Enable Interrupts for the TX and RX*/
	//HUART_u8EnableInterrupt(HUART_USART1, UART_INTERRUPT_TX_COMPLETE, UART_INTERRUPT_ENABLE);
	//HUART_u8EnableInterrupt(HUART_USART1, UART_INTERRUPT_RX_NOT_EMPTY, UART_INTERRUPT_ENABLE);

	//trace_printf("While will begin now");

	//HUART_u8ReceiveAsync(HUART_USART1, Local_u8Receive, 5);
	//HUART_u8SendAsync(HUART_USART1, Local_u8DataTest, 5);
	//UART_voidSendSync(0x40004800, Local_u8DataTest, 5);
	//HUART_u8SendAsync(HUART_USART1, Local_u8DataTest, sizeof(Local_u8DataTest));
  /*Body of task
  This function will be called only once, then the whole process of sending and receiving will be handled using interrupt flags*/
  while (1)
    {
	  HUART_u8ReceiveSync(HUART_USART1, Local_u8Receive, 5);
	  if (Local_u8Receive[0]!='0')
	  {
		  HUART_u8SendSync(HUART_USART1, &Local_u8Receive[0], 1);
		  Local_u8Receive[0]='0';
	  }

	  //trace_printf("End of while\n");
    }
}
/*TX callback function */
void txDone (void)
{
	trace_printf("Transmission done");
}
/*RX Callback Function*/
void rxDone(void)
{
	trace_printf("Receiving Done");
}

#pragma GCC diagnostic pop
//
//// ----------------------------------------------------------------------------

