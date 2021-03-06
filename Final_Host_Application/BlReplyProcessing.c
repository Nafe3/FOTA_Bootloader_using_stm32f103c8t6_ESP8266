/* This file implements the logic to read and process the replies from the Bootloader .
 * This file is common across win/linux/mac
 */

#include "main.h"
#include "WindowsSerialPort.h"

int read_bootloader_reply(uint8_t command_code, uint8_t* Copy_u8DataBuffer)
//Reads and processes the reply sent from the MCU bootloader
{
    #define TIMEOUT (uint16_t)10
    uint8_t ack[2]={0}; //MCU sends ack + len field
    uint32_t len_to_follow=0;
    int ret = -2;
    uint8_t whileFlag=1;
    uint16_t timeout_counter=0;

    //The MCU bootloader always sends ack/nack first . read that !!

    if (Copy_u8DataBuffer[0]==0xA5)
    {
        //CRC of last command was good .. received ACK and "len to follow"
        len_to_follow=Copy_u8DataBuffer[1];
        printf("\n\n   CRC : SUCCESS \r\n   BL reply Length : %d\n",len_to_follow);

        switch(0x50 | command_code)
        {
        case COMMAND_BL_GET_VER:
             process_COMMAND_BL_GET_VER(len_to_follow, Copy_u8DataBuffer);
            break;
        case COMMAND_BL_GET_HELP:
            process_COMMAND_BL_GET_HELP(len_to_follow, Copy_u8DataBuffer);
            break;
        case COMMAND_BL_GET_CID:
            process_COMMAND_BL_GET_CID(len_to_follow, Copy_u8DataBuffer);
            break;


        case COMMAND_BL_GO_TO_ADDR:
            process_COMMAND_BL_GO_TO_ADDR(len_to_follow, Copy_u8DataBuffer);
            break;


        case COMMAND_BL_FLASH_ERASE:
            process_COMMAND_BL_FLASH_ERASE(len_to_follow, Copy_u8DataBuffer);
            break;
        case COMMAND_BL_MASS_ERASE:
            process_COMMAND_BL_FLASH_MASS_ERASE(len_to_follow, Copy_u8DataBuffer);
            break;


        case COMMAND_BL_MEM_WRITE:
            process_COMMAND_BL_MEM_WRITE(len_to_follow, Copy_u8DataBuffer);
            break;
        case COMMAND_BL_MEM_READ:
            process_COMMAND_BL_MEM_READ(len_to_follow);
            break;


        case COMMAND_BL_EN_R_PROTECT:
            process_COMMAND_BL_EN_R_PROTECT(len_to_follow, Copy_u8DataBuffer);
            break;
        case COMMAND_BL_DIS_R_PROTECT:
            process_COMMAND_BL_DIS_R_PROTECT(len_to_follow, Copy_u8DataBuffer);
            break;
        case COMMAND_BL_EN_W_PROTECT:
            process_COMMAND_BL_EN_W_PROTECT(len_to_follow, Copy_u8DataBuffer);
            break;
        case COMMAND_BL_DIS_W_PROTECT:
            process_COMMAND_BL_DIS_W_PROTECT(len_to_follow, Copy_u8DataBuffer);
            break;
        case COMMAND_BL_GET_RDP_STATUS:
            process_COMMAND_BL_GET_RDP_STATUS(len_to_follow, Copy_u8DataBuffer);
            break;
        case COMMAND_BL_READ_SECTOR_P_STATUS:
            process_COMMAND_BL_READ_SECTOR_P_STATUS(len_to_follow, Copy_u8DataBuffer);
            break;


        case COMMAND_BL_MY_SYSTEM_RESET:
            process_COMMAND_BL_MY_SYSTEM_RESET(len_to_follow);
            break;
        case COMMAND_BL_EXISTING_APPS:
            process_COMMAND_BL_EXISTING_APPS(len_to_follow, Copy_u8DataBuffer);
            break;
        case COMMAND_BL_SAVE_APP_INFO:
            process_COMMAND_BL_SAVE_APP_INFO(len_to_follow, Copy_u8DataBuffer);
            break;
        //default:
            //printf("\n  Invalid command code\n");

        }

          ret = 0;
    }
    else if( ack[0] == 0x7F)
    {
        //CRC of last command was bad .. received NACK
        printf("\n   CRC: FAIL \n");
        ret= -1;
    }

    return ret;
}

