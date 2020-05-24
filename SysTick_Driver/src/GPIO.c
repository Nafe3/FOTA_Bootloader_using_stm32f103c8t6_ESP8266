#include "STD_TYPES.h"
#include  "GPIO.h"

/******************************************************************************/
/*                                                                            */
/*                General Purpose and Alternate Function I/O                  */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for GPIO_CRL register  *******************/
#define  GPIO_CRL_MODE                       ((u32)0x33333333)        /*!< Port x mode bits */

#define  GPIO_CRL_MODE0                      ((u32)0x00000003)        /*!< MODE0[1:0] bits (Port x mode bits, pin 0) */
#define  GPIO_CRL_MODE0_0                    ((u32)0x00000001)        /*!< Bit 0 */
#define  GPIO_CRL_MODE0_1                    ((u32)0x00000002)        /*!< Bit 1 */

#define  GPIO_CRL_MODE1                      ((u32)0x00000030)        /*!< MODE1[1:0] bits (Port x mode bits, pin 1) */
#define  GPIO_CRL_MODE1_0                    ((u32)0x00000010)        /*!< Bit 0 */
#define  GPIO_CRL_MODE1_1                    ((u32)0x00000020)        /*!< Bit 1 */

#define  GPIO_CRL_MODE2                      ((u32)0x00000300)        /*!< MODE2[1:0] bits (Port x mode bits, pin 2) */
#define  GPIO_CRL_MODE2_0                    ((u32)0x00000100)        /*!< Bit 0 */
#define  GPIO_CRL_MODE2_1                    ((u32)0x00000200)        /*!< Bit 1 */

#define  GPIO_CRL_MODE3                      ((u32)0x00003000)        /*!< MODE3[1:0] bits (Port x mode bits, pin 3) */
#define  GPIO_CRL_MODE3_0                    ((u32)0x00001000)        /*!< Bit 0 */
#define  GPIO_CRL_MODE3_1                    ((u32)0x00002000)        /*!< Bit 1 */

#define  GPIO_CRL_MODE4                      ((u32)0x00030000)        /*!< MODE4[1:0] bits (Port x mode bits, pin 4) */
#define  GPIO_CRL_MODE4_0                    ((u32)0x00010000)        /*!< Bit 0 */
#define  GPIO_CRL_MODE4_1                    ((u32)0x00020000)        /*!< Bit 1 */

#define  GPIO_CRL_MODE5                      ((u32)0x00300000)        /*!< MODE5[1:0] bits (Port x mode bits, pin 5) */
#define  GPIO_CRL_MODE5_0                    ((u32)0x00100000)        /*!< Bit 0 */
#define  GPIO_CRL_MODE5_1                    ((u32)0x00200000)        /*!< Bit 1 */

#define  GPIO_CRL_MODE6                      ((u32)0x03000000)        /*!< MODE6[1:0] bits (Port x mode bits, pin 6) */
#define  GPIO_CRL_MODE6_0                    ((u32)0x01000000)        /*!< Bit 0 */
#define  GPIO_CRL_MODE6_1                    ((u32)0x02000000)        /*!< Bit 1 */

#define  GPIO_CRL_MODE7                      ((u32)0x30000000)        /*!< MODE7[1:0] bits (Port x mode bits, pin 7) */
#define  GPIO_CRL_MODE7_0                    ((u32)0x10000000)        /*!< Bit 0 */
#define  GPIO_CRL_MODE7_1                    ((u32)0x20000000)        /*!< Bit 1 */

#define  GPIO_CRL_CNF                        ((u32)0xCCCCCCCC)        /*!< Port x configuration bits */

#define  GPIO_CRL_CNF0                       ((u32)0x0000000C)        /*!< CNF0[1:0] bits (Port x configuration bits, pin 0) */
#define  GPIO_CRL_CNF0_0                     ((u32)0x00000004)        /*!< Bit 0 */
#define  GPIO_CRL_CNF0_1                     ((u32)0x00000008)        /*!< Bit 1 */

#define  GPIO_CRL_CNF1                       ((u32)0x000000C0)        /*!< CNF1[1:0] bits (Port x configuration bits, pin 1) */
#define  GPIO_CRL_CNF1_0                     ((u32)0x00000040)        /*!< Bit 0 */
#define  GPIO_CRL_CNF1_1                     ((u32)0x00000080)        /*!< Bit 1 */

