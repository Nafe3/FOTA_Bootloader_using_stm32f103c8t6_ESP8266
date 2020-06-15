/*
 * Flash.h
 *
 *  Created on: May 5, 2020
 *      Author: Mohamed Nafea
 */

#ifndef FLASH_H_
#define FLASH_H_

/*Main Flash Memory Pages*/
/*STM32F103C8 has 128K Flash memory, divided into 128 pages of 1K each*/
#define K								*0x400
#define FLASH_MEMORY_BASE_ADDRESS 		(u32)0x08000000
#define FLASH_MEMORY_PAGE_0				((u32)FLASH_MEMORY_BASE_ADDRESS+0x00000)
#define FLASH_MEMORY_PAGE_1             ((u32)FLASH_MEMORY_BASE_ADDRESS+0x00400)
#define FLASH_MEMORY_PAGE_2             ((u32)FLASH_MEMORY_BASE_ADDRESS+0x00800)
#define FLASH_MEMORY_PAGE_3             ((u32)FLASH_MEMORY_BASE_ADDRESS+0x00C00)
#define FLASH_MEMORY_PAGE_4             ((u32)FLASH_MEMORY_BASE_ADDRESS+0x01000)
#define FLASH_MEMORY_PAGE_5             ((u32)FLASH_MEMORY_BASE_ADDRESS+0x01400)
#define FLASH_MEMORY_PAGE_6             ((u32)FLASH_MEMORY_BASE_ADDRESS+0x01800)
#define FLASH_MEMORY_PAGE_7             ((u32)FLASH_MEMORY_BASE_ADDRESS+0x01C00)
#define FLASH_MEMORY_PAGE_8             ((u32)FLASH_MEMORY_BASE_ADDRESS+0x02000)
#define FLASH_MEMORY_PAGE_9             ((u32)FLASH_MEMORY_BASE_ADDRESS+0x02400)
#define FLASH_MEMORY_PAGE_10            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x02800)
#define FLASH_MEMORY_PAGE_11            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x02C00)
#define FLASH_MEMORY_PAGE_12            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x03000)
#define FLASH_MEMORY_PAGE_13            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x03400)
#define FLASH_MEMORY_PAGE_14            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x03800)
#define FLASH_MEMORY_PAGE_15            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x03C00)
#define FLASH_MEMORY_PAGE_16            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x04000)
#define FLASH_MEMORY_PAGE_17            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x04400)
#define FLASH_MEMORY_PAGE_18            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x04800)
#define FLASH_MEMORY_PAGE_19            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x04C00)
#define FLASH_MEMORY_PAGE_20            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x05000)
#define FLASH_MEMORY_PAGE_21            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x05400)
#define FLASH_MEMORY_PAGE_22            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x05800)
#define FLASH_MEMORY_PAGE_23            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x05C00)
#define FLASH_MEMORY_PAGE_24            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x06000)
#define FLASH_MEMORY_PAGE_25            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x06400)
#define FLASH_MEMORY_PAGE_26            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x06800)
#define FLASH_MEMORY_PAGE_27            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x06C00)
#define FLASH_MEMORY_PAGE_28            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x07000)
#define FLASH_MEMORY_PAGE_29            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x07400)
#define FLASH_MEMORY_PAGE_30            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x07800)
#define FLASH_MEMORY_PAGE_31            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x07C00)
#define FLASH_MEMORY_PAGE_32            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x08000)
#define FLASH_MEMORY_PAGE_33            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x08400)
#define FLASH_MEMORY_PAGE_34            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x08800)
#define FLASH_MEMORY_PAGE_35            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x08C00)
#define FLASH_MEMORY_PAGE_36            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x09000)
#define FLASH_MEMORY_PAGE_37            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x09400)
#define FLASH_MEMORY_PAGE_38            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x09800)
#define FLASH_MEMORY_PAGE_39            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x09C00)
#define FLASH_MEMORY_PAGE_40            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0A000)
#define FLASH_MEMORY_PAGE_41            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0A400)
#define FLASH_MEMORY_PAGE_42            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0A800)
#define FLASH_MEMORY_PAGE_43            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0AC00)
#define FLASH_MEMORY_PAGE_44            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0B000)
#define FLASH_MEMORY_PAGE_45            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0B400)
#define FLASH_MEMORY_PAGE_46            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0B800)
#define FLASH_MEMORY_PAGE_47            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0BC00)
#define FLASH_MEMORY_PAGE_48            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0C000)
#define FLASH_MEMORY_PAGE_49            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0C400)
#define FLASH_MEMORY_PAGE_50            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0C800)
#define FLASH_MEMORY_PAGE_51            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0CC00)
#define FLASH_MEMORY_PAGE_52            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0D000)
#define FLASH_MEMORY_PAGE_53            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0D400)
#define FLASH_MEMORY_PAGE_54            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0D800)
#define FLASH_MEMORY_PAGE_55            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0DC00)
#define FLASH_MEMORY_PAGE_56            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0E000)
#define FLASH_MEMORY_PAGE_57            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0E400)
#define FLASH_MEMORY_PAGE_58            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0E800)
#define FLASH_MEMORY_PAGE_59            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0EC00)
#define FLASH_MEMORY_PAGE_60            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0F000)
#define FLASH_MEMORY_PAGE_61            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0F400)
#define FLASH_MEMORY_PAGE_62            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0F800)
#define FLASH_MEMORY_PAGE_63            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x0FC00)
#define FLASH_MEMORY_PAGE_64            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x10000)
#define FLASH_MEMORY_PAGE_65            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x10400)
#define FLASH_MEMORY_PAGE_66            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x10800)
#define FLASH_MEMORY_PAGE_67            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x10C00)
#define FLASH_MEMORY_PAGE_68            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x11000)
#define FLASH_MEMORY_PAGE_69            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x11400)
#define FLASH_MEMORY_PAGE_70            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x11800)
#define FLASH_MEMORY_PAGE_71            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x11C00)
#define FLASH_MEMORY_PAGE_72            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x12000)
#define FLASH_MEMORY_PAGE_73            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x12400)
#define FLASH_MEMORY_PAGE_74            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x12800)
#define FLASH_MEMORY_PAGE_75            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x12C00)
#define FLASH_MEMORY_PAGE_76            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x13000)
#define FLASH_MEMORY_PAGE_77            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x13400)
#define FLASH_MEMORY_PAGE_78            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x13800)
#define FLASH_MEMORY_PAGE_79            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x13C00)
#define FLASH_MEMORY_PAGE_80            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x14000)
#define FLASH_MEMORY_PAGE_81            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x14400)
#define FLASH_MEMORY_PAGE_82            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x14800)
#define FLASH_MEMORY_PAGE_83            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x14C00)
#define FLASH_MEMORY_PAGE_84            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x15000)
#define FLASH_MEMORY_PAGE_85            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x15400)
#define FLASH_MEMORY_PAGE_86            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x15800)
#define FLASH_MEMORY_PAGE_87            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x15C00)
#define FLASH_MEMORY_PAGE_88            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x16000)
#define FLASH_MEMORY_PAGE_89            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x16400)
#define FLASH_MEMORY_PAGE_90            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x16800)
#define FLASH_MEMORY_PAGE_91            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x16C00)
#define FLASH_MEMORY_PAGE_92            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x17000)
#define FLASH_MEMORY_PAGE_93            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x17400)
#define FLASH_MEMORY_PAGE_94            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x17800)
#define FLASH_MEMORY_PAGE_95            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x17C00)
#define FLASH_MEMORY_PAGE_96            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x18000)
#define FLASH_MEMORY_PAGE_97            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x18400)
#define FLASH_MEMORY_PAGE_98            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x18800)
#define FLASH_MEMORY_PAGE_99            ((u32)FLASH_MEMORY_BASE_ADDRESS+0x18C00)
#define FLASH_MEMORY_PAGE_100           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x19000)
#define FLASH_MEMORY_PAGE_101           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x19400)
#define FLASH_MEMORY_PAGE_102           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x19800)
#define FLASH_MEMORY_PAGE_103           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x19C00)
#define FLASH_MEMORY_PAGE_104           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1A000)
#define FLASH_MEMORY_PAGE_105           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1A400)
#define FLASH_MEMORY_PAGE_106           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1A800)
#define FLASH_MEMORY_PAGE_107           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1AC00)
#define FLASH_MEMORY_PAGE_108           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1B000)
#define FLASH_MEMORY_PAGE_109           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1B400)
#define FLASH_MEMORY_PAGE_110           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1B800)
#define FLASH_MEMORY_PAGE_111           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1BC00)
#define FLASH_MEMORY_PAGE_112           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1C000)
#define FLASH_MEMORY_PAGE_113           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1C400)
#define FLASH_MEMORY_PAGE_114           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1C800)
#define FLASH_MEMORY_PAGE_115           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1CC00)
#define FLASH_MEMORY_PAGE_116           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1D000)
#define FLASH_MEMORY_PAGE_117           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1D400)
#define FLASH_MEMORY_PAGE_118           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1D800)
#define FLASH_MEMORY_PAGE_119           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1DC00)
#define FLASH_MEMORY_PAGE_120           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1E000)
#define FLASH_MEMORY_PAGE_121           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1E400)
#define FLASH_MEMORY_PAGE_122           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1E800)
#define FLASH_MEMORY_PAGE_123           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1EC00)
#define FLASH_MEMORY_PAGE_124           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1F000)
#define FLASH_MEMORY_PAGE_125           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1F400)
#define FLASH_MEMORY_PAGE_126           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1F800)
#define FLASH_MEMORY_PAGE_127           ((u32)FLASH_MEMORY_BASE_ADDRESS+0x1FC00)

