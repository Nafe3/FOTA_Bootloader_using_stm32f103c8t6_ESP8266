/*
 * 1.  Enable/Disable interrupt-> ISER & ICER
 * 2.  Set/clear pending
 * 3.  Is active
 * 4.  Set priority
 * 5.  Get priority
 * 6.  Set group bits (SCB_AIRCR)
 * 7.  Enable/disable all interrupts (ASM)
 * 8.  Enable/disable all faults     (ASM)
 * 9.  Filter according to priority  (ASM)
 * 10. Generate Software Interrupt   (STIR)
 * */
/*NVIC registers*/
/*
 * NVIC_ISER  Interrupt set-enable register
 * NVIC_ICER  Interrupt clear-enable register
 * NVIC_ISPR  Interrupt set-pending register
 * NVIC_ICPR  Interrupt clear-pending register
 * NVIC_IABR  Interrupt active bit register
 * NVIC_IPR   Interrupt priority register
 * NVIC_STIR  Software trigger interrupt register
 * */
typedef  unsigned char           u8 ;
typedef  unsigned short int      u16;
typedef  unsigned long int       u32;

typedef  signed char             s8 ;
typedef  signed short int        s16;
typedef  signed long int         s32;

typedef  float                   f32;
typedef  double                  f64;
typedef  long double             f96;


#define  STD_ERR	            u8
#define  STD_TYPES_ERROR_OK     (STD_ERR)1U
#define  STD_TYPES_ERROR_NOK    (STD_ERR)2U

#define  STD_NULL 				((void*)0)

/*User options to use as functions' arguments*/
/*inum can be any positive number from 0 to 239*/

/*GROUPING OPTIONS:
 *Argument: PRIGROUP_MASK*/
#define GROUP_MASK_16 			0b011
#define GROUP_MASK_8 			0b100
#define GROUP_MASK_4 			0b101
#define GROUP_MASK_2 			0b110
#define GROUP_MASK_0 			0b111
/* GROUP_MASK_16
 *  ___________________________________________________________________________________________________________________
 * |	                    |	                       |                       |               |     |     |     |     |
 * |            Bit 7       |            Bit 6         |         Bit 5         |     Bit 4     |Bit 3|Bit 2|Bit 1|Bit 0|
 * |________________________|__________________________|_______________________|_______________|_____|_____|_____|_____|
 * |                 	 	                                                                   |                       |
 * |                                  Preempt(Group)Priorities                                 | NOT IMPLEMENTED       |
 * |                                  16 Group priorities                                      |                       |
 * |                                  Range-> 0:15                                             |                       |
 * |___________________________________________________________________________________________|_______________________|
 * */

/* GROUP_MASK_8
 *  ____________________________________________________________________________________________________________________
 * |	                    |	                       |                       |                |     |     |     |     |
 * |            Bit 7       |            Bit 6         |         Bit 5         |     Bit 4      |Bit 3|Bit 2|Bit 1|Bit 0|
 * |________________________|__________________________|_______________________|________________|_____|_____|_____|_____|
 * |                 	 	                                                   |                |                       |
 * |                                  Preempt(Group)Priorities                 |Sub-priorities  | NOT IMPLEMENTED       |
 * |                                  8  Group priorities                      |2 Sub-priorities|                       |
 * |                                  Range-> 0:7                              |Range-> 0:1     |                       |
 * |___________________________________________________________________________|________________|_______________________|
 * */

/* GROUP_MASK_4
 *  ____________________________________________________________________________________________________________________
 * |	                    |	                       |                       |                |     |     |     |     |
 * |            Bit 7       |            Bit 6         |         Bit 5         |     Bit 4      |Bit 3|Bit 2|Bit 1|Bit 0|
 * |________________________|__________________________|_______________________|________________|_____|_____|_____|_____|
 * |                 	 	                           |                                        |                       |
 * |          Preempt(Group)Priorities                 |             Sub-priorities             | NOT IMPLEMENTED       |
 * |          4  Group priorities                      |             4 Sub-priorities           |                       |
 * |          Range-> 0:3                              |             Range-> 0:3                |                       |
 * |___________________________________________________|________________________________________|_______________________|
 * */

/* GROUP_MASK_2
 *  ____________________________________________________________________________________________________________________
 * |	                    |	                       |                       |                |     |     |     |     |
 * |            Bit 7       |            Bit 6         |         Bit 5         |     Bit 4      |Bit 3|Bit 2|Bit 1|Bit 0|
 * |________________________|__________________________|_______________________|________________|_____|_____|_____|_____|
 * |                 	 	|                                                                   |                       |
 * |Preempt(Group)Priorities|                              Sub-priorities                       | NOT IMPLEMENTED       |
 * |2  Group priorities     |                              8 Sub-priorities                     |                       |
 * |Range-> 0:1             |                              Range-> 0:7                          |                       |
 * |________________________|___________________________________________________________________|_______________________|
 * */

/* GROUP_MASK_0
 *  ____________________________________________________________________________________________________________________
 * |	                    |	                       |                       |                |     |     |     |     |
 * |            Bit 7       |            Bit 6         |         Bit 5         |     Bit 4      |Bit 3|Bit 2|Bit 1|Bit 0|
 * |________________________|__________________________|_______________________|________________|_____|_____|_____|_____|
 * |                                                                                            |                       |
 * |                          Sub-priorities                                                    | NOT IMPLEMENTED       |
 * |                          16 Sub-priorities                                                 |                       |
 * |                          Range-> 0:15                                                      |                       |
 * |____________________________________________________________________________________________|_______________________|
 * */

STD_ERR NVIC_u8EnableEXTI 			(u8 inum)				;
STD_ERR NVIC_u8DisableEXTI			(u8 inum)				;
STD_ERR NVIC_u8SetPendFlag			(u8 inum)				;
STD_ERR NVIC_u8ClrPendFlag			(u8 inum)				;
STD_ERR NVIC_u8IsActive   			(u8 inum,u8* state)		;
STD_ERR NVIC_u8SetPriority			(u8 inum, u8  priority)	;
u8      NVIC_u8GetPriority			(u8 inum)				;
STD_ERR NVIC_u8SetGrpBits 			(u32 PRIGROUP_MASK)		;
void	NVIC_voidEnableAllEXTI		(void)					;
void    NVIC_voidDisableAllEXTI 	(void)					;
void    NVIC_voidEnableAllFaults	(void)					;
void    NVIC_voidDisableAllFaults	(void)					;
void    NVIC_voidFilterEXTI_lowerThan(u8 cancelStart_inum)	;
void    NVIC_voidCancelFilterEXTI   (void)					;
STD_ERR NVIC_u8GenerateSWI			(u8 inum)				;
