/********************************/
/* Author		: Mohamed Nafea */
/* Version		: V01			*/
/* Release Date	: 25-1-2020		*/
/********************************/

#ifndef RCC_H_
#define RCC_H_

/*Macros for user input arguments*/
//bus
#define AHB 	1
#define	APB2 	2
#define APB1  	3
//clk
#define HSI		1
#define HSE		2
#define PLL		3
//PRE
#define USBPRE  1
#define ADCPRE  2
#define PPRE2   3
#define PPRE1   4
#define HPRE    5
//state
#define ON		1
#define OFF		0
//returns
#define OK		0
#define NOT_OK 	1

/*FROM STM32F10X.H*/
/******************************************************************************/
/*                                                                            */
/*                         Reset and Clock Control                            */
/*                                                                            */
/******************************************************************************/

/********************  Bit definition for RCC_CR register  ********************/
/*Note: Bit definitions are casted to u32 variables since they are masks used when dealing with operations on their registers*/
/*Their registers are the casted to pointer to u32 because they are memory addresses that we access(do read/write operations on to enable their functionalities*/
#define  RCC_CR_HSION                        ((u32)0x00000001)        /*!< Internal High Speed clock enable */
#define  RCC_CR_HSIRDY                       ((u32)0x00000002)        /*!< Internal High Speed clock ready flag */
#define  RCC_CR_HSITRIM                      ((u32)0x000000F8)        /*!< Internal High Speed clock trimming */
#define  RCC_CR_HSICAL                       ((u32)0x0000FF00)        /*!< Internal High Speed clock Calibration */
#define  RCC_CR_HSEON                        ((u32)0x00010000)        /*!< External High Speed clock enable */
#define  RCC_CR_HSERDY                       ((u32)0x00020000)        /*!< External High Speed clock ready flag */
#define  RCC_CR_HSEBYP                       ((u32)0x00040000)        /*!< External High Speed clock Bypass */
#define  RCC_CR_CSSON                        ((u32)0x00080000)        /*!< Clock Security System enable */
#define  RCC_CR_PLLON                        ((u32)0x01000000)        /*!< PLL enable */
#define  RCC_CR_PLLRDY                       ((u32)0x02000000)        /*!< PLL clock ready flag */

/*******************  Bit definition for RCC_CFGR register  *******************/
/*!< SW configuration */
#define  RCC_CFGR_SW                         ((u32)0x00000003)        /*!< SW[1:0] bits (System clock Switch) */
#define  RCC_CFGR_SW_0                       ((u32)0x00000001)        /*!< Bit 0 */
#define  RCC_CFGR_SW_1                       ((u32)0x00000002)        /*!< Bit 1 */

#define  RCC_CFGR_SW_HSI                     ((u32)0x00000000)        /*!< HSI selected as system clock */
#define  RCC_CFGR_SW_HSE                     ((u32)0x00000001)        /*!< HSE selected as system clock */
#define  RCC_CFGR_SW_PLL                     ((u32)0x00000002)        /*!< PLL selected as system clock */

/*!< SWS configuration */
#define  RCC_CFGR_SWS                        ((u32)0x0000000C)        /*!< SWS[1:0] bits (System Clock Switch Status) */
#define  RCC_CFGR_SWS_0                      ((u32)0x00000004)        /*!< Bit 0 */
#define  RCC_CFGR_SWS_1                      ((u32)0x00000008)        /*!< Bit 1 */

#define  RCC_CFGR_SWS_HSI                    ((u32)0x00000000)        /*!< HSI oscillator used as system clock */
#define  RCC_CFGR_SWS_HSE                    ((u32)0x00000004)        /*!< HSE oscillator used as system clock */
#define  RCC_CFGR_SWS_PLL                    ((u32)0x00000008)        /*!< PLL used as system clock */

/*!< HPRE configuration */
#define  RCC_CFGR_HPRE                       ((u32)0x000000F0)        /*!< HPRE[3:0] bits (AHB prescaler) */
#define  RCC_CFGR_HPRE_0                     ((u32)0x00000010)        /*!< Bit 0 */
#define  RCC_CFGR_HPRE_1                     ((u32)0x00000020)        /*!< Bit 1 */
#define  RCC_CFGR_HPRE_2                     ((u32)0x00000040)        /*!< Bit 2 */
#define  RCC_CFGR_HPRE_3                     ((u32)0x00000080)        /*!< Bit 3 */

