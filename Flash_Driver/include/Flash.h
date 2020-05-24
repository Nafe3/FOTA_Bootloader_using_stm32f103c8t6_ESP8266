/*
 * Flash.h
 *
 *  Created on: May 5, 2020
 *      Author: moham
 */

#ifndef FLASH_H_
#define FLASH_H_

extern void Flash_voidLock  	  (void);
extern void Flash_voidUnlock      (void);

extern void Flash_voidWriteWord   (void* destAddress, u32 data);
extern void Flash_voidWriteProgram(void* srcAddress, void* destAddress, u8 numberOfBytes);

extern void Flash_voidePageErase   (u32 pageAddress);
extern void Flash_voidMassErase    (void);

#endif /* FLASH_H_ */
