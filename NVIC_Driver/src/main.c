#include "NVIC.h"
#include "Trace.h"
void EXTI0_IRQHandler(void)
{
	trace_printf("a7ooooooo");
}

void main(void)
{
	//u8 koko=0;
	//u8EnableEXTI(31);
	//u8IsActive(31,&koko);
	//u8EnableEXTI(32);
	//NVIC_voidEnableAllEXTI();
	//NVIC_voidFilterEXTI_lowerThan(1);
	//NVIC_voidFilterEXTI_lowerThan(2);
	//NVIC_voidFilterEXTI_lowerThan(3);
	//NVIC_voidFilterEXTI_lowerThan(4);
	//NVIC_voidFilterEXTI_lowerThan(5);
	//NVIC_voidFilterEXTI_lowerThan(6);
	//NVIC_voidFilterEXTI_lowerThan(7);
	//NVIC_voidFilterEXTI_lowerThan(8);
	//NVIC_u8EnableEXTI(40);
	//NVIC_u8EnableEXTI(41);
	//NVIC_u8EnableEXTI(42);
	//NVIC_u8EnableEXTI(43);
	//NVIC_u8EnableEXTI(44);
	//NVIC_u8EnableEXTI(45);
	//NVIC_u8EnableEXTI(46);
	//NVIC_u8EnableEXTI(47);
	//NVIC_u8EnableEXTI(48);
	//NVIC_u8EnableEXTI(49);
	//NVIC_u8EnableEXTI(50);
	//NVIC_u8EnableEXTI(51);
	//NVIC_u8EnableEXTI(60);
	//NVIC_u8EnableEXTI(61);
	//NVIC_u8EnableEXTI(65);
	//NVIC_u8EnableEXTI(40);
	//NVIC_u8EnableEXTI(40);
	//NVIC_u8EnableEXTI(40);
	//NVIC_u8EnableEXTI(40);
	//NVIC_u8EnableEXTI(40);
	//NVIC_u8EnableEXTI(40);
	//NVIC_u8EnableEXTI(40);
	//NVIC_u8EnableEXTI(40);
	//NVIC_u8EnableEXTI(40);
	//NVIC_u8EnableEXTI(40);
	//NVIC_u8EnableEXTI(40);
	//NVIC_u8EnableEXTI(40);
	NVIC_u8EnableEXTI(6) ;
	NVIC_u8SetPendFlag(6);

}