#define  RCC_CFGR_HPRE_DIV1                  ((u32)0x00000000)        /*!< SYSCLK not divided */
#define  RCC_CFGR_HPRE_DIV2                  ((u32)0x00000080)        /*!< SYSCLK divided by 2 */
#define  RCC_CFGR_HPRE_DIV4                  ((u32)0x00000090)        /*!< SYSCLK divided by 4 */
#define  RCC_CFGR_HPRE_DIV8                  ((u32)0x000000A0)        /*!< SYSCLK divided by 8 */
#define  RCC_CFGR_HPRE_DIV16                 ((u32)0x000000B0)        /*!< SYSCLK divided by 16 */
#define  RCC_CFGR_HPRE_DIV64                 ((u32)0x000000C0)        /*!< SYSCLK divided by 64 */
#define  RCC_CFGR_HPRE_DIV128                ((u32)0x000000D0)        /*!< SYSCLK divided by 128 */
#define  RCC_CFGR_HPRE_DIV256                ((u32)0x000000E0)        /*!< SYSCLK divided by 256 */
#define  RCC_CFGR_HPRE_DIV512                ((u32)0x000000F0)        /*!< SYSCLK divided by 512 */

/*!< PPRE1 configuration */
#define  RCC_CFGR_PPRE1                      ((u32)0x00000700)        /*!< PRE1[2:0] bits (APB1 prescaler) */
#define  RCC_CFGR_PPRE1_0                    ((u32)0x00000100)        /*!< Bit 0 */
#define  RCC_CFGR_PPRE1_1                    ((u32)0x00000200)        /*!< Bit 1 */
#define  RCC_CFGR_PPRE1_2                    ((u32)0x00000400)        /*!< Bit 2 */

#define  RCC_CFGR_PPRE1_DIV1                 ((u32)0x00000000)        /*!< HCLK not divided */
#define  RCC_CFGR_PPRE1_DIV2                 ((u32)0x00000400)        /*!< HCLK divided by 2 */
#define  RCC_CFGR_PPRE1_DIV4                 ((u32)0x00000500)        /*!< HCLK divided by 4 */
#define  RCC_CFGR_PPRE1_DIV8                 ((u32)0x00000600)        /*!< HCLK divided by 8 */
#define  RCC_CFGR_PPRE1_DIV16                ((u32)0x00000700)        /*!< HCLK divided by 16 */

/*!< PPRE2 configuration */
#define  RCC_CFGR_PPRE2                      ((u32)0x00003800)        /*!< PRE2[2:0] bits (APB2 prescaler) */
#define  RCC_CFGR_PPRE2_0                    ((u32)0x00000800)        /*!< Bit 0 */
#define  RCC_CFGR_PPRE2_1                    ((u32)0x00001000)        /*!< Bit 1 */
#define  RCC_CFGR_PPRE2_2                    ((u32)0x00002000)        /*!< Bit 2 */

#define  RCC_CFGR_PPRE2_DIV1                 ((u32)0x00000000)        /*!< HCLK not divided */
#define  RCC_CFGR_PPRE2_DIV2                 ((u32)0x00002000)        /*!< HCLK divided by 2 */
#define  RCC_CFGR_PPRE2_DIV4                 ((u32)0x00002800)        /*!< HCLK divided by 4 */
#define  RCC_CFGR_PPRE2_DIV8                 ((u32)0x00003000)        /*!< HCLK divided by 8 */
#define  RCC_CFGR_PPRE2_DIV16                ((u32)0x00003800)        /*!< HCLK divided by 16 */

/*!< ADCPPRE configuration */
#define  RCC_CFGR_ADCPRE                     ((u32)0x0000C000)        /*!< ADCPRE[1:0] bits (ADC prescaler) */
#define  RCC_CFGR_ADCPRE_0                   ((u32)0x00004000)        /*!< Bit 0 */
#define  RCC_CFGR_ADCPRE_1                   ((u32)0x00008000)        /*!< Bit 1 */

#define  RCC_CFGR_ADCPRE_DIV2                ((u32)0x00000000)        /*!< PCLK2 divided by 2 */
#define  RCC_CFGR_ADCPRE_DIV4                ((u32)0x00004000)        /*!< PCLK2 divided by 4 */
#define  RCC_CFGR_ADCPRE_DIV6                ((u32)0x00008000)        /*!< PCLK2 divided by 6 */
#define  RCC_CFGR_ADCPRE_DIV8                ((u32)0x0000C000)        /*!< PCLK2 divided by 8 */