#define  GPIO_CRL_CNF2                       ((u32)0x00000C00)        /*!< CNF2[1:0] bits (Port x configuration bits, pin 2) */
#define  GPIO_CRL_CNF2_0                     ((u32)0x00000400)        /*!< Bit 0 */
#define  GPIO_CRL_CNF2_1                     ((u32)0x00000800)        /*!< Bit 1 */

#define  GPIO_CRL_CNF3                       ((u32)0x0000C000)        /*!< CNF3[1:0] bits (Port x configuration bits, pin 3) */
#define  GPIO_CRL_CNF3_0                     ((u32)0x00004000)        /*!< Bit 0 */
#define  GPIO_CRL_CNF3_1                     ((u32)0x00008000)        /*!< Bit 1 */

#define  GPIO_CRL_CNF4                       ((u32)0x000C0000)        /*!< CNF4[1:0] bits (Port x configuration bits, pin 4) */
#define  GPIO_CRL_CNF4_0                     ((u32)0x00040000)        /*!< Bit 0 */
#define  GPIO_CRL_CNF4_1                     ((u32)0x00080000)        /*!< Bit 1 */

#define  GPIO_CRL_CNF5                       ((u32)0x00C00000)        /*!< CNF5[1:0] bits (Port x configuration bits, pin 5) */
#define  GPIO_CRL_CNF5_0                     ((u32)0x00400000)        /*!< Bit 0 */
#define  GPIO_CRL_CNF5_1                     ((u32)0x00800000)        /*!< Bit 1 */

#define  GPIO_CRL_CNF6                       ((u32)0x0C000000)        /*!< CNF6[1:0] bits (Port x configuration bits, pin 6) */
#define  GPIO_CRL_CNF6_0                     ((u32)0x04000000)        /*!< Bit 0 */
#define  GPIO_CRL_CNF6_1                     ((u32)0x08000000)        /*!< Bit 1 */

#define  GPIO_CRL_CNF7                       ((u32)0xC0000000)        /*!< CNF7[1:0] bits (Port x configuration bits, pin 7) */
#define  GPIO_CRL_CNF7_0                     ((u32)0x40000000)        /*!< Bit 0 */
#define  GPIO_CRL_CNF7_1                     ((u32)0x80000000)        /*!< Bit 1 */

/*******************  Bit definition for GPIO_CRH register  *******************/
#define  GPIO_CRH_MODE                       ((u32)0x33333333)        /*!< Port x mode bits */

#define  GPIO_CRH_MODE8                      ((u32)0x00000003)        /*!< MODE8[1:0] bits (Port x mode bits, pin 8) */
#define  GPIO_CRH_MODE8_0                    ((u32)0x00000001)        /*!< Bit 0 */
#define  GPIO_CRH_MODE8_1                    ((u32)0x00000002)        /*!< Bit 1 */

#define  GPIO_CRH_MODE9                      ((u32)0x00000030)        /*!< MODE9[1:0] bits (Port x mode bits, pin 9) */
#define  GPIO_CRH_MODE9_0                    ((u32)0x00000010)        /*!< Bit 0 */
#define  GPIO_CRH_MODE9_1                    ((u32)0x00000020)        /*!< Bit 1 */

#define  GPIO_CRH_MODE10                     ((u32)0x00000300)        /*!< MODE10[1:0] bits (Port x mode bits, pin 10) */
#define  GPIO_CRH_MODE10_0                   ((u32)0x00000100)        /*!< Bit 0 */
#define  GPIO_CRH_MODE10_1                   ((u32)0x00000200)        /*!< Bit 1 */

#define  GPIO_CRH_MODE11                     ((u32)0x00003000)        /*!< MODE11[1:0] bits (Port x mode bits, pin 11) */
#define  GPIO_CRH_MODE11_0                   ((u32)0x00001000)        /*!< Bit 0 */
#define  GPIO_CRH_MODE11_1                   ((u32)0x00002000)        /*!< Bit 1 */

