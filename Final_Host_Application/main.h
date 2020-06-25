#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

//Bl commands prototypes
void decode_menu_command_code(uint32_t command_code);

//BL Reply Process prototypes
void process_COMMAND_BL_GET_VER					(uint32_t len, uint8_t* Copy_u8DataBuffer);
void process_COMMAND_BL_GET_HELP				(uint32_t len, uint8_t* Copy_u8DataBuffer);
void process_COMMAND_BL_GET_CID					(uint32_t len, uint8_t* Copy_u8DataBuffer);

void process_COMMAND_BL_GO_TO_ADDR				(uint32_t len, uint8_t* Copy_u8DataBuffer);

void process_COMMAND_BL_FLASH_ERASE				(uint32_t len, uint8_t* Copy_u8DataBuffer);
void process_COMMAND_BL_FLASH_MASS_ERASE		(uint32_t len, uint8_t* Copy_u8DataBuffer);

void process_COMMAND_BL_MEM_WRITE				(uint32_t len, uint8_t* Copy_u8DataBuffer);
void process_COMMAND_BL_MEM_READ				(uint32_t len);

void process_COMMAND_BL_EN_R_PROTECT			(uint32_t len, uint8_t* Copy_u8DataBuffer);
void process_COMMAND_BL_DIS_R_PROTECT			(uint32_t len, uint8_t* Copy_u8DataBuffer);
void process_COMMAND_BL_EN_W_PROTECT			(uint32_t len, uint8_t* Copy_u8DataBuffer);
void process_COMMAND_BL_DIS_W_PROTECT			(uint32_t len, uint8_t* Copy_u8DataBuffer);
void process_COMMAND_BL_GET_RDP_STATUS			(uint32_t len, uint8_t* Copy_u8DataBuffer);
void process_COMMAND_BL_READ_SECTOR_P_STATUS	(uint32_t len, uint8_t* Copy_u8DataBuffer);

void process_COMMAND_BL_MY_SYSTEM_RESET			(uint32_t len);
void process_COMMAND_BL_EXISTING_APPS			(uint32_t len, uint8_t* Copy_u8DataBuffer);
void process_COMMAND_BL_SAVE_APP_INFO			(uint32_t len, uint8_t* Copy_u8DataBuffer);

int read_bootloader_reply						(uint8_t command_code, uint8_t* Copy_u8DataBuffer);
//int check_flash_status						(void);

//utilities Prototypes
uint32_t get_crc		(uint8_t *buff, uint32_t len);
uint8_t  word_to_byte	(uint32_t addr, uint8_t index, uint8_t lowerfirst);
void hex2char           (uint8_t* inBuffer, uint8_t* outBuffer, uint16_t NumOfBytesToBeConverted);
void char2hex           (uint8_t* inBuffer, uint8_t* outBuffer, uint16_t NumOfBytesToBeConverted );

//file ops
void 		close_the_file	(void);
uint32_t 	read_the_file	(uint8_t *buffer, uint32_t len);
void 		open_the_file	(void);
uint32_t 	calc_file_len	(void);

//BL Commands
#define COMMAND_BL_GET_VER                  0x51
#define COMMAND_BL_GET_HELP                 0x52
#define COMMAND_BL_GET_CID                  0x53

#define COMMAND_BL_GO_TO_ADDR               0x55

#define COMMAND_BL_FLASH_ERASE              0x56
#define COMMAND_BL_MASS_ERASE               0x5B

#define COMMAND_BL_MEM_WRITE                0x57
#define COMMAND_BL_MEM_READ                 0x59

#define COMMAND_BL_EN_R_PROTECT				0x5F
#define COMMAND_BL_DIS_R_PROTECT			0x60
#define COMMAND_BL_EN_W_PROTECT             0x58
#define COMMAND_BL_DIS_W_PROTECT            0x5C
#define COMMAND_BL_GET_RDP_STATUS           0x54
#define COMMAND_BL_READ_SECTOR_P_STATUS     0x5A

#define COMMAND_BL_MY_SYSTEM_RESET          0x5D
#define COMMAND_BL_EXISTING_APPS            0x5E
#define COMMAND_BL_SAVE_APP_INFO			0x61

//len details of the command
#define COMMAND_BL_GET_VER_LEN				6
#define COMMAND_BL_GET_HELP_LEN				6
#define COMMAND_BL_GET_CID_LEN				6

#define COMMAND_BL_GO_TO_ADDR_LEN			10

#define COMMAND_BL_FLASH_ERASE_LEN			11       //8 //19
#define COMMAND_BL_MASS_ERASE_LEN			6       //8 //10