#define  RCC_CFGR_PLLSRC                     ((u32)0x00010000)        /*!< PLL entry clock source */

#define  RCC_CFGR_PLLXTPRE                   ((u32)0x00020000)        /*!< HSE divider for PLL entry */

/*!< PLLMUL configuration */
#define  RCC_CFGR_PLLMULL                    ((u32)0x003C0000)        /*!< PLLMUL[3:0] bits (PLL multiplication factor) */
#define  RCC_CFGR_PLLMULL_0                  ((u32)0x00040000)        /*!< Bit 0 */
#define  RCC_CFGR_PLLMULL_1                  ((u32)0x00080000)        /*!< Bit 1 */
#define  RCC_CFGR_PLLMULL_2                  ((u32)0x00100000)        /*!< Bit 2 */
#define  RCC_CFGR_PLLMULL_3                  ((u32)0x00200000)        /*!< Bit 3 */

 #define  RCC_CFGR_PLLSRC_HSI_Div2           ((u32)0x00000000)        /*!< HSI clock divided by 2 selected as PLL entry clock source */
 #define  RCC_CFGR_PLLSRC_HSE                ((u32)0x00010000)        /*!< HSE clock selected as PLL entry clock source */

 #define  RCC_CFGR_PLLXTPRE_HSE              ((u32)0x00000000)        /*!< HSE clock not divided for PLL entry */
 #define  RCC_CFGR_PLLXTPRE_HSE_Div2         ((u32)0x00020000)        /*!< HSE clock divided by 2 for PLL entry */

 #define  RCC_CFGR_PLLMULL2                  ((u32)0x00000000)        /*!< PLL input clock*2 */
 #define  RCC_CFGR_PLLMULL3                  ((u32)0x00040000)        /*!< PLL input clock*3 */
 #define  RCC_CFGR_PLLMULL4                  ((u32)0x00080000)        /*!< PLL input clock*4 */
 #define  RCC_CFGR_PLLMULL5                  ((u32)0x000C0000)        /*!< PLL input clock*5 */
 #define  RCC_CFGR_PLLMULL6                  ((u32)0x00100000)        /*!< PLL input clock*6 */
 #define  RCC_CFGR_PLLMULL7                  ((u32)0x00140000)        /*!< PLL input clock*7 */
 #define  RCC_CFGR_PLLMULL8                  ((u32)0x00180000)        /*!< PLL input clock*8 */
 #define  RCC_CFGR_PLLMULL9                  ((u32)0x001C0000)        /*!< PLL input clock*9 */
 #define  RCC_CFGR_PLLMULL10                 ((u32)0x00200000)        /*!< PLL input clock10 */
 #define  RCC_CFGR_PLLMULL11                 ((u32)0x00240000)        /*!< PLL input clock*11 */
 #define  RCC_CFGR_PLLMULL12                 ((u32)0x00280000)        /*!< PLL input clock*12 */
 #define  RCC_CFGR_PLLMULL13                 ((u32)0x002C0000)        /*!< PLL input clock*13 */
 #define  RCC_CFGR_PLLMULL14                 ((u32)0x00300000)        /*!< PLL input clock*14 */
 #define  RCC_CFGR_PLLMULL15                 ((u32)0x00340000)        /*!< PLL input clock*15 */
 #define  RCC_CFGR_PLLMULL16                 ((u32)0x00380000)        /*!< PLL input clock*16 */
 #define  RCC_CFGR_USBPRE                    ((u32)0x00400000)        /*!< USB Device prescaler */

/*!< MCO configuration */
 #define  RCC_CFGR_MCO                       ((u32)0x07000000)        /*!< MCO[2:0] bits (Microcontroller Clock Output) */
 #define  RCC_CFGR_MCO_0                     ((u32)0x01000000)        /*!< Bit 0 */
 #define  RCC_CFGR_MCO_1                     ((u32)0x02000000)        /*!< Bit 1 */
 #define  RCC_CFGR_MCO_2                     ((u32)0x04000000)        /*!< Bit 2 */

 #define  RCC_CFGR_MCO_NOCLOCK               ((u32)0x00000000)        /*!< No clock */
 #define  RCC_CFGR_MCO_SYSCLK                ((u32)0x04000000)        /*!< System clock selected as MCO source */
 #define  RCC_CFGR_MCO_HSI                   ((u32)0x05000000)        /*!< HSI clock selected as MCO source */
 #define  RCC_CFGR_MCO_HSE                   ((u32)0x06000000)        /*!< HSE clock selected as MCO source  */
 #define  RCC_CFGR_MCO_PLL                   ((u32)0x07000000)        /*!< PLL clock divided by 2 selected as MCO source */