#define  GPIO_CRH_MODE12                     ((u32)0x00030000)        /*!< MODE12[1:0] bits (Port x mode bits, pin 12) */
#define  GPIO_CRH_MODE12_0                   ((u32)0x00010000)        /*!< Bit 0 */
#define  GPIO_CRH_MODE12_1                   ((u32)0x00020000)        /*!< Bit 1 */

#define  GPIO_CRH_MODE13                     ((u32)0x00300000)        /*!< MODE13[1:0] bits (Port x mode bits, pin 13) */
#define  GPIO_CRH_MODE13_0                   ((u32)0x00100000)        /*!< Bit 0 */
#define  GPIO_CRH_MODE13_1                   ((u32)0x00200000)        /*!< Bit 1 */

#define  GPIO_CRH_MODE14                     ((u32)0x03000000)        /*!< MODE14[1:0] bits (Port x mode bits, pin 14) */
#define  GPIO_CRH_MODE14_0                   ((u32)0x01000000)        /*!< Bit 0 */
#define  GPIO_CRH_MODE14_1                   ((u32)0x02000000)        /*!< Bit 1 */

#define  GPIO_CRH_MODE15                     ((u32)0x30000000)        /*!< MODE15[1:0] bits (Port x mode bits, pin 15) */
#define  GPIO_CRH_MODE15_0                   ((u32)0x10000000)        /*!< Bit 0 */
#define  GPIO_CRH_MODE15_1                   ((u32)0x20000000)        /*!< Bit 1 */

#define  GPIO_CRH_CNF                        ((u32)0xCCCCCCCC)        /*!< Port x configuration bits */

#define  GPIO_CRH_CNF8                       ((u32)0x0000000C)        /*!< CNF8[1:0] bits (Port x configuration bits, pin 8) */
#define  GPIO_CRH_CNF8_0                     ((u32)0x00000004)        /*!< Bit 0 */
#define  GPIO_CRH_CNF8_1                     ((u32)0x00000008)        /*!< Bit 1 */

#define  GPIO_CRH_CNF9                       ((u32)0x000000C0)        /*!< CNF9[1:0] bits (Port x configuration bits, pin 9) */
#define  GPIO_CRH_CNF9_0                     ((u32)0x00000040)        /*!< Bit 0 */
#define  GPIO_CRH_CNF9_1                     ((u32)0x00000080)        /*!< Bit 1 */

#define  GPIO_CRH_CNF10                      ((u32)0x00000C00)        /*!< CNF10[1:0] bits (Port x configuration bits, pin 10) */
#define  GPIO_CRH_CNF10_0                    ((u32)0x00000400)        /*!< Bit 0 */
#define  GPIO_CRH_CNF10_1                    ((u32)0x00000800)        /*!< Bit 1 */

#define  GPIO_CRH_CNF11                      ((u32)0x0000C000)        /*!< CNF11[1:0] bits (Port x configuration bits, pin 11) */
#define  GPIO_CRH_CNF11_0                    ((u32)0x00004000)        /*!< Bit 0 */
#define  GPIO_CRH_CNF11_1                    ((u32)0x00008000)        /*!< Bit 1 */

#define  GPIO_CRH_CNF12                      ((u32)0x000C0000)        /*!< CNF12[1:0] bits (Port x configuration bits, pin 12) */
#define  GPIO_CRH_CNF12_0                    ((u32)0x00040000)        /*!< Bit 0 */
#define  GPIO_CRH_CNF12_1                    ((u32)0x00080000)        /*!< Bit 1 */

#define  GPIO_CRH_CNF13                      ((u32)0x00C00000)        /*!< CNF13[1:0] bits (Port x configuration bits, pin 13) */
#define  GPIO_CRH_CNF13_0                    ((u32)0x00400000)        /*!< Bit 0 */
#define  GPIO_CRH_CNF13_1                    ((u32)0x00800000)        /*!< Bit 1 */

#define  GPIO_CRH_CNF14                      ((u32)0x0C000000)        /*!< CNF14[1:0] bits (Port x configuration bits, pin 14) */
#define  GPIO_CRH_CNF14_0                    ((u32)0x04000000)        /*!< Bit 0 */
#define  GPIO_CRH_CNF14_1                    ((u32)0x08000000)        /*!< Bit 1 */

