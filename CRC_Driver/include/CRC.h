/*
 * CRC.h
 *
 *  Created on: May 24, 2020
 *      Author: Mohamed Nafea
 */

typedef struct
{
  u32 DR;
  u8  IDR;
  u8  RESERVED0;
  u16 RESERVED1;
  u32 CR;
} CRC_TypeDef;


void 		CRC_ResetDR(void);
u32 		CRC_CalcCRC(u32 Data);
u32 		CRC_CalcBlockCRC(u32 pBuffer[], u32 BufferLength);
u32 		CRC_GetCRC(void);
void 		CRC_SetIDRegister(u8 IDValue);
u8 	        CRC_GetIDRegister(void);