/*!<******************  Bit definition for RCC_CIR register  ********************/
#define  RCC_CIR_LSIRDYF                     ((u32)0x00000001)        /*!< LSI Ready Interrupt flag */
#define  RCC_CIR_LSERDYF                     ((u32)0x00000002)        /*!< LSE Ready Interrupt flag */
#define  RCC_CIR_HSIRDYF                     ((u32)0x00000004)        /*!< HSI Ready Interrupt flag */
#define  RCC_CIR_HSERDYF                     ((u32)0x00000008)        /*!< HSE Ready Interrupt flag */
#define  RCC_CIR_PLLRDYF                     ((u32)0x00000010)        /*!< PLL Ready Interrupt flag */
#define  RCC_CIR_CSSF                        ((u32)0x00000080)        /*!< Clock Security System Interrupt flag */
#define  RCC_CIR_LSIRDYIE                    ((u32)0x00000100)        /*!< LSI Ready Interrupt Enable */
#define  RCC_CIR_LSERDYIE                    ((u32)0x00000200)        /*!< LSE Ready Interrupt Enable */
#define  RCC_CIR_HSIRDYIE                    ((u32)0x00000400)        /*!< HSI Ready Interrupt Enable */
#define  RCC_CIR_HSERDYIE                    ((u32)0x00000800)        /*!< HSE Ready Interrupt Enable */
#define  RCC_CIR_PLLRDYIE                    ((u32)0x00001000)        /*!< PLL Ready Interrupt Enable */
#define  RCC_CIR_LSIRDYC                     ((u32)0x00010000)        /*!< LSI Ready Interrupt Clear */
#define  RCC_CIR_LSERDYC                     ((u32)0x00020000)        /*!< LSE Ready Interrupt Clear */
#define  RCC_CIR_HSIRDYC                     ((u32)0x00040000)        /*!< HSI Ready Interrupt Clear */
#define  RCC_CIR_HSERDYC                     ((u32)0x00080000)        /*!< HSE Ready Interrupt Clear */
#define  RCC_CIR_PLLRDYC                     ((u32)0x00100000)        /*!< PLL Ready Interrupt Clear */
#define  RCC_CIR_CSSC                        ((u32)0x00800000)        /*!< Clock Security System Interrupt Clear */

/*****************  Bit definition for RCC_APB2RSTR register  *****************/
#define  RCC_APB2RSTR_AFIORST                ((u32)0x00000001)        /*!< Alternate Function I/O reset */
#define  RCC_APB2RSTR_IOPARST                ((u32)0x00000004)        /*!< I/O port A reset */
#define  RCC_APB2RSTR_IOPBRST                ((u32)0x00000008)        /*!< I/O port B reset */
#define  RCC_APB2RSTR_IOPCRST                ((u32)0x00000010)        /*!< I/O port C reset */
#define  RCC_APB2RSTR_IOPDRST                ((u32)0x00000020)        /*!< I/O port D reset */
#define  RCC_APB2RSTR_ADC1RST                ((u32)0x00000200)        /*!< ADC 1 interface reset */

#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
#define  RCC_APB2RSTR_ADC2RST                ((u32)0x00000400)        /*!< ADC 2 interface reset */
#endif

#define  RCC_APB2RSTR_TIM1RST                ((u32)0x00000800)        /*!< TIM1 Timer reset */
#define  RCC_APB2RSTR_SPI1RST                ((u32)0x00001000)        /*!< SPI 1 reset */
#define  RCC_APB2RSTR_USART1RST              ((u32)0x00004000)        /*!< USART1 reset */

#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
 #define  RCC_APB2RSTR_IOPERST               ((u32)0x00000040)        /*!< I/O port E reset */
#endif /* STM32F10X_LD && STM32F10X_LD_VL */

/*****************  Bit definition for RCC_APB1RSTR register  *****************/
#define  RCC_APB1RSTR_TIM2RST                ((u32)0x00000001)        /*!< Timer 2 reset */
#define  RCC_APB1RSTR_TIM3RST                ((u32)0x00000002)        /*!< Timer 3 reset */
#define  RCC_APB1RSTR_WWDGRST                ((u32)0x00000800)        /*!< Window Watchdog reset */
#define  RCC_APB1RSTR_USART2RST              ((u32)0x00020000)        /*!< USART 2 reset */
#define  RCC_APB1RSTR_I2C1RST                ((u32)0x00200000)        /*!< I2C 1 reset */

