#ifndef DELAY_INTERFACE_H_
#define DELAY_INTERFACE_H_

/* this should be called with the current (actual) value of HCLK
 * and NOT SysClk */
void delay_setCPUclockFactor(u32 clk);
void delay_ms(u32 time);
void delay_us(u32 time);

#endif /* DELAY_INTERFACE_H_ */
