#include "RCC.h"
#include "HUART_interface.h"
#include "Delay_interface.h"
#include "Debug.h"
#include"Trace.h"

uint32_t num=0;
void func(void)
{
	trace_printf("a7eh %d\n",num);
}
void func2(void)
{
	trace_printf("RX \n");
}

int main(void)
{
	//u8 str[]="Hello %d \r\n";
	//u8 str[]="Hello %d \r\n";
	u8 byte;
	RCC_voidClkControl(HSE,ON);

	HUART_u8Init(UART_USART1,UART_BAUDRATE_115200,UART_STOP_BIT1,UART_PARITY_DISABLED);
	//HUART_u8Init(UART_USART2,UART_BAUDRATE_115200,UART_STOP_BIT1,UART_PARITY_DISABLED);
	//HUART_u8SetTXCallBack(func);
	//HUART_u8SetRXCallBack(func2);
	//HUART_u8EnableInterrupt(UART_INTERRUPT_TX_COMPLETE, UART_INTERRUPT_ENABLE);
	//HUART_u8EnableInterrupt(UART_INTERRUPT_RX_NOT_EMPTY, UART_INTERRUPT_ENABLE);
	printmsg("WELCOME TO ((( PRINT HEX )))\r\n");
	while(1)
	{
		printmsg("0x%x\r\n", num);

		//HUART_u8Receive(&byte,1);

		num++;
		delay_ms(10000);
	}
}
