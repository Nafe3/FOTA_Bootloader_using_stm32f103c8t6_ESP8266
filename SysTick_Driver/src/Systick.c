/*
 * Systick.c
 *
 *  Created on: Mar 10, 2020
 *      Author: moham
 */
#include "STD_TYPES.h"
#include "Systick.h"
#include "Systick_config.h"
#include "..\system\include\diag\Trace.h"
#include "..\system\include\cortexm\ExceptionHandlers.h"

#include "HRCC.h"

#define STK_BASE_ADDRESS		0XE000E010
#define STK_CTRL			    *((volatile u32*)(STK_BASE_ADDRESS+0x00))
#define STK_LOAD                *((volatile u32*)(STK_BASE_ADDRESS+0x04))
#define STK_VAL                 *((volatile u32*)(STK_BASE_ADDRESS+0x08))
#define STK_CALIB               *((volatile u32*)(STK_BASE_ADDRESS+0x0C))


static u32 Cov_Count;
static u8  Cov_flag;
static u32 reload_again;


static systickcbf_t APPcbf;			//global variable(pointer to function to save the user's ISR function address

/*this is the second step after setting the callback function*/
void SysT_Handler(void)
{
	static u32 counter = 0;
	counter++;


	if(Cov_flag)
	{

		/*checking if the user has set the callback function or not*/
		if( (APPcbf) && (counter== (Cov_Count+1)))
		{
			APPcbf();
			counter =0;
			STK_LOAD = reload_again;
			STK_VAL  = 0 ;
		}
		if(counter == 1)
		{
			STK_LOAD = 0xFFFFFF;
			STK_VAL  = 0 ;
		}
	}
	else
	{
		/*checking if the user has set the callback function or not*/
		if( (APPcbf) && (counter== (Cov_Count+1)))
		{
			APPcbf();
			counter =0;
		}
	}


}

/////////////////////////////////////////////////////////////////
/*saving the the pointer to the user's function in a global variable*/
/*This is step 1*/
extern void SYSTICK_SetCallback(systickcbf_t cbf)
{
	if(cbf)
	{
		APPcbf=cbf;
	}
}

/////////////////////////////////////////////////////////////////
extern void SYSTICK_Init		(void)
{
	/* enable systick interrupt - clock =AHB/8*/
	/*stop systick*/
	STK_CTRL |= 0x00000006;

}

/////////////////////////////////////////////////////////////////
//
// extern void SYSTICK_Start(u32 Copy_u32Value)
// {
// 	/*stop system timer*/
// 	STK_CTRL &=~(0x1);
// 	/*load the value into load reg*/
// 	STK_LOAD = Copy_u32Value;
// 	/*clear value register*/
// 	STK_VAL=0;
// 	/*start system timer*/
// 	STK_CTRL |= 0x1;
// }
//

/////////////////////////////////////////////////////////////////
extern void SYSTICK_Start(void)
{
	/*stop system timer*/
	STK_CTRL &=~(0x1);
	/*start system timer*/
	STK_CTRL |= 0x1;

}

/////////////////////////////////////////////////////////////////
extern void SYSTICK_Stop(void)
{
	/*stop system timer*/
	STK_CTRL &=~ 0x1;
}

/////////////////////////////////////////////////////////////////
extern void SYSTICK_SetTime_us(u32 timeus)
{

	/* 1- get F(sys)  -> HRCC
	 * 2- F(timer)= F(sys)/Prescaler 	-> get from user's configurations
	 * 3- T(tick)= 1/F(timer)
	 * 4- T(overflow) = 2^N * T(tick)  -> put N in the configurations (here it is 24 bits)
	 * 5- C(overflow) = (timems*1000)/T(overflow)
	 * */
	u32 Fsys   = HRCC_getSystemClk();
	trace_printf("Fsys = %lu\n",Fsys);

	f32 Ftimer = Fsys/Clk_pre;
	trace_printf("Ftimer = %f\n",Ftimer);

	f32 Ttick  = 1000000/Ftimer;   //1000000 us = 1 second
	trace_printf("Ttick = %f\n",Ttick);

	f32 Tov    = (2<<23) * Ttick;//timer register is 24 bit wide
	trace_printf("Tov = %f\n",Tov);

	f32 Cov    = timeus/Tov;
	trace_printf("Cov = %f\n",Cov);

	Cov_Count = (u32)Cov;
	trace_printf("Cov_Count = %lu\n",Cov_Count);

	if(Cov_Count) Cov_flag = 1;
	else 	      Cov_flag = 0;

	Cov -= Cov_Count;
	trace_printf("Cov = %f\n",Cov);

	u32 Preload_value = Cov * (2<<23);
	trace_printf("Preload = %lu\n\n",Preload_value);

	reload_again = Preload_value ;

	/*load the value into load reg*/
	STK_LOAD = Preload_value;
	/*clear value register*/
	STK_VAL=0;
}

/////////////////////////////////////////////////////////////////
extern void SYSTICK_SetTime_ms	(u32 timems)
{
	/* 1- get F(sys)  -> HRCC
	 * 2- F(timer)= F(sys)/Prescaler 	-> get from user's configurations
	 * 3- T(tick)= 1/F(timer)
	 * 4- T(overflow) = 2^N * T(tick)  -> put N in the configurations (here it is 24 bits)
	 * 5- C(overflow) = (timems*1000)/T(overflow)
	 * */
	u32 Fsys   = HRCC_getSystemClk();
	trace_printf("Fsys = %lu\n",Fsys);

	f32 Ftimer = Fsys/Clk_pre;
	trace_printf("Ftimer = %f\n",Ftimer);

	f32 Ttick  = 1000/Ftimer;   //1000 ms = 1 second
	trace_printf("Ttick = %f\n",Ttick);

	f32 Tov    = (2<<23) * Ttick;//timer register is 24 bit wide
	trace_printf("Tov = %f\n",Tov);

	f32 Cov    = timems/Tov;
	trace_printf("Cov = %f\n",Cov);

	Cov_Count = (u32)Cov;
	trace_printf("Cov_Count = %lu\n",Cov_Count);

	if(Cov_Count) Cov_flag = 1;
	else		  Cov_flag = 0;

	Cov -= Cov_Count;
	trace_printf("Cov = %f\n",Cov);

	u32 Preload_value = Cov * (2<<23);
	trace_printf("Preload = %lu\n\n",Preload_value);

	reload_again = Preload_value ;

	/*load the value into load reg*/
	STK_LOAD = Preload_value;
	/*clear value register*/
	STK_VAL=0;
}

/////////////////////////////////////////////////////////////////
extern void SYSTICK_Increment10Hz	(void)
{

}

/////////////////////////////////////////////////////////////////
extern void SYSTICK_Decrement10Hz	(void)
{

}
