#include "RCC.h"
#include "GPIO.h"
#include "Bootloader.h"

int main(void)
{
	u8 Bootloader_Request_button_State;

	RCC_voidClkControl(HSE,ON);
	RCC_voidEnablePeripheral(APB2,RCC_APB2ENR_IOPBEN,ON);

	GPIO_Pin_t Bootloader_Request_button;
	Bootloader_Request_button.port = PORTB;
	Bootloader_Request_button.mode = GPIO_MODE_INPUT_PULLUP_PULLDOWN;
	Bootloader_Request_button.pin  = 12;
	Bootloader_Request_button.speed= GPIO_INPUT_MODE_RESET_STATE;

	GPIO_Init(&Bootloader_Request_button);
	GPIO_Pin_Write(&Bootloader_Request_button,ON);

	GPIO_Pin_Read(&Bootloader_Request_button,&Bootloader_Request_button_State);

	if(Bootloader_Request_button_State) bootloader_voidJumpToUserApp();
	else bootloader_voidUARTReadData();

	while(1)
	{

	}

	return 0;
}
