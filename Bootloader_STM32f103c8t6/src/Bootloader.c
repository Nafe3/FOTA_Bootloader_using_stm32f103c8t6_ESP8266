/*
 * Bootloader.c
 *
 *  Created on: May 24, 2020
 *      Author: Mohamed Nafea
 */

#include "STD_TYPES.h"
#include "Bootloader.h"
#include "Trace.h"

extern void bootloader_voidUARTReadData (void)
{
	trace_printf("BL_DEBUG_MSG:Button is pressed .. going to BL mode\n\r");
}
extern void bootloader_voidJumpToUserApp(void)
{
	trace_printf("BL_DEBUG_MSG:Button is not pressed .. executing user app\n\r");
}

