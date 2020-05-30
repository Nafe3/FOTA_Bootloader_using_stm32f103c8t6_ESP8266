/*RCC Registers Addresses*/
#define RCC_BASE_ADDRESS 	 0x40021000
#define RCC_CR				 *((volatile u32*)(RCC_BASE_ADDRESS + 0x00))/*Control Register*/
#define RCC_CFGR			 *((volatile u32*)(RCC_BASE_ADDRESS + 0X04))/*Clock Configuration Register*/
#define RCC_CIR              *((volatile u32*)(RCC_BASE_ADDRESS + 0X08))/*Clock Interrupt  Register*/
#define RCC_APB2RSTR         *((volatile u32*)(RCC_BASE_ADDRESS + 0X0C))/*APB2 Peripheral reset register*/
#define RCC_APB1RSTR         *((volatile u32*)(RCC_BASE_ADDRESS + 0X10))/*APB1 Peripheral reset register*/
#define RCC_AHBENR           *((volatile u32*)(RCC_BASE_ADDRESS + 0X14))/*AHB  peripheral clock enable register*/
#define RCC_APB2ENR          *((volatile u32*)(RCC_BASE_ADDRESS + 0X18))/*APB2 peripheral clock enable register*/
#define RCC_APB1ENR          *((volatile u32*)(RCC_BASE_ADDRESS + 0X1C))/*APB1 peripheral clock enable register*/
#define RCC_BDCR             *((volatile u32*)(RCC_BASE_ADDRESS + 0X20))/*Backup Domain control register*/
#define RCC_CSR              *((volatile u32*)(RCC_BASE_ADDRESS + 0X24))/*Control/status register*/
#define RCC_AHBRSTR			 *((volatile u32*)(RCC_BASE_ADDRESS + 0x28))/*AHB peripheral clock reset register*/
#define RCC_CFGR2			 *((volatile u32*)(RCC_BASE_ADDRESS + 0x2C))/*Clock configuration register2*/
