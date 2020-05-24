//Registers addresses
#include "NVIC.h"

#define NVIC_BASE_ADDRESS 		0xE000E100
#define NVIC_PTR				(void*)NVIC_BASE_ADDRESS

#define  SCB_BASE_ADDRESS       0xE000E008
#define  SCB_AIRCR              *((volatile u32*)(SCB_BASE_ADDRESS+0x0C))
#define  SCB_AIRCR_VECTKEY		0x05FA0000	/*SCB_AIRCR Password*/
#define  SCB_CCR				*((volatile u32*)(SCB_BASE_ADDRESS+0x14))
#define  SCB_CCR_USERSETMPEND   0x2

/*#define NVIC_CCR				0xE000ED14 /*Used to enable writing to STIR, FROM M3 Guide but different address
 	 	 	 	 	 	 	 	 	 	 	 but with different address	in STM-programming manual 	 */
											/*I think ST added SCB as a logical core peripheral, since M3 Guide
											 * never mentioned SCB*/




typedef struct
{
  volatile u32 ISER[8U];               /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register */
           u32 RESERVED0[24U];
  volatile u32 ICER[8U];               /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register */
           u32 RSERVED1[24U];
  volatile u32 ISPR[8U];               /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register */
           u32 RESERVED2[24U];
  volatile u32 ICPR[8U];               /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register */
           u32 RESERVED3[24U];
  volatile u32 IABR[8U];               /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register */
           u32 RESERVED4[56U];
  volatile u8  IPR[240U];               /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
  	  	   u32 RESERVED5[644U];
  volatile u32 STIR;                   /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register */
}NVIC_Type;



///////////////////////////////////////////////////////
STD_ERR NVIC_u8EnableEXTI(u8 inum)
{
	STD_ERR err_status = STD_TYPES_ERROR_OK;
	NVIC_Type* ptr = (NVIC_Type*)NVIC_PTR;
	if(inum > 239)
	{
		err_status = STD_TYPES_ERROR_NOK;
	}
	else
	{
		ptr->ICPR[inum>>5U]=( 1 << (inum & 0b11111) );/*Clear pending flag*/
		ptr->ISER[inum>>5U]=( 1 << (inum & 0b11111) );/*Enable Interrupt*/
	}
	return err_status;
}


/////////////////////////////////////////////////////////
STD_ERR NVIC_u8DisableEXTI(u8 inum)
{
		STD_ERR err_status = STD_TYPES_ERROR_OK;
		NVIC_Type* ptr = (NVIC_Type*)NVIC_PTR;
		if(inum > 239)
		{
			err_status = STD_TYPES_ERROR_NOK;
		}
		else
		{
			ptr->ICER[inum>>5U]=( 1 << (inum & 0b11111) );
		}
		return err_status;
}

////////////////////////////////////////////////////////////
STD_ERR NVIC_u8SetPendFlag(u8 inum)
{
		STD_ERR err_status = STD_TYPES_ERROR_OK;
		NVIC_Type* ptr = (NVIC_Type*)NVIC_PTR;
		if(inum > 239)
		{
			err_status = STD_TYPES_ERROR_NOK;
		}
		else
		{
			ptr->ISPR[inum>>5U]=( 1 << (inum & 0b11111) );
		}
		return err_status;
}

////////////////////////////////////////////////////////////
STD_ERR NVIC_u8ClrPendFlag(u8 inum)
{
		STD_ERR err_status = STD_TYPES_ERROR_OK;
		NVIC_Type* ptr = (NVIC_Type*)NVIC_PTR;
		if(inum > 239)
		{
			err_status = STD_TYPES_ERROR_NOK;
		}
		else
		{
			ptr->ICPR[inum>>5U]=( 1 << (inum & 0b11111) );
		}
		return err_status;
}

//////////////////////////////////////////////////////////////
STD_ERR NVIC_u8IsActive(u8 inum,u8* state)
{
		STD_ERR err_status = STD_TYPES_ERROR_OK;
		NVIC_Type* ptr = (NVIC_Type*)NVIC_PTR;
		if(inum > 239)
		{
			err_status = STD_TYPES_ERROR_NOK;
		}
		else
		{
			//       go to the required register
			//       right shift it with the value of the require bit position
			//		 ANDing it with one to get the state
			*state = ((ptr->IABR[inum>>5U] >> (1U << inum & 0b11111 ) )&1U);
		}
		return err_status;
}

