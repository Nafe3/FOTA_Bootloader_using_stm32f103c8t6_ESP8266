#include "STD_TYPES.h"
#include "RCC.h"
//#include "CRC.h"
#include "Trace.h"

#define CRC_BASE_ADDRESS 					0x40023400
#define CRC_DR								*((volatile u32*)(CRC_BASE_ADDRESS+0x00))
#define CRC_CR								*((volatile u32*)(CRC_BASE_ADDRESS+0x08))

int main(void)
{
	//u32 value=0x88888888;
	//u32 return_val;

	RCC_voidClkControl(HSE,ON);
	RCC_voidEnablePeripheral(AHB,RCC_AHBENR_CRCEN,ON);
	CRC_DR = 0x55550000;
	CRC_CR = 1;
	//CRC_ResetDR();
	while(1)
	{
		//return_val = CRC_CalcCRC(value);
		//trace_printf("%x \n",return_val);
	}
	return 0;
}