#define  GPIO_CRH_CNF15                      ((u32)0xC0000000)        /*!< CNF15[1:0] bits (Port x configuration bits, pin 15) */
#define  GPIO_CRH_CNF15_0                    ((u32)0x40000000)        /*!< Bit 0 */
#define  GPIO_CRH_CNF15_1                    ((u32)0x80000000)        /*!< Bit 1 */

/*!<******************  Bit definition for GPIO_IDR register  *******************/
#define GPIO_IDR_IDR0                        ((u16)0x0001)            /*!< Port input data, bit 0 */
#define GPIO_IDR_IDR1                        ((u16)0x0002)            /*!< Port input data, bit 1 */
#define GPIO_IDR_IDR2                        ((u16)0x0004)            /*!< Port input data, bit 2 */
#define GPIO_IDR_IDR3                        ((u16)0x0008)            /*!< Port input data, bit 3 */
#define GPIO_IDR_IDR4                        ((u16)0x0010)            /*!< Port input data, bit 4 */
#define GPIO_IDR_IDR5                        ((u16)0x0020)            /*!< Port input data, bit 5 */
#define GPIO_IDR_IDR6                        ((u16)0x0040)            /*!< Port input data, bit 6 */
#define GPIO_IDR_IDR7                        ((u16)0x0080)            /*!< Port input data, bit 7 */
#define GPIO_IDR_IDR8                        ((u16)0x0100)            /*!< Port input data, bit 8 */
#define GPIO_IDR_IDR9                        ((u16)0x0200)            /*!< Port input data, bit 9 */
#define GPIO_IDR_IDR10                       ((u16)0x0400)            /*!< Port input data, bit 10 */
#define GPIO_IDR_IDR11                       ((u16)0x0800)            /*!< Port input data, bit 11 */
#define GPIO_IDR_IDR12                       ((u16)0x1000)            /*!< Port input data, bit 12 */
#define GPIO_IDR_IDR13                       ((u16)0x2000)            /*!< Port input data, bit 13 */
#define GPIO_IDR_IDR14                       ((u16)0x4000)            /*!< Port input data, bit 14 */
#define GPIO_IDR_IDR15                       ((u16)0x8000)            /*!< Port input data, bit 15 */

/*******************  Bit definition for GPIO_ODR register  *******************/
#define GPIO_ODR_ODR0                        ((u16)0x0001)            /*!< Port output data, bit 0 */
#define GPIO_ODR_ODR1                        ((u16)0x0002)            /*!< Port output data, bit 1 */
#define GPIO_ODR_ODR2                        ((u16)0x0004)            /*!< Port output data, bit 2 */
#define GPIO_ODR_ODR3                        ((u16)0x0008)            /*!< Port output data, bit 3 */
#define GPIO_ODR_ODR4                        ((u16)0x0010)            /*!< Port output data, bit 4 */
#define GPIO_ODR_ODR5                        ((u16)0x0020)            /*!< Port output data, bit 5 */
#define GPIO_ODR_ODR6                        ((u16)0x0040)            /*!< Port output data, bit 6 */
#define GPIO_ODR_ODR7                        ((u16)0x0080)            /*!< Port output data, bit 7 */
#define GPIO_ODR_ODR8                        ((u16)0x0100)            /*!< Port output data, bit 8 */
#define GPIO_ODR_ODR9                        ((u16)0x0200)            /*!< Port output data, bit 9 */
#define GPIO_ODR_ODR10                       ((u16)0x0400)            /*!< Port output data, bit 10 */
#define GPIO_ODR_ODR11                       ((u16)0x0800)            /*!< Port output data, bit 11 */
#define GPIO_ODR_ODR12                       ((u16)0x1000)            /*!< Port output data, bit 12 */
#define GPIO_ODR_ODR13                       ((u16)0x2000)            /*!< Port output data, bit 13 */
#define GPIO_ODR_ODR14                       ((u16)0x4000)            /*!< Port output data, bit 14 */
#define GPIO_ODR_ODR15                       ((u16)0x8000)            /*!< Port output data, bit 15 */