void process_COMMAND_BL_GET_VER(uint32_t len, uint8_t* Copy_u8DataBuffer)
{
    uint8_t ver;
    printf("\n   Bootloader Ver. : 0x%x\n",Copy_u8DataBuffer[2]);
}

void process_COMMAND_BL_GET_HELP(uint32_t len, uint8_t* Copy_u8Buffer)
{
    //uint8_t reply[16];
    //read_serial_port(reply,len);
    printf("\n   Supported Commands :");
    for(uint32_t i =2 ; i < (len+2) ; i++)
    {
        printf("0x%x  ",Copy_u8Buffer[i]);
    }
    printf("\n");
}

void process_COMMAND_BL_GET_CID(uint32_t len, uint8_t* Copy_u8DataBuffer)
{
    uint8_t cid[4];

    uint16_t ci =0;
    uint16_t rev=0;
    //read_serial_port(cid,len);
    cid[0]= Copy_u8DataBuffer[2];
    cid[1]= Copy_u8DataBuffer[3];
    cid[2]= Copy_u8DataBuffer[4];
    cid[3]= Copy_u8DataBuffer[5];
    ci = (uint16_t)(cid[1] << 8 )+ cid[0];
    rev= (uint16_t)(cid[3] << 8 )+ cid[2];
    printf("\n   Chip Id.     : %#x\n",ci);
    //printf("\n   Revision Id. : %#x\n",rev);
    if(rev == 0 )printf("\n   Revision Id. : Revision A\n");
}



void process_COMMAND_BL_GO_TO_ADDR(uint32_t len, uint8_t* Copy_u8DataBuffer)
{
    uint8_t addr_status=0;
    //read_serial_port(&addr_status,len);
    /*Address status will be the first byte coming after the length, which is at index 2*/
    addr_status = Copy_u8DataBuffer[2];
    printf("\n   Address Status : 0x%x\n",addr_status);
    if(addr_status) printf("\n   Address Status : Address Invalid \r\n");
    else            printf("\n   Address Status : Address Valid \r\n");
}




void process_COMMAND_BL_FLASH_ERASE(uint32_t len, uint8_t* Copy_u8DataBuffer)
{
    uint8_t erase_status=Copy_u8DataBuffer[2];
    //read_serial_port(&erase_status,len);

    if(erase_status) printf("   Erase Status : Success!");
    else             printf("   Erase Status : Failed!");
}

void process_COMMAND_BL_FLASH_MASS_ERASE(uint32_t len_to_follow, uint8_t* Copy_u8DataBuffer)
{
    uint8_t erase_status=Copy_u8DataBuffer[2];
    //read_serial_port(&erase_status,len_to_follow);
    //printf("Erase Status = 0x%x\n",erase_status);
    printf("   Erase Status : Success!");


}



void process_COMMAND_BL_MEM_WRITE(uint32_t len, uint8_t* Copy_u8DataBuffer)
{
    uint8_t write_status=0;
    read_serial_port(&write_status,len);
    printf("   Write Status : 0x%x\n",write_status);
}

void process_COMMAND_BL_MEM_READ(uint32_t len)
{

}



void process_COMMAND_BL_EN_R_PROTECT(uint32_t len, uint8_t* Copy_u8DataBuffer)
{
    uint8_t status=Copy_u8DataBuffer[2];
    //read_serial_port(&status,len);
    printf("\n\n   Done!\n");
}

void process_COMMAND_BL_DIS_R_PROTECT(uint32_t len, uint8_t* Copy_u8DataBuffer)
{
    uint8_t status=Copy_u8DataBuffer[2];
    //read_serial_port(&status,len);
    printf("\n\n   Done!\n");
}

void process_COMMAND_BL_EN_W_PROTECT(uint32_t len, uint8_t* Copy_u8DataBuffer)
{
    uint8_t status=Copy_u8DataBuffer[2];
    //read_serial_port(&status,len);
    printf("\n\n   Done!Please reset microcontroller for changes to take change.\n");
}

