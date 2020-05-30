#include "STD_TYPES.h"
#include "RCC.h"
#include"RCC_private.h"
#include "CRC.h"
#include "Trace.h"

#define CRC_BASE_ADDRESS 					0x40023000
#define CRC_DR								*((volatile u32*)(CRC_BASE_ADDRESS+0x00))
#define CRC_CR								*((volatile u32*)(CRC_BASE_ADDRESS+0x08))

int main(void)
{
	u8 i;
	u32 iData;
	u8 value[]={0x05,0x51};
	//u32 value = 0x88888888;
	volatile u32 return_val;
	RCC_voidClkControl(HSE,ON);
	RCC_voidEnablePeripheral(AHB,RCC_AHBENR_CRCEN,ON);
	//RCC_CR=RCC_CR_HSEON;
	//CRC_DR = 0x55550000;
	//return_val = CRC_DR;
	//return_val= CRC_DR;
	//trace_printf("%x",return_val);
	//CRC_ResetDR();
	//value =0x40023400;
	//asm ("mov  r11, #0");

	//asm ("mov  r11, #0x3400");
	//asm ("movt r11, #0x4002");
	//asm ("mov  r7, #1");
	//asm ("str  r11, [r7, #0]");

	while(1)
	{
		//CRC_CalcCRC(value,&return_val);
		//return_val = CRC_CalcCRC(value);
		for(i=0;i<2;i++)
		{
			iData=value[i];
			return_val = CRC_CalcBlockCRC(&iData,1);
		}
		//	return_val &= 0xffffffff;//Skipped always
		trace_printf("%x \n",return_val);
		CRC_ResetDR();

	}
	return 0;
}
