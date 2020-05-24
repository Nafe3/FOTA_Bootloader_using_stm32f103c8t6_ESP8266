#include "STD_TYPES.h"
#include "GPIO.h"
#include "RCC.h"
#include "Systick.h"


u8 counter=0;
GPIO_Pin_t led;

static u8  volatile f=0;
void func (void)
{
	f^=1;
		GPIO_Pin_Write(PORTC,&led,f);

}

int main(void)
{

	led.mode = GPIO_MODE_OUTPUT_PUSH_PULL;
	led.pin  = GPIO_PIN_13;
	led.speed= GPIO_OUTPUT_SPEED_2MHz;

	RCC_voidSelectSysClk(2);
	RCC_voidEnablePeripheral(APB2,RCC_APB2ENR_IOPCEN,1);

	GPIO_Init(PORTC,&led);
	//GPIO_Pin_Write(PORTC,&led,1);
	SYSTICK_Init();
	SYSTICK_SetCallback(func);
	SYSTICK_SetTime_us(5000000);
	SYSTICK_Start();
	while(1);
}