/* Values to be used with WRP */
#define FLASH_WRProt_Pages0to3         ((u32)0x00000001)
#define FLASH_WRProt_Pages4to7         ((u32)0x00000002)
#define FLASH_WRProt_Pages8to11        ((u32)0x00000004)
#define FLASH_WRProt_Pages12to15       ((u32)0x00000008)
#define FLASH_WRProt_Pages16to19       ((u32)0x00000010)
#define FLASH_WRProt_Pages20to23       ((u32)0x00000020)
#define FLASH_WRProt_Pages24to27       ((u32)0x00000040)
#define FLASH_WRProt_Pages28to31       ((u32)0x00000080)
#define FLASH_WRProt_Pages32to35       ((u32)0x00000100)
#define FLASH_WRProt_Pages36to39       ((u32)0x00000200)
#define FLASH_WRProt_Pages40to43       ((u32)0x00000400)
#define FLASH_WRProt_Pages44to47       ((u32)0x00000800)
#define FLASH_WRProt_Pages48to51       ((u32)0x00001000)
#define FLASH_WRProt_Pages52to55       ((u32)0x00002000)
#define FLASH_WRProt_Pages56to59       ((u32)0x00004000)
#define FLASH_WRProt_Pages60to63       ((u32)0x00008000)
#define FLASH_WRProt_Pages64to67       ((u32)0x00010000)
#define FLASH_WRProt_Pages68to71       ((u32)0x00020000)
#define FLASH_WRProt_Pages72to75       ((u32)0x00040000)
#define FLASH_WRProt_Pages76to79       ((u32)0x00080000)
#define FLASH_WRProt_Pages80to83       ((u32)0x00100000)
#define FLASH_WRProt_Pages84to87       ((u32)0x00200000)
#define FLASH_WRProt_Pages88to91       ((u32)0x00400000)
#define FLASH_WRProt_Pages92to95       ((u32)0x00800000)
#define FLASH_WRProt_Pages96to99       ((u32)0x01000000)
#define FLASH_WRProt_Pages100to103     ((u32)0x02000000)
#define FLASH_WRProt_Pages104to107     ((u32)0x04000000)
#define FLASH_WRProt_Pages108to111     ((u32)0x08000000)
#define FLASH_WRProt_Pages112to115     ((u32)0x10000000)
#define FLASH_WRProt_Pages116to119     ((u32)0x20000000)
#define FLASH_WRProt_Pages120to123     ((u32)0x40000000)
#define FLASH_WRProt_Pages124to127     ((u32)0x80000000)


