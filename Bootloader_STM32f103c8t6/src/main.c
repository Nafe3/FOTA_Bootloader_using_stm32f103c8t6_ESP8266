#include "RCC.h"
#include "GPIO.h"
#include "Bootloader.h"
#include "HUART_interface.h"

void func(void)
{

}
void func2(void)
{

}


int main(void)
{
	u8 Bootloader_Request_button_State;


	RCC_voidClkControl(HSE,ON);

	HUART_u8Init(UART_USART1,UART_BAUDRATE_115200,UART_STOP_BIT1,UART_PARITY_DISABLED);
	HUART_u8SetTXCallBack(func);
	HUART_u8SetRXCallBack(func2);
	HUART_u8EnableInterrupt(UART_INTERRUPT_TX_COMPLETE, UART_INTERRUPT_ENABLE);


	RCC_voidEnablePeripheral(APB2,RCC_APB2ENR_IOPBEN,ON);

	GPIO_Pin_t Bootloader_Request_button;
	Bootloader_Request_button.port = PORTB;
	Bootloader_Request_button.mode = GPIO_MODE_INPUT_PULLUP_PULLDOWN;
	Bootloader_Request_button.pin  = 12;
	Bootloader_Request_button.speed= GPIO_INPUT_MODE_RESET_STATE;

	GPIO_Init(&Bootloader_Request_button);
	GPIO_Pin_Write(&Bootloader_Request_button,ON);

	GPIO_Pin_Read(&Bootloader_Request_button,&Bootloader_Request_button_State);

	if(Bootloader_Request_button_State) bootloader_voidJumpToUserApp(); //if the button is not pressed
	else bootloader_voidUARTReadData();									//if the button is  pressed

	while(1)
	{

	}

	return 0;
}
