/* libs */
#include "STD_TYPES.h"
/* own */
#include "Delay_interface.h"
#include "RCC_interface.h"


/* this should be set to the current (actual) value of HCLK,
 * set by default to 8MHz (represents HSI) */
static volatile u32 CPU_CLOCK = 8000000;

void delay_setCPUclockFactor(void)
{
 	/*This local variable will be used to determine clock in use*/
 	u8 volatile Local_u8currentClock=0;
 	/*This Local variable will hold multiplier value in case of PLL*/
 	f32 volatile Local_f32Multiplier=1;
 	/*This Local Variable will hold clock speed, in case of HSE and HSI, it will be 8MHz (or the value found in RCC_interface.h generally)*/
 	u32 volatile Local_u32ClockSpeed= RCC_HSE_VALUE;

 	/*Check for current clock and store it in local variable*/
 	Local_u8currentClock = RCC_u8GetSWSStatus();
 	/*If current clock is PLL, check its multiplier settings. Otherwise, multiplier will be kept at 1*/
 	if (Local_u8currentClock == RCC_SWS_PLL)
 	{
 		/*Get multiplier settings through specific API in RCC register*/
 		Local_f32Multiplier = RCC_f32GetPLLMultiplierValue();
 		/*Calculate new speed*/
 		Local_u32ClockSpeed = Local_u32ClockSpeed * Local_f32Multiplier;
 	}


	/*Set CPU clock as the one calculated*/
 	CPU_CLOCK = Local_u32ClockSpeed;
}

void delay_ms(u32 time)
{
	volatile u32 c = 0;

	/* - time * 1000 to convert it to us
	 * - 189us is the time of: prologue + volatile counter initialization + const max counter value
	 * - 3.5us is the time of epilogue
	 * - 1.6248us is the time of each iteration
	 * - the result is normalized to 8MHz because all the calculations are based on 8MHz osc.,
	 *   hence the denominator is divided by 8MHz
	 * - CPU_CLOCK is the SysClk factor (linear mapping for any frequency 'x') */
	/*Check for current clock*/
	delay_setCPUclockFactor();
	const u32 ctr = (u32)((((f64)time * (f64)1000.0 - (f64)189.0 - (f64)3.5) * (f64)(CPU_CLOCK)) / ((f64)8000000.0 * (f64)1.6248));

	for (; c < ctr; c++)
	{

	}
}

void delay_us(u32 time)
{
	volatile u32 c = 0;
	/* - 189us is the time of: prologue + volatile counter initialization + const max counter value
	 * - 3.5us is the time of epilogue
	 * - 1.6248us is the time of each iteration
	 * - the result is normalized to 8MHz because all the calculations are based on 8MHz osc.,
	 *   hence the denominator is divided by 8MHz
         an equivalent amount of iterations for us resolution/time
	 * - CPU_CLOCK is the SysClk factor (linear mapping for any frequency 'x') */
	/*Check for current clock*/
	delay_setCPUclockFactor();
	const u32 ctr = (u32)((((f64)time- (f64)189.0 - (f64)3.5) * (f64)(CPU_CLOCK)) / ((f64)8000000.0 * (f64)1.6248));

	for (; c < ctr; c++)
	{

	}
}
