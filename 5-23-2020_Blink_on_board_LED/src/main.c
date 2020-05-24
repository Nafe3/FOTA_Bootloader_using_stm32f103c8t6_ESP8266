#include "STD_TYPES.h"
#include "RCC.h"
#include "GPIO.h"
#include "Delay_interface.h"

int main(void)
{
	RCC_voidClkControl(HSE,ON);
	RCC_voidEnablePeripheral(APB2,RCC_APB2ENR_IOPCEN,ON);

	GPIO_Pin_t C13_LED;
	C13_LED.mode = GPIO_MODE_OUTPUT_PUSH_PULL;
	C13_LED.pin  = 13;
	C13_LED.speed= GPIO_OUTPUT_SPEED_50MHz;

	GPIO_Init(PORTC,&C13_LED);

	while(1)
	{
		GPIO_Pin_Write(PORTC,&C13_LED,OFF);
		delay_ms(10000);
		GPIO_Pin_Write(PORTC,&C13_LED,ON);
		delay_ms(10000);
	}
	return 0;
}
