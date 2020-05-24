#ifndef HRCC_H_
#define HRCC_H_

typedef enum {
	hsi,
	hse,
	pll
}SystemClk_t;

/*This function shall return the current system clock*/
/*It should be used in other Handlers or in Application*/
u32 HRCC_getSystemClk(void);

#endif
