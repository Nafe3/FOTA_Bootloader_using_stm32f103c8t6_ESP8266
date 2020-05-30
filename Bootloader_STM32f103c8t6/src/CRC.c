/*
 * CRC.c
 *
 *  Created on: May 24, 2020
 *      Author: Mohamed Nafea
 */



#include "STD_TYPES.h"
#include "CRC.h"


#define CRC_BASE_ADDRESS 					((void*)0x40023000)

#define CRC									((CRC_TypeDef*)(CRC_BASE_ADDRESS))

/******************************************************************************/
/*                                                                            */
/*                          CRC calculation unit                              */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for CRC_DR register  *********************/
#define  CRC_DR_DR                           ((u32)0xFFFFFFFF) /*!< Data register bits */


/*******************  Bit definition for CRC_IDR register  ********************/
#define  CRC_IDR_IDR                         ((u8)0xFF)        /*!< General-purpose 8-bit data register bits */


/********************  Bit definition for CRC_CR register  ********************/
#define  CRC_CR_RESET                        ((u8)0x01)        /*!< RESET bit */



/**
  * @brief  Resets the CRC Data register (DR).
  * @param  None
  * @retval None
  */
void CRC_ResetDR(void)
{
  /* Reset CRC generator */
  CRC->CR = CRC_CR_RESET;

}

/**
  * @brief  Computes the 32-bit CRC of a given data word(32-bit).
  * @param  Data: data word(32-bit) to compute its CRC
  * @retval 32-bit CRC
  */
/*
u32 CRC_CalcCRC(u32 Data) //original
{
	CRC->DR = Data;

	return (CRC->DR);
}
*/
/*
u32 CRC_CalcCRC(u32 Data,u32* ret_val)//Hamdy
{
	//u32 volatile returnValue;
	CRC->DR = Data;
	//returnValue=(CRC->DR) & 0xFFFFFFFF;
	asm("mov r0, r3");
	*ret_val = CRC->DR;
  return (CRC->DR);
}*/
u32 CRC_CalcCRC(u32 Data/*,u32* ret_val*/)//Nafe3
{
	CRC->DR = Data;

	asm("mov r0, r3"); // moving the value of r3 into r0, since r3 contains our
					   // calculated value while the processor return the value in r0
	//*ret_val = CRC->DR;
  return (CRC->DR);
}

/**
  * @brief  Computes the 32-bit CRC of a given buffer of data word(32-bit).
  * @param  pBuffer: pointer to the buffer containing the data to be computed
  * @param  BufferLength: length of the buffer to be computed
  * @retval 32-bit CRC
  */
u32 CRC_CalcBlockCRC(u32 pBuffer[], u32 BufferLength)
{
  u32 index = 0;

  for(index = 0; index < BufferLength; index++)
  {
    CRC->DR = pBuffer[index];
  }
  asm("mov r0, r3");
  return (CRC->DR);
}

/**
  * @brief  Returns the current CRC value.
  * @param  None
  * @retval 32-bit CRC
  */
u32 CRC_GetCRC(void)
{
  return (CRC->DR);
}

/**
  * @brief  Stores a 8-bit data in the Independent Data(ID) register.
  * @param  IDValue: 8-bit value to be stored in the ID register
  * @retval None
  */
void CRC_SetIDRegister(u8 IDValue)
{
  CRC->IDR = IDValue;
}

/**
  * @brief  Returns the 8-bit data stored in the Independent Data(ID) register
  * @param  None
  * @retval 8-bit value of the ID register
  */
u8 CRC_GetIDRegister(void)
{
  return (CRC->IDR);
}