/******************  Bit definition for GPIO_BSRR register  *******************/
#define GPIO_BSRR_BS0                        ((u32)0x00000001)        /*!< Port x Set bit 0 */
#define GPIO_BSRR_BS1                        ((u32)0x00000002)        /*!< Port x Set bit 1 */
#define GPIO_BSRR_BS2                        ((u32)0x00000004)        /*!< Port x Set bit 2 */
#define GPIO_BSRR_BS3                        ((u32)0x00000008)        /*!< Port x Set bit 3 */
#define GPIO_BSRR_BS4                        ((u32)0x00000010)        /*!< Port x Set bit 4 */
#define GPIO_BSRR_BS5                        ((u32)0x00000020)        /*!< Port x Set bit 5 */
#define GPIO_BSRR_BS6                        ((u32)0x00000040)        /*!< Port x Set bit 6 */
#define GPIO_BSRR_BS7                        ((u32)0x00000080)        /*!< Port x Set bit 7 */
#define GPIO_BSRR_BS8                        ((u32)0x00000100)        /*!< Port x Set bit 8 */
#define GPIO_BSRR_BS9                        ((u32)0x00000200)        /*!< Port x Set bit 9 */
#define GPIO_BSRR_BS10                       ((u32)0x00000400)        /*!< Port x Set bit 10 */
#define GPIO_BSRR_BS11                       ((u32)0x00000800)        /*!< Port x Set bit 11 */
#define GPIO_BSRR_BS12                       ((u32)0x00001000)        /*!< Port x Set bit 12 */
#define GPIO_BSRR_BS13                       ((u32)0x00002000)        /*!< Port x Set bit 13 */
#define GPIO_BSRR_BS14                       ((u32)0x00004000)        /*!< Port x Set bit 14 */
#define GPIO_BSRR_BS15                       ((u32)0x00008000)        /*!< Port x Set bit 15 */

#define GPIO_BSRR_BR0                        ((u32)0x00010000)        /*!< Port x Reset bit 0 */
#define GPIO_BSRR_BR1                        ((u32)0x00020000)        /*!< Port x Reset bit 1 */
#define GPIO_BSRR_BR2                        ((u32)0x00040000)        /*!< Port x Reset bit 2 */
#define GPIO_BSRR_BR3                        ((u32)0x00080000)        /*!< Port x Reset bit 3 */
#define GPIO_BSRR_BR4                        ((u32)0x00100000)        /*!< Port x Reset bit 4 */
#define GPIO_BSRR_BR5                        ((u32)0x00200000)        /*!< Port x Reset bit 5 */
#define GPIO_BSRR_BR6                        ((u32)0x00400000)        /*!< Port x Reset bit 6 */
#define GPIO_BSRR_BR7                        ((u32)0x00800000)        /*!< Port x Reset bit 7 */
#define GPIO_BSRR_BR8                        ((u32)0x01000000)        /*!< Port x Reset bit 8 */
#define GPIO_BSRR_BR9                        ((u32)0x02000000)        /*!< Port x Reset bit 9 */
#define GPIO_BSRR_BR10                       ((u32)0x04000000)        /*!< Port x Reset bit 10 */
#define GPIO_BSRR_BR11                       ((u32)0x08000000)        /*!< Port x Reset bit 11 */
#define GPIO_BSRR_BR12                       ((u32)0x10000000)        /*!< Port x Reset bit 12 */
#define GPIO_BSRR_BR13                       ((u32)0x20000000)        /*!< Port x Reset bit 13 */
#define GPIO_BSRR_BR14                       ((u32)0x40000000)        /*!< Port x Reset bit 14 */
#define GPIO_BSRR_BR15                       ((u32)0x80000000)        /*!< Port x Reset bit 15 */