#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
#define  RCC_APB1RSTR_CAN1RST                ((u32)0x02000000)        /*!< CAN1 reset */
#endif

#define  RCC_APB1RSTR_BKPRST                 ((u32)0x08000000)        /*!< Backup interface reset */
#define  RCC_APB1RSTR_PWRRST                 ((u32)0x10000000)        /*!< Power interface reset */

#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
 #define  RCC_APB1RSTR_TIM4RST               ((u32)0x00000004)        /*!< Timer 4 reset */
 #define  RCC_APB1RSTR_SPI2RST               ((u32)0x00004000)        /*!< SPI 2 reset */
 #define  RCC_APB1RSTR_USART3RST             ((u32)0x00040000)        /*!< USART 3 reset */
 #define  RCC_APB1RSTR_I2C2RST               ((u32)0x00400000)        /*!< I2C 2 reset */
#endif /* STM32F10X_LD && STM32F10X_LD_VL */

#if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined (STM32F10X_LD) || defined  (STM32F10X_XL)
 #define  RCC_APB1RSTR_USBRST                ((u32)0x00800000)        /*!< USB Device reset */
#endif

/******************  Bit definition for RCC_AHBENR register  ******************/
#define  RCC_AHBENR_DMA1EN                   ((u16)0x0001)            /*!< DMA1 clock enable */
#define  RCC_AHBENR_SRAMEN                   ((u16)0x0004)            /*!< SRAM interface clock enable */
#define  RCC_AHBENR_FLITFEN                  ((u16)0x0010)            /*!< FLITF clock enable */
#define  RCC_AHBENR_CRCEN                    ((u16)0x0040)            /*!< CRC clock enable */

/******************  Bit definition for RCC_APB2ENR register  *****************/
#define  RCC_APB2ENR_AFIOEN                  ((u32)0x00000001)         /*!< Alternate Function I/O clock enable */
#define  RCC_APB2ENR_IOPAEN                  ((u32)0x00000004)         /*!< I/O port A clock enable */
#define  RCC_APB2ENR_IOPBEN                  ((u32)0x00000008)         /*!< I/O port B clock enable */
#define  RCC_APB2ENR_IOPCEN                  ((u32)0x00000010)         /*!< I/O port C clock enable */
#define  RCC_APB2ENR_IOPDEN                  ((u32)0x00000020)         /*!< I/O port D clock enable */
#define  RCC_APB2ENR_ADC1EN                  ((u32)0x00000200)         /*!< ADC 1 interface clock enable */

#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
#define  RCC_APB2ENR_ADC2EN                  ((u32)0x00000400)         /*!< ADC 2 interface clock enable */
#endif

#define  RCC_APB2ENR_TIM1EN                  ((u32)0x00000800)         /*!< TIM1 Timer clock enable */
#define  RCC_APB2ENR_SPI1EN                  ((u32)0x00001000)         /*!< SPI 1 clock enable */
#define  RCC_APB2ENR_USART1EN                ((u32)0x00004000)         /*!< USART1 clock enable */

#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
 #define  RCC_APB2ENR_IOPEEN                 ((u32)0x00000040)         /*!< I/O port E clock enable */
#endif /* STM32F10X_LD && STM32F10X_LD_VL */

/*****************  Bit definition for RCC_APB1ENR register  ******************/
#define  RCC_APB1ENR_TIM2EN                  ((u32)0x00000001)        /*!< Timer 2 clock enabled*/
#define  RCC_APB1ENR_TIM3EN                  ((u32)0x00000002)        /*!< Timer 3 clock enable */
#define  RCC_APB1ENR_WWDGEN                  ((u32)0x00000800)        /*!< Window Watchdog clock enable */
#define  RCC_APB1ENR_USART2EN                ((u32)0x00020000)        /*!< USART 2 clock enable */
#define  RCC_APB1ENR_I2C1EN                  ((u32)0x00200000)        /*!< I2C 1 clock enable */

#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
#define  RCC_APB1ENR_CAN1EN                  ((u32)0x02000000)        /*!< CAN1 clock enable */
#endif

#define  RCC_APB1ENR_BKPEN                   ((u32)0x08000000)        /*!< Backup interface clock enable */
#define  RCC_APB1ENR_PWREN                   ((u32)0x10000000)        /*!< Power interface clock enable */