#define COMMAND_BL_MEM_WRITE_LEN(x)			(11+x+8)//(11+x+8)//(11+x+4) //(7+x+4)
#define COMMAND_BL_MEM_READ_LEN				11

#define COMMAND_BL_EN_R_PROTECT_LEN			6      //10
#define COMMAND_BL_DIS_R_PROTECT_LEN		6       //10
#define COMMAND_BL_EN_W_PROTECT_LEN			10      //18
#define COMMAND_BL_DIS_W_PROTECT_LEN		10      //18
#define COMMAND_BL_GET_RDP_STATUS_LEN		6
#define COMMAND_BL_READ_SECTOR_P_STATUS_LEN	6      //10

#define COMMAND_BL_MY_SYSTEM_RESET_LEN		6      //10
#define COMMAND_BL_EXISTING_APPS_LEN		6
#define COMMAND_BL_SAVE_APP_INFO_LEN        22//34//42

/* Values to be used with WRP */
#define FLASH_WRProt_AllPages          ((uint32_t)0xFFFFFFFF)
#define FLASH_WRProt_Pages0to3         ((uint32_t)0x00000001)
#define FLASH_WRProt_Pages4to7         ((uint32_t)0x00000002)
#define FLASH_WRProt_Pages8to11        ((uint32_t)0x00000004)
#define FLASH_WRProt_Pages12to15       ((uint32_t)0x00000008)
#define FLASH_WRProt_Pages16to19       ((uint32_t)0x00000010)
#define FLASH_WRProt_Pages20to23       ((uint32_t)0x00000020)
#define FLASH_WRProt_Pages24to27       ((uint32_t)0x00000040)
#define FLASH_WRProt_Pages28to31       ((uint32_t)0x00000080)
#define FLASH_WRProt_Pages32to35       ((uint32_t)0x00000100)
#define FLASH_WRProt_Pages36to39       ((uint32_t)0x00000200)
#define FLASH_WRProt_Pages40to43       ((uint32_t)0x00000400)
#define FLASH_WRProt_Pages44to47       ((uint32_t)0x00000800)
#define FLASH_WRProt_Pages48to51       ((uint32_t)0x00001000)
#define FLASH_WRProt_Pages52to55       ((uint32_t)0x00002000)
#define FLASH_WRProt_Pages56to59       ((uint32_t)0x00004000)
#define FLASH_WRProt_Pages60to63       ((uint32_t)0x00008000)
#define FLASH_WRProt_Pages64to67       ((uint32_t)0x00010000)
#define FLASH_WRProt_Pages68to71       ((uint32_t)0x00020000)
#define FLASH_WRProt_Pages72to75       ((uint32_t)0x00040000)
#define FLASH_WRProt_Pages76to79       ((uint32_t)0x00080000)
#define FLASH_WRProt_Pages80to83       ((uint32_t)0x00100000)
#define FLASH_WRProt_Pages84to87       ((uint32_t)0x00200000)
#define FLASH_WRProt_Pages88to91       ((uint32_t)0x00400000)
#define FLASH_WRProt_Pages92to95       ((uint32_t)0x00800000)
#define FLASH_WRProt_Pages96to99       ((uint32_t)0x01000000)
#define FLASH_WRProt_Pages100to103     ((uint32_t)0x02000000)
#define FLASH_WRProt_Pages104to107     ((uint32_t)0x04000000)
#define FLASH_WRProt_Pages108to111     ((uint32_t)0x08000000)
#define FLASH_WRProt_Pages112to115     ((uint32_t)0x10000000)
#define FLASH_WRProt_Pages116to119     ((uint32_t)0x20000000)
#define FLASH_WRProt_Pages120to123     ((uint32_t)0x40000000)
#define FLASH_WRProt_Pages124to127     ((uint32_t)0x80000000)


//These addresses are used for GO_TO_ADDR for testing purpose.
#define GO_TO_ADDR1 0x20001234
#define GO_TO_ADDR2 0x40804434
#define GO_TO_ADDR3 0x08001234
#define GO_TO_ADDR4 0x12801234
#define GO_TO_ADDR5 0X08008248

//This is the flash sector 2 base address where we have stored the user application
#define USE_APP_FLASH_BASE_ADDR 0X08008000

#define RESET_HANDLER_ADDR_OF_APP 0x080081D8

typedef union
{
    uint16_t flash_sector_status;
    struct
    {
        uint16_t sector0:2;
        uint16_t sector1:2;
        uint16_t sector2:2;
        uint16_t sector3:2;
        uint16_t sector4:2;
        uint16_t sector5:2;
        uint16_t sector6:2;
        uint16_t sector7:2;

    }sectors;

}t_sector_status;


#endif // MAIN_H_INCLUDED