//////////////////////////////////////////////////////////////////
STD_ERR NVIC_u8SetPriority(u8 inum, u8 priority)
{
	STD_ERR err_status = STD_TYPES_ERROR_OK;
	NVIC_Type* ptr = (NVIC_Type*)NVIC_PTR;
	u8 u8getGroup    = ((SCB_AIRCR>>8) & 0b111);//Checking which group bits are chosen

	if(u8getGroup  == GROUP_MASK_16)
	{
		if(priority >15) 	return STD_TYPES_ERROR_NOK;
		else 				ptr->IPR[ inum >> 3U ] = priority;
	}
	else if(u8getGroup  == GROUP_MASK_8)
	{
		if(priority >7) 	return STD_TYPES_ERROR_NOK;
		else 				ptr->IPR[ inum >> 3U ] = priority;
	}
	else if(u8getGroup  == GROUP_MASK_4)
	{
		if(priority >3) 	return STD_TYPES_ERROR_NOK;
		else 				ptr->IPR[ inum >> 3U ] = priority;
	}
	else if(u8getGroup  == GROUP_MASK_2)
	{
		if(priority >1) 	return STD_TYPES_ERROR_NOK;
		else 				ptr->IPR[ inum >> 3U ] = priority;
	}
	else if(u8getGroup  == GROUP_MASK_0)
	{
		ptr->IPR[ inum >> 3U ] = priority;
	}
	else
	{
		err_status = STD_TYPES_ERROR_NOK ;
	}
	return err_status;
}

///////////////////////////////////////////////////////////////////
u8 NVIC_u8GetPriority(u8 inum)
{
	NVIC_Type* ptr = (NVIC_Type*)NVIC_PTR;
	return ptr->IPR[inum>>3U] & 0xFF;
}

///////////////////////////////////////////////////////////////////
STD_ERR NVIC_u8SetGrpBits 		(u32 PRIGROUP_MASK)
{
	STD_ERR err_status = STD_TYPES_ERROR_OK;
	SCB_AIRCR = SCB_AIRCR_VECTKEY; /*Writing the password to enable writing PRIGROUP configurations*/

	SCB_AIRCR = (8 << PRIGROUP_MASK);

	return err_status;
}

///////////////////////////////////////////////////////////////////
void NVIC_voidEnableAllEXTI(void)
{
	asm ("CPSIE I");
}

///////////////////////////////////////////////////////////////////
void NVIC_voidDisableAllEXTI(void)
{
	asm ("CPSID I");
}

///////////////////////////////////////////////////////////////////
void NVIC_voidEnableAllFaults(void)
{
	asm ("CPSIE F");
}

///////////////////////////////////////////////////////////////////
void NVIC_voidDisableAllFaults(void)
{
	asm ("CPSID I");
}

///////////////////////////////////////////////////////////////////
void    NVIC_voidFilterEXTI_lowerThan(u8 cancelStart_inum)
{
	//asm("MOV R0, #0x60");
	asm("LSLS R0, R0, #4");
	asm("MSR BASEPRI, R0");
}
///////////////////////////////////////////////////////////////////
void    NVIC_voidCancelFilterEXTI    (void)
{
	asm ("MOV R0, #0x0")    ;
	asm ("MSR BASEPRI, R0") ; /*Turn off BASEPRI masking*/
}

///////////////////////////////////////////////////////////////////
STD_ERR NVIC_u8GenerateSWI		(u8 inum)
{
	STD_ERR err_status = STD_TYPES_ERROR_OK;
	NVIC_Type* ptr = (NVIC_Type*)NVIC_PTR;

	SCB_CCR = SCB_CCR_USERSETMPEND ;/*Enabling Software Interrupt Generation*/

	if(inum > 239)
	{
		err_status = STD_TYPES_ERROR_NOK;
	}
	else
	{
		ptr->STIR = inum;
	}

	return err_status;
}