/*******************  Bit definition for GPIO_BRR register  *******************/
#define GPIO_BRR_BR0                         ((u16)0x0001)            /*!< Port x Reset bit 0 */
#define GPIO_BRR_BR1                         ((u16)0x0002)            /*!< Port x Reset bit 1 */
#define GPIO_BRR_BR2                         ((u16)0x0004)            /*!< Port x Reset bit 2 */
#define GPIO_BRR_BR3                         ((u16)0x0008)            /*!< Port x Reset bit 3 */
#define GPIO_BRR_BR4                         ((u16)0x0010)            /*!< Port x Reset bit 4 */
#define GPIO_BRR_BR5                         ((u16)0x0020)            /*!< Port x Reset bit 5 */
#define GPIO_BRR_BR6                         ((u16)0x0040)            /*!< Port x Reset bit 6 */
#define GPIO_BRR_BR7                         ((u16)0x0080)            /*!< Port x Reset bit 7 */
#define GPIO_BRR_BR8                         ((u16)0x0100)            /*!< Port x Reset bit 8 */
#define GPIO_BRR_BR9                         ((u16)0x0200)            /*!< Port x Reset bit 9 */
#define GPIO_BRR_BR10                        ((u16)0x0400)            /*!< Port x Reset bit 10 */
#define GPIO_BRR_BR11                        ((u16)0x0800)            /*!< Port x Reset bit 11 */
#define GPIO_BRR_BR12                        ((u16)0x1000)            /*!< Port x Reset bit 12 */
#define GPIO_BRR_BR13                        ((u16)0x2000)            /*!< Port x Reset bit 13 */
#define GPIO_BRR_BR14                        ((u16)0x4000)            /*!< Port x Reset bit 14 */
#define GPIO_BRR_BR15                        ((u16)0x8000)            /*!< Port x Reset bit 15 */

/******************  Bit definition for GPIO_LCKR register  *******************/
#define GPIO_LCKR_LCK0                       ((u32)0x00000001)        /*!< Port x Lock bit 0 */
#define GPIO_LCKR_LCK1                       ((u32)0x00000002)        /*!< Port x Lock bit 1 */
#define GPIO_LCKR_LCK2                       ((u32)0x00000004)        /*!< Port x Lock bit 2 */
#define GPIO_LCKR_LCK3                       ((u32)0x00000008)        /*!< Port x Lock bit 3 */
#define GPIO_LCKR_LCK4                       ((u32)0x00000010)        /*!< Port x Lock bit 4 */
#define GPIO_LCKR_LCK5                       ((u32)0x00000020)        /*!< Port x Lock bit 5 */
#define GPIO_LCKR_LCK6                       ((u32)0x00000040)        /*!< Port x Lock bit 6 */
#define GPIO_LCKR_LCK7                       ((u32)0x00000080)        /*!< Port x Lock bit 7 */
#define GPIO_LCKR_LCK8                       ((u32)0x00000100)        /*!< Port x Lock bit 8 */
#define GPIO_LCKR_LCK9                       ((u32)0x00000200)        /*!< Port x Lock bit 9 */
#define GPIO_LCKR_LCK10                      ((u32)0x00000400)        /*!< Port x Lock bit 10 */
#define GPIO_LCKR_LCK11                      ((u32)0x00000800)        /*!< Port x Lock bit 11 */
#define GPIO_LCKR_LCK12                      ((u32)0x00001000)        /*!< Port x Lock bit 12 */
#define GPIO_LCKR_LCK13                      ((u32)0x00002000)        /*!< Port x Lock bit 13 */
#define GPIO_LCKR_LCK14                      ((u32)0x00004000)        /*!< Port x Lock bit 14 */
#define GPIO_LCKR_LCK15                      ((u32)0x00008000)        /*!< Port x Lock bit 15 */
#define GPIO_LCKR_LCKK                       ((u32)0x00010000)        /*!< Lock key */



typedef struct
{
	volatile u32 CRL;
	volatile u32 CRH;
	volatile u32 IDR;
	volatile u32 ODR;
	volatile u32 BSRR;
	volatile u32 BRR;
	volatile u32 LCKR;
}GPIO_t;

#define Clear_4bits			0b0000
//////////////////////////////////////////////////////////
std_err GPIO_Init         (u32* Port_Mask, GPIO_Pin_t* Pin){
	std_err err;
	err.error_status= STATUS_OK;

	GPIO_t* RegPtr = (GPIO_t*)Port_Mask;		//casting the input port base address to pointer to structure that holds the gpio peripheral registers
	u32 localTemp = 0;

	if     (((Pin->pin) >= (u16)0) && (Pin->pin <= 7))//CRL
	{
		localTemp 	= 	RegPtr->CRL;							//saving register data to a temporary variable
		localTemp 	&=	Clear_4bits << (Pin->pin*4);			//clear the 4 bits of the required mode
		localTemp 	|=  (Pin->mode|Pin->speed) << ((Pin->pin-1)*4); //setting the input mode to their matching bits in the register
		RegPtr->CRL = 	localTemp;								//updating the register with the new input
		RegPtr->CRL = 	localTemp;								//updating the register with the new input
		RegPtr->CRL = 	localTemp;								//updating the register with the new input
	}
	else if((Pin->pin >  7) && (Pin->pin <=15))//CRH
	{
		Pin->pin -= 8;
		localTemp 	= 	RegPtr->CRH;							//saving register data to a temporary variable
		localTemp 	&=	Clear_4bits << (Pin->pin*4);			//clear the 4 bits of the required mode
		localTemp 	=  (Pin->mode|Pin->speed) << ((Pin->pin)*4); //setting the input mode to their matching bits in the register
		RegPtr->CRH = 	localTemp;                              //updating the register with the new input
		Pin->pin += 8;
	}
	else err.error_status= STATUS_NOK;

	return err;
}