void process_COMMAND_BL_DIS_W_PROTECT(uint32_t len, uint8_t* Copy_u8DataBuffer)
{
    uint8_t status=0;
    //read_serial_port(&status,len);
    printf("\n\n   Done!Please reset microcontroller for changes to take change.\n");
}



void process_COMMAND_BL_GET_RDP_STATUS(uint32_t len, uint8_t* Copy_u8DataBuffer)
{
    uint8_t rdp=Copy_u8DataBuffer[2];
    //read_serial_port(&rdp,len);
    printf("\n   RDP Status : 0x%X\r\n",rdp);
    if(rdp) printf("\n   RDP Status : Flash memory is read-protected\r\n");
    else printf("\n   RDP Status : Flash memory is NOT read-protected\r\n");
}

void process_COMMAND_BL_READ_SECTOR_P_STATUS(uint32_t len, uint8_t* Copy_u8DataBuffer)
{
    uint8_t index;
    uint8_t reply[10] = {0} ;
    uint8_t reply_final[5]  = {0} ;

    uint8_t  rdp  =0;
    uint32_t wrp  =0;

    //read_serial_port(reply, len);
    //char2hex(reply, reply_final, 5);

    //rdp  =           reply_final[0];
    //wrp  = *((uint32_t*)(reply_final+1));
    rdp  =           Copy_u8DataBuffer[2];
    wrp  = *((uint32_t*)(Copy_u8DataBuffer+3));

    if(rdp) printf("\n   RDP Status : Flash memory is read-protected\r\n");
    else    printf("\n   RDP Status : Flash memory is NOT read-protected\r\n");

    printf("   Flash memory write protection status:\r\n");
    printf("   wrp = %#x\n",wrp);
    for( index=0; index<32 ; index++)
	{
        if( wrp & (1u<<index) )
        {
            printf("   Page %d\t: not protected\r\n",index*4  );
            printf("   Page %d\t: not protected\r\n",index*4+1);
            printf("   Page %d\t: not protected\r\n",index*4+2);
            printf("   Page %d\t: not protected\r\n",index*4+3);
        }
        else
        {
            printf("   Page %d\t: protected\r\n",index*4  );
            printf("   Page %d\t: protected\r\n",index*4+1);
            printf("   Page %d\t: protected\r\n",index*4+2);
            printf("   Page %d\t: protected\r\n",index*4+3);
        }
	}
	printf("\n\n   Done!\n");
}

void process_COMMAND_BL_MY_SYSTEM_RESET			(uint32_t len)
{
     uint8_t status=0;
     //read_serial_port(&status,len);
     printf("\n\n   Done! Resetting system\n");
}

void process_COMMAND_BL_EXISTING_APPS			(uint32_t len, uint8_t* Copy_u8DataBuffer)
{
    uint8_t  i,j;
   // uint8_t  bl_reply[512]    = {0};
    uint8_t  number_of_apps   = len/16;
	uint32_t app_base_address = 0;
	uint32_t app_size_in_bytes= 0;
	uint8_t  app_name[8]      ={0};
   // read_serial_port(bl_reply,len);
    for(i=0;i<number_of_apps;i++)
    {
        app_base_address  = *((uint32_t*)(Copy_u8DataBuffer+2+(i*16)));
        app_size_in_bytes = *((uint32_t*)(Copy_u8DataBuffer+2+(i*16)+4));
        for(j=0;j<8;j++)app_name[j] =     Copy_u8DataBuffer[2+(i*16)+8+j];
        if(i==0){
        printf("\r\n--------  ---------  ----------------   ---------");
        printf("\r\napp no.| |app name| |app base address|  |app size|");
        printf("\r\n--------  ---------  ----------------   ---------");
        }
        printf("\r\n%d\t  %.8s   %#x\t         %d",i+1,app_name,app_base_address,app_size_in_bytes);

    }

}
void process_COMMAND_BL_SAVE_APP_INFO			(uint32_t len, uint8_t* Copy_u8DataBuffer)
{
     uint8_t status=0;
     //read_serial_port(&status,len);
     status = Copy_u8DataBuffer[2];
     printf("\n\n   Done!\n");
}
