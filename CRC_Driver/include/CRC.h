/*
 * CRC.h
 *
 *  Created on: May 24, 2020
 *      Author: Mohamed Nafea
 */

typedef struct
{
  uint32_t DR;
  uint8_t  IDR;
  uint8_t   RESERVED0;
  uint16_t  RESERVED1;
  uint32_t CR;
} CRC_TypeDef;

/******************************************************************************/
/*                                                                            */
/*                          CRC calculation unit                              */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for CRC_DR register  *********************/
#define  CRC_DR_DR                           ((uint32_t)0xFFFFFFFF) /*!< Data register bits */


/*******************  Bit definition for CRC_IDR register  ********************/
#define  CRC_IDR_IDR                         ((uint8_t)0xFF)        /*!< General-purpose 8-bit data register bits */


/********************  Bit definition for CRC_CR register  ********************/
#define  CRC_CR_RESET                        ((uint8_t)0x01)        /*!< RESET bit */

void 		CRC_ResetDR(void);
uint32_t 	CRC_CalcCRC(uint32_t Data);
uint32_t 	CRC_CalcBlockCRC(uint32_t pBuffer[], uint32_t BufferLength);
uint32_t 	CRC_GetCRC(void);
void 		CRC_SetIDRegister(uint8_t IDValue);
uint8_t 	CRC_GetIDRegister(void);