/////////////////////////////////////////////////////////ODR
std_err GPIO_Pin_Write    (u32* Port_Mask, GPIO_Pin_t* Pin,u8  State){
	std_err err;
	err.error_status= STATUS_OK;
	GPIO_t* RegPtr = (GPIO_t*) Port_Mask;//casting

	if(State == HIGH)		RegPtr->ODR |=  (1U<<(Pin->pin));
	else if(State == LOW) 	RegPtr->ODR	&=~ (1U<<(Pin->pin));
	else      				err.error_status= STATUS_NOK;

	return err;
}


/////////////////////////////////////////////////////////////
std_err GPIO_Pin_Read     (u32* Port_Mask, GPIO_Pin_t* Pin,u8* ReturnValue){
	std_err err;
	err.error_status= STATUS_OK;
	GPIO_t* RegPtr = (GPIO_t*) Port_Mask;//casting

	if(Pin->pin >= 0 && Pin->pin <= 15 )	*ReturnValue = (u8)(RegPtr->IDR & Pin->pin) >> Pin->pin;
	else err.error_status= STATUS_NOK;
	return err;
}

/////////////////////////////////////////////////////////////
std_err GPIO_Pin_FastSet  (u32* Port_Mask, GPIO_Pin_t* Pin){
std_err err;
err.error_status= STATUS_OK;

GPIO_t* RegPtr = (GPIO_t*) Port_Mask;
if(Pin->pin >=0 && Pin->pin <= 7) RegPtr->BSRR |= Pin->pin;
else err.error_status = STATUS_NOK;

return err;
}

//////////////////////////////////////////////////////////////
std_err GPIO_Pin_FastReset(u32* Port_Mask, GPIO_Pin_t* Pin){
	std_err err;
	err.error_status= STATUS_OK;

	GPIO_t* RegPtr = (GPIO_t*) Port_Mask;
	if(Pin->pin >=0 && Pin->pin <= 7) RegPtr->BRR |= Pin->pin;
	else err.error_status = STATUS_NOK;

	return err;
}

/////////////////////////////////////////////////////////////
std_err GPIO_Pin_Lock     (u32* Port_Mask, GPIO_Pin_t* Pin){
	std_err err;
	err.error_status= STATUS_OK;
	GPIO_t* RegPtr = (GPIO_t*) Port_Mask;

	volatile u32 LocalTemp   = RegPtr->LCKR;

	RegPtr->LCKR |= Pin->pin;

	/*Lock key sequence*/
	/*LOCK key writing sequence:
	Write 1
	Write 0
	Write 1
	Read 0
	Read 1 (this read is optional but confirms that the lock is active)
	Note: During the LOCK Key Writing sequence, the value of LCK[15:0] must not change.
	Any error in the lock sequence will abort the lock.
	 */

	LocalTemp 	  			|=   GPIO_LCKR_LCKK;
	RegPtr->LCKR  			 =   LocalTemp;		//Write 1
	LocalTemp 	 			&=~  GPIO_LCKR_LCKK;
	RegPtr->LCKR  			 =   LocalTemp;		//Write 0
	LocalTemp 	  			|=   GPIO_LCKR_LCKK;
	RegPtr->LCKR  			 =   LocalTemp;		//Write 1
	LocalTemp				 =	 RegPtr->LCKR ; //Read 0
	LocalTemp				 =	 RegPtr->LCKR ; //Read 1

	if(!(RegPtr->LCKR & Pin->pin)) err.error_status=STATUS_NOK;
	return err;
}