#define SAVE_FLASH						0U
#define SAVE_OPT						1U

extern ErrorStatus 	FLASH_Lock  	  					(void);
extern ErrorStatus 	FLASH_Unlock      					(void);

extern ErrorStatus 	FLASH_WriteWord   					(void* destAddress, u32 data);
extern ErrorStatus 	FLASH_WriteProgram					(void* srcAddress, void* destAddress, u32 numberOfBytes);

extern ErrorStatus 	FLASH_PageErase   					(u32 pageAddress);
extern ErrorStatus 	FLASH_MultiplePageErase   			(u32 pageAddress, u8 numberOfPages);
extern ErrorStatus 	FLASH_MassErase    					(void);

/*Flash Memory Protection Functions*/
extern ErrorStatus 	FLASH_OPT_Lock						(void);
extern ErrorStatus 	FLASH_OPT_Unlock					(void);

extern ErrorStatus  FLASH_OPT_EraseOPT					(void);

extern u8 			FLASH_OPT_GetRDPStatus				(void);
extern void		 	FLASH_OPT_ReadProtection_Enable		(void);
extern void 		FLASH_OPT_ReadProtection_Disable	(void);

extern u32 			FLASH_OPT_GetWRPStatus				(void);
extern void 		FLASH_OPT_WriteProtection_Enable	(u32 WRProt_mask);
extern void 		FLASH_OPT_WriteProtection_Disable	(u32 WRProt_mask);

/*System reset*/
extern void 	   	FLASH_SystemReset					(void);

/*Flash memory management*/
extern ErrorStatus	FLASH_savePage  					(u32  srcAddress,  u8 Flash_or_OPT);
extern ErrorStatus  FLASH_updatePage 					(u32* newData,    u16 numberOfBytes, u16 startIndex, u8 Flash_or_OPT);
extern ErrorStatus 	FLASH_reloadPage					(u32  destAddress, u8 Flash_or_OPT);

#endif /* FLASH_H_ */
