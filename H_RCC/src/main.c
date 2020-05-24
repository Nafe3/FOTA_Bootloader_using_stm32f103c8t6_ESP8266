#include "STD_TYPES.h"
#include "HRCC.h"
#include "RCC.h"
void main(void)
{
	u32 clk;
	RCC_voidSelectSysClk(HSI);
	clk = HRCC_getSystemClk();
	while(1)
	{

	}
}
