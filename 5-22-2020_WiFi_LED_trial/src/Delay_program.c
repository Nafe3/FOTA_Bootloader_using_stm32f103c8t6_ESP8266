/* libs */
#include "STD_TYPES.h"
/* own */
#include "Delay_interface.h"

/* this should be set to the current (actual) value of HCLK,
 * set by default to 8MHz (represents HSI) */
static volatile u32 CPU_CLOCK = 8000000;

void delay_setCPUclockFactor(u32 clk)
{
	CPU_CLOCK = clk;
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
	const u32 ctr = (u32)((((f64)time- (f64)189.0 - (f64)3.5) * (f64)(CPU_CLOCK)) / ((f64)8000000.0 * (f64)1.6248));

	for (; c < ctr; c++)
	{

	}
}