#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
 #define  RCC_APB1ENR_TIM4EN                 ((u32)0x00000004)        /*!< Timer 4 clock enable */
 #define  RCC_APB1ENR_SPI2EN                 ((u32)0x00004000)        /*!< SPI 2 clock enable */
 #define  RCC_APB1ENR_USART3EN               ((u32)0x00040000)        /*!< USART 3 clock enable */
 #define  RCC_APB1ENR_I2C2EN                 ((u32)0x00400000)        /*!< I2C 2 clock enable */
#endif /* STM32F10X_LD && STM32F10X_LD_VL */

#if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined  (STM32F10X_LD)
 #define  RCC_APB1ENR_USBEN                  ((u32)0x00800000)        /*!< USB Device clock enable */
#endif

/*******************  Bit definition for RCC_BDCR register  *******************/
#define  RCC_BDCR_LSEON                      ((u32)0x00000001)        /*!< External Low Speed oscillator enable */
#define  RCC_BDCR_LSERDY                     ((u32)0x00000002)        /*!< External Low Speed oscillator Ready */
#define  RCC_BDCR_LSEBYP                     ((u32)0x00000004)        /*!< External Low Speed oscillator Bypass */

#define  RCC_BDCR_RTCSEL                     ((u32)0x00000300)        /*!< RTCSEL[1:0] bits (RTC clock source selection) */
#define  RCC_BDCR_RTCSEL_0                   ((u32)0x00000100)        /*!< Bit 0 */
#define  RCC_BDCR_RTCSEL_1                   ((u32)0x00000200)        /*!< Bit 1 */

/*!< RTC configuration */
#define  RCC_BDCR_RTCSEL_NOCLOCK             ((u32)0x00000000)        /*!< No clock */
#define  RCC_BDCR_RTCSEL_LSE                 ((u32)0x00000100)        /*!< LSE oscillator clock used as RTC clock */
#define  RCC_BDCR_RTCSEL_LSI                 ((u32)0x00000200)        /*!< LSI oscillator clock used as RTC clock */
#define  RCC_BDCR_RTCSEL_HSE                 ((u32)0x00000300)        /*!< HSE oscillator clock divided by 128 used as RTC clock */

#define  RCC_BDCR_RTCEN                      ((u32)0x00008000)        /*!< RTC clock enable */
#define  RCC_BDCR_BDRST                      ((u32)0x00010000)        /*!< Backup domain software reset  */

/*******************  Bit definition for RCC_CSR register  ********************/
#define  RCC_CSR_LSION                       ((u32)0x00000001)        /*!< Internal Low Speed oscillator enable */
#define  RCC_CSR_LSIRDY                      ((u32)0x00000002)        /*!< Internal Low Speed oscillator Ready */
#define  RCC_CSR_RMVF                        ((u32)0x01000000)        /*!< Remove reset flag */
#define  RCC_CSR_PINRSTF                     ((u32)0x04000000)        /*!< PIN reset flag */
#define  RCC_CSR_PORRSTF                     ((u32)0x08000000)        /*!< POR/PDR reset flag */
#define  RCC_CSR_SFTRSTF                     ((u32)0x10000000)        /*!< Software Reset flag */
#define  RCC_CSR_IWDGRSTF                    ((u32)0x20000000)        /*!< Independent Watchdog reset flag */
#define  RCC_CSR_WWDGRSTF                    ((u32)0x40000000)        /*!< Window watchdog reset flag */
#define  RCC_CSR_LPWRRSTF                    ((u32)0x80000000)        /*!< Low-Power reset flag */

/*Bit operations*/
/*
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define CLEAR_REG(REG)        ((REG) = (0x0))
#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))
*/
/*********************/
/*Function Prototypes*/
/*********************/

/*Description: user chooses the state(on/off) of a certain peripheral and certain bus*/
void RCC_voidEnablePeripheral(u8 bus,u32 peripheral,u8 state);
/*choose a clock to set as system clock*/
void RCC_voidSelectSysClk(u8 clk);
/*switch any clock on or off*/
void RCC_voidClkControl	 (u8 clk,u8 state);
/*configure PLL using its masks*/
void RCC_voidPLLConfig	 (u32 PLLMULmask,u32 PPLXTPREmask,u32 PLLSRCmask);
/*configure Prescalers using its masks*/
void RCC_voidPreConfig	 (u32 PREtype,u32 PREmask);
/*configure MCO using its masks*/
void RCC_voidMCOConfig	 (u32 MCOmask);


#endif
