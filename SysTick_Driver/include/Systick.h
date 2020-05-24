/*
 * Systick.h
 *
 *  Created on: Mar 10, 2020
 *      Author: Mohamed Nafea
 */

/*	To set up the SysTick Timer, the recommended programming sequence is as follows:
		1• Disable SysTick by writing 0 to the SYSTICK Control and Status register.
		2• Write new reload value to the SYSTICK Reload Value register.
		3• Write to the SYSTICK Current Value register to clear the current value to 0.
		4• Write to the SYSTICK Control and Status register to start the SysTick timer.
 */
/*	Launching Sequence:
 * 		1. Systick_init
 * 		2. systick_settime
 * 		3. systick_SetCallback
 * 		4. systick_start
 * */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#define CLK_PRE_AHB				0U
#define CLK_PRE_AHB_DIV_8		1U

typedef void(*systickcbf_t)(void);

//void SysTick_Handler(void);

extern void SYSTICK_Init			(void)						;
extern void SYSTICK_Start			(void)						;
extern void SYSTICK_Stop			(void)						;
extern void SYSTICK_SetTime_ms		(u32 timems)				;
extern void SYSTICK_SetTime_us		(u32 timeus)				;
extern void SYSTICK_SetCallback		(systickcbf_t cbf)			;
extern void SYSTICK_Increment10Hz	(void)						;
extern void SYSTICK_Decrement10Hz	(void)						;


#endif /* SYSTICK_H_ */
