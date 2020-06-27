/* This file implements logic to decode the user input , prepare and send the bootloader command packet
 * over the serial port. This file is common across win/linux/mac
 */

#include <stdio.h>
#include <time.h>
#include "main.h"

#include "HTTP_interface.h"

//Flash memory pages addresses
#define FLASH_MEMORY_BASE_ADDRESS 		(uint32_t)0x08000000
#define FLASH_MEMORY_PAGE_0				((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x00000)
#define FLASH_MEMORY_PAGE_1             ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x00400)
#define FLASH_MEMORY_PAGE_2             ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x00800)
#define FLASH_MEMORY_PAGE_3             ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x00C00)
#define FLASH_MEMORY_PAGE_4             ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x01000)
#define FLASH_MEMORY_PAGE_5             ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x01400)
#define FLASH_MEMORY_PAGE_6             ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x01800)
#define FLASH_MEMORY_PAGE_7             ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x01C00)
#define FLASH_MEMORY_PAGE_8             ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x02000)
#define FLASH_MEMORY_PAGE_9             ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x02400)
#define FLASH_MEMORY_PAGE_10            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x02800)
#define FLASH_MEMORY_PAGE_11            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x02C00)
#define FLASH_MEMORY_PAGE_12            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x03000)
#define FLASH_MEMORY_PAGE_13            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x03400)
#define FLASH_MEMORY_PAGE_14            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x03800)
#define FLASH_MEMORY_PAGE_15            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x03C00)
#define FLASH_MEMORY_PAGE_16            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x04000)
#define FLASH_MEMORY_PAGE_17            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x04400)
#define FLASH_MEMORY_PAGE_18            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x04800)
#define FLASH_MEMORY_PAGE_19            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x04C00)
#define FLASH_MEMORY_PAGE_20            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x05000)
#define FLASH_MEMORY_PAGE_21            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x05400)
#define FLASH_MEMORY_PAGE_22            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x05800)
#define FLASH_MEMORY_PAGE_23            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x05C00)
#define FLASH_MEMORY_PAGE_24            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x06000)
#define FLASH_MEMORY_PAGE_25            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x06400)
#define FLASH_MEMORY_PAGE_26            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x06800)
#define FLASH_MEMORY_PAGE_27            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x06C00)
#define FLASH_MEMORY_PAGE_28            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x07000)
#define FLASH_MEMORY_PAGE_29            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x07400)
#define FLASH_MEMORY_PAGE_30            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x07800)
#define FLASH_MEMORY_PAGE_31            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x07C00)
#define FLASH_MEMORY_PAGE_32            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x08000)
#define FLASH_MEMORY_PAGE_33            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x08400)
#define FLASH_MEMORY_PAGE_34            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x08800)
#define FLASH_MEMORY_PAGE_35            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x08C00)
#define FLASH_MEMORY_PAGE_36            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x09000)
#define FLASH_MEMORY_PAGE_37            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x09400)
#define FLASH_MEMORY_PAGE_38            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x09800)
#define FLASH_MEMORY_PAGE_39            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x09C00)
#define FLASH_MEMORY_PAGE_40            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0A000)
#define FLASH_MEMORY_PAGE_41            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0A400)
#define FLASH_MEMORY_PAGE_42            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0A800)
#define FLASH_MEMORY_PAGE_43            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0AC00)
#define FLASH_MEMORY_PAGE_44            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0B000)
#define FLASH_MEMORY_PAGE_45            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0B400)
#define FLASH_MEMORY_PAGE_46            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0B800)
#define FLASH_MEMORY_PAGE_47            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0BC00)
#define FLASH_MEMORY_PAGE_48            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0C000)
#define FLASH_MEMORY_PAGE_49            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0C400)
#define FLASH_MEMORY_PAGE_50            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0C800)
#define FLASH_MEMORY_PAGE_51            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0CC00)
#define FLASH_MEMORY_PAGE_52            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0D000)
#define FLASH_MEMORY_PAGE_53            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0D400)
#define FLASH_MEMORY_PAGE_54            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0D800)
#define FLASH_MEMORY_PAGE_55            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0DC00)
#define FLASH_MEMORY_PAGE_56            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0E000)
#define FLASH_MEMORY_PAGE_57            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0E400)
#define FLASH_MEMORY_PAGE_58            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0E800)
#define FLASH_MEMORY_PAGE_59            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0EC00)
#define FLASH_MEMORY_PAGE_60            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0F000)
#define FLASH_MEMORY_PAGE_61            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0F400)
#define FLASH_MEMORY_PAGE_62            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0F800)
#define FLASH_MEMORY_PAGE_63            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x0FC00)
#define FLASH_MEMORY_PAGE_64            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x10000)
#define FLASH_MEMORY_PAGE_65            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x10400)
#define FLASH_MEMORY_PAGE_66            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x10800)
#define FLASH_MEMORY_PAGE_67            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x10C00)
#define FLASH_MEMORY_PAGE_68            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x11000)
#define FLASH_MEMORY_PAGE_69            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x11400)
#define FLASH_MEMORY_PAGE_70            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x11800)
#define FLASH_MEMORY_PAGE_71            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x11C00)
#define FLASH_MEMORY_PAGE_72            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x12000)
#define FLASH_MEMORY_PAGE_73            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x12400)
#define FLASH_MEMORY_PAGE_74            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x12800)
#define FLASH_MEMORY_PAGE_75            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x12C00)
#define FLASH_MEMORY_PAGE_76            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x13000)
#define FLASH_MEMORY_PAGE_77            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x13400)
#define FLASH_MEMORY_PAGE_78            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x13800)
#define FLASH_MEMORY_PAGE_79            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x13C00)
#define FLASH_MEMORY_PAGE_80            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x14000)
#define FLASH_MEMORY_PAGE_81            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x14400)
#define FLASH_MEMORY_PAGE_82            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x14800)
#define FLASH_MEMORY_PAGE_83            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x14C00)
#define FLASH_MEMORY_PAGE_84            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x15000)
#define FLASH_MEMORY_PAGE_85            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x15400)
#define FLASH_MEMORY_PAGE_86            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x15800)
#define FLASH_MEMORY_PAGE_87            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x15C00)
#define FLASH_MEMORY_PAGE_88            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x16000)
#define FLASH_MEMORY_PAGE_89            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x16400)
#define FLASH_MEMORY_PAGE_90            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x16800)
#define FLASH_MEMORY_PAGE_91            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x16C00)
#define FLASH_MEMORY_PAGE_92            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x17000)
#define FLASH_MEMORY_PAGE_93            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x17400)
#define FLASH_MEMORY_PAGE_94            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x17800)
#define FLASH_MEMORY_PAGE_95            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x17C00)
#define FLASH_MEMORY_PAGE_96            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x18000)
#define FLASH_MEMORY_PAGE_97            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x18400)
#define FLASH_MEMORY_PAGE_98            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x18800)
#define FLASH_MEMORY_PAGE_99            ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x18C00)
#define FLASH_MEMORY_PAGE_100           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x19000)
#define FLASH_MEMORY_PAGE_101           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x19400)
#define FLASH_MEMORY_PAGE_102           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x19800)
#define FLASH_MEMORY_PAGE_103           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x19C00)
#define FLASH_MEMORY_PAGE_104           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1A000)
#define FLASH_MEMORY_PAGE_105           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1A400)
#define FLASH_MEMORY_PAGE_106           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1A800)
#define FLASH_MEMORY_PAGE_107           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1AC00)
#define FLASH_MEMORY_PAGE_108           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1B000)
#define FLASH_MEMORY_PAGE_109           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1B400)
#define FLASH_MEMORY_PAGE_110           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1B800)
#define FLASH_MEMORY_PAGE_111           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1BC00)
#define FLASH_MEMORY_PAGE_112           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1C000)
#define FLASH_MEMORY_PAGE_113           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1C400)
#define FLASH_MEMORY_PAGE_114           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1C800)
#define FLASH_MEMORY_PAGE_115           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1CC00)
#define FLASH_MEMORY_PAGE_116           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1D000)
#define FLASH_MEMORY_PAGE_117           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1D400)
#define FLASH_MEMORY_PAGE_118           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1D800)
#define FLASH_MEMORY_PAGE_119           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1DC00)
#define FLASH_MEMORY_PAGE_120           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1E000)
#define FLASH_MEMORY_PAGE_121           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1E400)
#define FLASH_MEMORY_PAGE_122           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1E800)
#define FLASH_MEMORY_PAGE_123           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1EC00)
#define FLASH_MEMORY_PAGE_124           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1F000)
#define FLASH_MEMORY_PAGE_125           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1F400)
#define FLASH_MEMORY_PAGE_126           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1F800)
#define FLASH_MEMORY_PAGE_127           ((uint32_t)FLASH_MEMORY_BASE_ADDRESS+0x1FC00)

void delay(int number_of_seconds)
{

/* Converting time into milli_seconds */

/*int milli_seconds = 1000 * number_of_seconds;*/
/*Here function is reimplemented so that the time used will be in milliseconds directly*/
int milli_seconds = number_of_seconds;

/*Storing start time*/

clock_t start_time = clock();

// looping till required time is not achieved

while (clock() < start_time + milli_seconds) ;

}

/* convert (inBuffer) which has (char) elements of double the size of the (outBuffer)
 * merging every two bytes of the (inBuffer) into one byte of (outBuffer)
 * This is done as we had to receive every byte(Hex) as two bytes in their (ASCII) representation
 * for example : (inBuffer) has 1024 bytes
 * 				 (outBuffer) has 512 bytes
 * 				 so we are going to iterate for 512 times*/
void   char2hex(uint8_t* inBuffer, uint8_t* outBuffer, uint16_t NumOfBytesToBeConverted )
{
	uint16_t index;
	uint8_t  Local_u8inBuffer;
	uint8_t  Local_u8inBuffer1;
	for(index=0;index<NumOfBytesToBeConverted;index++)
	{
		Local_u8inBuffer =inBuffer[index*2];
		Local_u8inBuffer1=inBuffer[index*2+1];
		if(Local_u8inBuffer   > 0x60)Local_u8inBuffer  +=9;
		if(Local_u8inBuffer1  > 0x60)Local_u8inBuffer1 +=9;
		outBuffer[index]  = ( (Local_u8inBuffer<<4) | (Local_u8inBuffer1&0x0F) );
	}
}

void hex2char(uint8_t* inBuffer, uint8_t* outBuffer, uint16_t NumOfBytesToBeConverted)
{
    uint16_t index;

    for(index=0;index<NumOfBytesToBeConverted;index++)
    {
        outBuffer[index*2] = inBuffer[index] & 0xF0 ;
        outBuffer[index*2] = outBuffer[index*2] >> 4 ;

        if      ( outBuffer[index*2] >= 0x0 && outBuffer[index*2] <= 0x9 )
                  outBuffer[index*2] |= 0x30;
        else if ( outBuffer[index*2] >= 0xA && outBuffer[index*2] <= 0xF ){
                  outBuffer[index*2] |= 0x60;
                  outBuffer[index*2] -= 9;}

        outBuffer[index*2+1] = inBuffer[index] & 0x0F ;

        if      ( outBuffer[index*2+1] >= 0x0 && outBuffer[index*2+1] <= 0x9 )
                  outBuffer[index*2+1] |= 0x30;
        else if ( outBuffer[index*2+1] >= 0xA && outBuffer[index*2+1] <= 0xF ){
                  outBuffer[index*2+1] |= 0x60;
                  outBuffer[index*2+1] -= 9;}

    }
}

/*This iterator will be used to make an empty for loop as a delay*/
uint16_t iterator=0;

//Decode the Bootloader command selection by the user
void decode_menu_command_code(uint32_t command_code)
{
    /*This macro defines timeout value that will be used while receiving data*/
    #define TIMEOUT (uint16_t)100

    uint8_t data_buf[1100]={0};
    uint8_t save_512_from_bin[512];
    uint8_t emptyFrame=0;
    uint32_t crc32=0;
    int ret_value=0;
    uint16_t bl_reply_without_ack=0;

    /*Write protection commands*/
    uint8_t  choice;
    uint32_t wrprot_mask;

    /*This variable will hold the command after being converted to char*/
    uint8_t commandPacket_TxBuffer[255]={0};
    uint8_t replyFromBootloaderChar[2000]={0};
    uint8_t replyFromBootloaderHex[1000]={0};
    /*This variable will be used for timeout while receiving data*/
    uint16_t timeout_counter=0;

    /*This variable will be used as iterator in the next function*/
    uint16_t Local_u16Iterator=0;

    /*Flash erase variables*/
    volatile uint8_t  sector_num;
    volatile uint8_t  nsec;
    uint32_t sector_address;

    /*Save app info*/
    uint32_t app_base_address   = 0;
    uint32_t app_size_in_bytes  = 0;
    uint8_t  app_name[8]        = {0};
    int c;
    uint8_t index;

    switch(command_code)
    {
    case 0:
        printf("\n  Exiting...!");
        exit(0);

    case 1:
        printf("\n   Command == > BL_GET_VER");

        data_buf[0] = COMMAND_BL_GET_VER_LEN-1;
        data_buf[1] = COMMAND_BL_GET_VER;
        crc32       = get_crc(data_buf,COMMAND_BL_GET_VER_LEN-4);
        data_buf[2] = word_to_byte(crc32,1,0);
        data_buf[3] = word_to_byte(crc32,2,0);
        data_buf[4] = word_to_byte(crc32,3,0);
        data_buf[5] = word_to_byte(crc32,4,0);


        /*Convert buffer to char to be sent through WIFI*/
        hex2char(data_buf,commandPacket_TxBuffer,COMMAND_BL_GET_VER_LEN);
        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,COMMAND_BL_GET_VER_LEN*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);
        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert response from char to hex*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,COMMAND_BL_GET_VER_LEN);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;

            timeout_counter++;
        }
        //printf("Done receiving\n");
        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_GET_VER, replyFromBootloaderHex);

        break;

    case 2:
        printf("\n   Command == > BL_GET_HLP");

        data_buf[0] = COMMAND_BL_GET_HELP_LEN-1;
        data_buf[1] = COMMAND_BL_GET_HELP;
        crc32       = get_crc(data_buf,COMMAND_BL_GET_HELP_LEN-4);
        data_buf[2] = word_to_byte(crc32,1,1);
        data_buf[3] = word_to_byte(crc32,2,1);
        data_buf[4] = word_to_byte(crc32,3,1);
        data_buf[5] = word_to_byte(crc32,4,1);

         /*Convert buffer to char to be sent through WIFI*/
        hex2char(data_buf,commandPacket_TxBuffer,COMMAND_BL_GET_HELP_LEN);
        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,COMMAND_BL_GET_HELP_LEN*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);

        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
            //printf("waiting for response\n");
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_GET_HELP, replyFromBootloaderHex);
        break;

    case 3:
        printf("\n   Command == > BL_GET_CID");

        data_buf[0] = COMMAND_BL_GET_CID_LEN-1;
        data_buf[1] = COMMAND_BL_GET_CID;
        crc32       = get_crc(data_buf,COMMAND_BL_GET_CID_LEN-4);
        data_buf[2] = word_to_byte(crc32,1,1);
        data_buf[3] = word_to_byte(crc32,2,1);
        data_buf[4] = word_to_byte(crc32,3,1);
        data_buf[5] = word_to_byte(crc32,4,1);
         /*Convert buffer to char to be sent through WIFI*/
        hex2char(data_buf,commandPacket_TxBuffer,COMMAND_BL_GET_CID_LEN);
        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,COMMAND_BL_GET_CID_LEN*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);
        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
            //printf("waiting for response\n");
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_GET_CID, replyFromBootloaderHex);
        break;


    case 4:
        printf("\n   Command == > BL_GO_TO_ADDR");
        printf("\n\n   Enter Address here : ");
        uint8_t go_address[4];
        /*Get input from user as string*/
        scanf(" %x",&go_address);

        data_buf[0] = COMMAND_BL_GO_TO_ADDR_LEN-1;
        data_buf[1] = COMMAND_BL_GO_TO_ADDR;

       // hex2char(go_address, &data_buf[2], 4);
        /*This for loop will assign address into data buffer*/
        for (Local_u16Iterator=0; Local_u16Iterator<4;Local_u16Iterator++)
        {
            data_buf[2+Local_u16Iterator]=go_address[Local_u16Iterator];
        }
//        data_buf[2]=(uint8_t)go_address;
//        data_buf[3]=(uint8_t)go_address>>8;
//        data_buf[4]=(uint8_t)go_address>>16;
//        data_buf[5]=(uint8_t)go_address>>24;
        crc32       = get_crc(data_buf,COMMAND_BL_GO_TO_ADDR_LEN-4);
        data_buf[6] = word_to_byte(crc32,1,1);
        data_buf[7] = word_to_byte(crc32,2,1);
        data_buf[8] = word_to_byte(crc32,3,1);
        data_buf[9] = word_to_byte(crc32,4,1);
        /*Convert buffer to char*/
        hex2char(data_buf,commandPacket_TxBuffer,COMMAND_BL_GO_TO_ADDR_LEN);

        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,COMMAND_BL_GO_TO_ADDR_LEN*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);

        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex[0]!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
            //printf("waiting for response\n");
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_GO_TO_ADDR, replyFromBootloaderHex);
        break;

    case 5:
        printf("\n   Command == > BL_FLASH_ERASE");

        data_buf[0] = COMMAND_BL_FLASH_ERASE_LEN-1;
        data_buf[1] = COMMAND_BL_FLASH_ERASE;

        printf("\n   Enter number of sectors to be erased here : ");
        scanf("%d",&nsec);

        printf("\n   Enter sector number(0-127): ");
        scanf("%d",&sector_num );
        //printf("\n\n   %d\n ",sector_num);
             if(sector_num == 0  ) sector_address = FLASH_MEMORY_PAGE_0;
        else if(sector_num == 1  ) sector_address = FLASH_MEMORY_PAGE_1  ;
		else if(sector_num == 2  ) sector_address = FLASH_MEMORY_PAGE_2  ;
		else if(sector_num == 3  ) sector_address = FLASH_MEMORY_PAGE_3  ;
		else if(sector_num == 4  ) sector_address = FLASH_MEMORY_PAGE_4  ;
		else if(sector_num == 5  ) sector_address = FLASH_MEMORY_PAGE_5  ;
		else if(sector_num == 6  ) sector_address = FLASH_MEMORY_PAGE_6  ;
		else if(sector_num == 7  ) sector_address = FLASH_MEMORY_PAGE_7  ;
		else if(sector_num == 8  ) sector_address = FLASH_MEMORY_PAGE_8  ;
		else if(sector_num == 9  ) sector_address = FLASH_MEMORY_PAGE_9  ;
		else if(sector_num == 10 ) sector_address = FLASH_MEMORY_PAGE_10 ;
		else if(sector_num == 11 ) sector_address = FLASH_MEMORY_PAGE_11 ;
		else if(sector_num == 12 ) sector_address = FLASH_MEMORY_PAGE_12 ;
		else if(sector_num == 13 ) sector_address = FLASH_MEMORY_PAGE_13 ;
		else if(sector_num == 14 ) sector_address = FLASH_MEMORY_PAGE_14 ;
		else if(sector_num == 15 ) sector_address = FLASH_MEMORY_PAGE_15 ;
		else if(sector_num == 16 ) sector_address = FLASH_MEMORY_PAGE_16 ;
		else if(sector_num == 17 ) sector_address = FLASH_MEMORY_PAGE_17 ;
		else if(sector_num == 18 ) sector_address = FLASH_MEMORY_PAGE_18 ;
		else if(sector_num == 19 ) sector_address = FLASH_MEMORY_PAGE_19 ;
		else if(sector_num == 20 ) sector_address = FLASH_MEMORY_PAGE_20 ;
		else if(sector_num == 21 ) sector_address = FLASH_MEMORY_PAGE_21 ;
		else if(sector_num == 22 ) sector_address = FLASH_MEMORY_PAGE_22 ;
		else if(sector_num == 23 ) sector_address = FLASH_MEMORY_PAGE_23 ;
		else if(sector_num == 24 ) sector_address = FLASH_MEMORY_PAGE_24 ;
		else if(sector_num == 25 ) sector_address = FLASH_MEMORY_PAGE_25 ;
		else if(sector_num == 26 ) sector_address = FLASH_MEMORY_PAGE_26 ;
		else if(sector_num == 27 ) sector_address = FLASH_MEMORY_PAGE_27 ;
		else if(sector_num == 28 ) sector_address = FLASH_MEMORY_PAGE_28 ;
		else if(sector_num == 29 ) sector_address = FLASH_MEMORY_PAGE_29 ;
		else if(sector_num == 30 ) sector_address = FLASH_MEMORY_PAGE_30 ;
		else if(sector_num == 31 ) sector_address = FLASH_MEMORY_PAGE_31 ;
		else if(sector_num == 32 ) sector_address = FLASH_MEMORY_PAGE_32 ;
		else if(sector_num == 33 ) sector_address = FLASH_MEMORY_PAGE_33 ;
		else if(sector_num == 34 ) sector_address = FLASH_MEMORY_PAGE_34 ;
		else if(sector_num == 35 ) sector_address = FLASH_MEMORY_PAGE_35 ;
		else if(sector_num == 36 ) sector_address = FLASH_MEMORY_PAGE_36 ;
		else if(sector_num == 37 ) sector_address = FLASH_MEMORY_PAGE_37 ;
		else if(sector_num == 38 ) sector_address = FLASH_MEMORY_PAGE_38 ;
		else if(sector_num == 39 ) sector_address = FLASH_MEMORY_PAGE_39 ;
		else if(sector_num == 40 ) sector_address = FLASH_MEMORY_PAGE_40 ;
		else if(sector_num == 41 ) sector_address = FLASH_MEMORY_PAGE_41 ;
		else if(sector_num == 42 ) sector_address = FLASH_MEMORY_PAGE_42 ;
		else if(sector_num == 43 ) sector_address = FLASH_MEMORY_PAGE_43 ;
		else if(sector_num == 44 ) sector_address = FLASH_MEMORY_PAGE_44 ;
		else if(sector_num == 45 ) sector_address = FLASH_MEMORY_PAGE_45 ;
		else if(sector_num == 46 ) sector_address = FLASH_MEMORY_PAGE_46 ;
		else if(sector_num == 47 ) sector_address = FLASH_MEMORY_PAGE_47 ;
		else if(sector_num == 48 ) sector_address = FLASH_MEMORY_PAGE_48 ;
		else if(sector_num == 49 ) sector_address = FLASH_MEMORY_PAGE_49 ;
		else if(sector_num == 50 ) sector_address = FLASH_MEMORY_PAGE_50 ;
		else if(sector_num == 51 ) sector_address = FLASH_MEMORY_PAGE_51 ;
		else if(sector_num == 52 ) sector_address = FLASH_MEMORY_PAGE_52 ;
		else if(sector_num == 53 ) sector_address = FLASH_MEMORY_PAGE_53 ;
		else if(sector_num == 54 ) sector_address = FLASH_MEMORY_PAGE_54 ;
		else if(sector_num == 55 ) sector_address = FLASH_MEMORY_PAGE_55 ;
		else if(sector_num == 56 ) sector_address = FLASH_MEMORY_PAGE_56 ;
		else if(sector_num == 57 ) sector_address = FLASH_MEMORY_PAGE_57 ;
		else if(sector_num == 58 ) sector_address = FLASH_MEMORY_PAGE_58 ;
		else if(sector_num == 59 ) sector_address = FLASH_MEMORY_PAGE_59 ;
		else if(sector_num == 60 ) sector_address = FLASH_MEMORY_PAGE_60 ;
		else if(sector_num == 61 ) sector_address = FLASH_MEMORY_PAGE_61 ;
		else if(sector_num == 62 ) sector_address = FLASH_MEMORY_PAGE_62 ;
		else if(sector_num == 63 ) sector_address = FLASH_MEMORY_PAGE_63 ;
		else if(sector_num == 64 ) sector_address = FLASH_MEMORY_PAGE_64 ;
		else if(sector_num == 65 ) sector_address = FLASH_MEMORY_PAGE_65 ;
		else if(sector_num == 66 ) sector_address = FLASH_MEMORY_PAGE_66 ;
		else if(sector_num == 67 ) sector_address = FLASH_MEMORY_PAGE_67 ;
		else if(sector_num == 68 ) sector_address = FLASH_MEMORY_PAGE_68 ;
		else if(sector_num == 69 ) sector_address = FLASH_MEMORY_PAGE_69 ;
		else if(sector_num == 70 ) sector_address = FLASH_MEMORY_PAGE_70 ;
		else if(sector_num == 71 ) sector_address = FLASH_MEMORY_PAGE_71 ;
		else if(sector_num == 72 ) sector_address = FLASH_MEMORY_PAGE_72 ;
		else if(sector_num == 73 ) sector_address = FLASH_MEMORY_PAGE_73 ;
		else if(sector_num == 74 ) sector_address = FLASH_MEMORY_PAGE_74 ;
		else if(sector_num == 75 ) sector_address = FLASH_MEMORY_PAGE_75 ;
		else if(sector_num == 76 ) sector_address = FLASH_MEMORY_PAGE_76 ;
		else if(sector_num == 77 ) sector_address = FLASH_MEMORY_PAGE_77 ;
		else if(sector_num == 78 ) sector_address = FLASH_MEMORY_PAGE_78 ;
		else if(sector_num == 79 ) sector_address = FLASH_MEMORY_PAGE_79 ;
		else if(sector_num == 80 ) sector_address = FLASH_MEMORY_PAGE_80 ;
		else if(sector_num == 81 ) sector_address = FLASH_MEMORY_PAGE_81 ;
		else if(sector_num == 82 ) sector_address = FLASH_MEMORY_PAGE_82 ;
		else if(sector_num == 83 ) sector_address = FLASH_MEMORY_PAGE_83 ;
		else if(sector_num == 84 ) sector_address = FLASH_MEMORY_PAGE_84 ;
		else if(sector_num == 85 ) sector_address = FLASH_MEMORY_PAGE_85 ;
		else if(sector_num == 86 ) sector_address = FLASH_MEMORY_PAGE_86 ;
		else if(sector_num == 87 ) sector_address = FLASH_MEMORY_PAGE_87 ;
		else if(sector_num == 88 ) sector_address = FLASH_MEMORY_PAGE_88 ;
		else if(sector_num == 89 ) sector_address = FLASH_MEMORY_PAGE_89 ;
		else if(sector_num == 90 ) sector_address = FLASH_MEMORY_PAGE_90 ;
		else if(sector_num == 91 ) sector_address = FLASH_MEMORY_PAGE_91 ;
		else if(sector_num == 92 ) sector_address = FLASH_MEMORY_PAGE_92 ;
		else if(sector_num == 93 ) sector_address = FLASH_MEMORY_PAGE_93 ;
		else if(sector_num == 94 ) sector_address = FLASH_MEMORY_PAGE_94 ;
		else if(sector_num == 95 ) sector_address = FLASH_MEMORY_PAGE_95 ;
		else if(sector_num == 96 ) sector_address = FLASH_MEMORY_PAGE_96 ;
		else if(sector_num == 97 ) sector_address = FLASH_MEMORY_PAGE_97 ;
		else if(sector_num == 98 ) sector_address = FLASH_MEMORY_PAGE_98 ;
		else if(sector_num == 99 ) sector_address = FLASH_MEMORY_PAGE_99 ;
		else if(sector_num == 100) sector_address = FLASH_MEMORY_PAGE_100;
		else if(sector_num == 101) sector_address = FLASH_MEMORY_PAGE_101;
		else if(sector_num == 102) sector_address = FLASH_MEMORY_PAGE_102;
		else if(sector_num == 103) sector_address = FLASH_MEMORY_PAGE_103;
		else if(sector_num == 104) sector_address = FLASH_MEMORY_PAGE_104;
		else if(sector_num == 105) sector_address = FLASH_MEMORY_PAGE_105;
		else if(sector_num == 106) sector_address = FLASH_MEMORY_PAGE_106;
		else if(sector_num == 107) sector_address = FLASH_MEMORY_PAGE_107;
		else if(sector_num == 108) sector_address = FLASH_MEMORY_PAGE_108;
		else if(sector_num == 109) sector_address = FLASH_MEMORY_PAGE_109;
		else if(sector_num == 110) sector_address = FLASH_MEMORY_PAGE_110;
		else if(sector_num == 111) sector_address = FLASH_MEMORY_PAGE_111;
		else if(sector_num == 112) sector_address = FLASH_MEMORY_PAGE_112;
		else if(sector_num == 113) sector_address = FLASH_MEMORY_PAGE_113;
		else if(sector_num == 114) sector_address = FLASH_MEMORY_PAGE_114;
		else if(sector_num == 115) sector_address = FLASH_MEMORY_PAGE_115;
		else if(sector_num == 116) sector_address = FLASH_MEMORY_PAGE_116;
		else if(sector_num == 117) sector_address = FLASH_MEMORY_PAGE_117;
		else if(sector_num == 118) sector_address = FLASH_MEMORY_PAGE_118;
		else if(sector_num == 119) sector_address = FLASH_MEMORY_PAGE_119;
		else if(sector_num == 120) sector_address = FLASH_MEMORY_PAGE_120;
		else if(sector_num == 121) sector_address = FLASH_MEMORY_PAGE_121;
		else if(sector_num == 122) sector_address = FLASH_MEMORY_PAGE_122;
		else if(sector_num == 123) sector_address = FLASH_MEMORY_PAGE_123;
		else if(sector_num == 124) sector_address = FLASH_MEMORY_PAGE_124;
		else if(sector_num == 125) sector_address = FLASH_MEMORY_PAGE_125;
		else if(sector_num == 126) sector_address = FLASH_MEMORY_PAGE_126;
		else if(sector_num == 127) sector_address = FLASH_MEMORY_PAGE_127;
        else{printf("\nInvalid Sector number!!!\r\n");return;}



        //populate base mem address
        //hex2char(&sector_address, &data_buf[2], 4); //assigning the 32bits memory address to 8 bytes to be sent to BL
        /*Populate sector address inside data buffer*/
        data_buf[2]=sector_address;
        data_buf[3]=sector_address>>8;
        data_buf[4]=sector_address>>16;
        data_buf[5]=sector_address>>24;

        //data_buf[10]= nsec;
        data_buf[6]= nsec;
        //printf(" sector num : %d %d \n",sector_num,nsec);
        //crc32       = get_crc(data_buf,COMMAND_BL_FLASH_ERASE_LEN-8);
        crc32       = get_crc(data_buf,COMMAND_BL_FLASH_ERASE_LEN-4);
        data_buf[7] = word_to_byte(crc32,1,1);
        data_buf[8] = word_to_byte(crc32,2,1);
        data_buf[9] = word_to_byte(crc32,3,1);
        data_buf[10] = word_to_byte(crc32,4,1);

         /*Convert buffer to char*/
        hex2char(data_buf,commandPacket_TxBuffer,COMMAND_BL_FLASH_ERASE_LEN);

        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,COMMAND_BL_FLASH_ERASE_LEN*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);

        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex[0]!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
            //printf("waiting for response\n");
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_FLASH_ERASE, replyFromBootloaderHex);
        printf("\n\n   Erased %d sectors starting from sector %d \n",nsec,sector_num);
        break;

    case 6:
        printf("\n   Command == > BL_FLASH_MASS_ERASE\n");
        data_buf[0] = COMMAND_BL_MASS_ERASE_LEN-1;
        data_buf[1] = COMMAND_BL_MASS_ERASE;

        crc32       = get_crc(data_buf,COMMAND_BL_MASS_ERASE_LEN-4);
        data_buf[2] = word_to_byte(crc32,1,1);
        data_buf[3] = word_to_byte(crc32,2,1);
        data_buf[4] = word_to_byte(crc32,3,1);
        data_buf[5] = word_to_byte(crc32,4,1);
        //hex2char(&crc32, &data_buf[COMMAND_BL_MASS_ERASE_LEN-8], 4);
         /*Convert buffer to char*/
        hex2char(data_buf,commandPacket_TxBuffer,COMMAND_BL_MASS_ERASE_LEN);

        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,COMMAND_BL_MASS_ERASE_LEN*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);

        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex[0]!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
            //printf("waiting for response\n");
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_MASS_ERASE, replyFromBootloaderHex);

        printf("\n\n   Mass Erase Done!\n");

        break;

    case 7:
        printf("\n   Command == > BL_MEM_WRITE");
//        uint32_t bytes_remaining   = 0;
        uint32_t t_len_of_file     = 0;
//        uint32_t bytes_so_far_sent = 0;
//        uint32_t len_to_read       = 0;
        uint32_t base_mem_address  = 0;

        /*Length of packet to be sent
        /* 1 byte len + 1 byte command code + 4 byte mem base address+ 4 byte CRC=10
        */
        uint32_t mem_write_cmd_total_len = 14;
        /*Put command length in first variable*/
        data_buf[0] = mem_write_cmd_total_len-1;
        /*Put command code in index 1*/
        data_buf[1] = COMMAND_BL_MEM_WRITE;

//        //First get the total number of bytes in the .bin file.
        t_len_of_file = calc_file_len();
//
//        //keep opening the file
//        open_the_file();
//
//        bytes_remaining = t_len_of_file - bytes_so_far_sent;

        printf("\n\n   Enter the memory write address here : ");
        scanf(" %x",&base_mem_address);

        /*Place address inside data buffer array*/
        data_buf[2]=base_mem_address;
        data_buf[3]=base_mem_address>>8;
        data_buf[4]=base_mem_address>>16;
        data_buf[5]=base_mem_address>>24;

        data_buf[6]=t_len_of_file;
        data_buf[7]=t_len_of_file>>8;
        data_buf[8]=t_len_of_file>>16;
        data_buf[9]=t_len_of_file>>24;

        crc32       = get_crc(data_buf,mem_write_cmd_total_len-4);
        data_buf[10] = word_to_byte(crc32,1,1);
        data_buf[11] = word_to_byte(crc32,2,1);
        data_buf[12] = word_to_byte(crc32,3,1);
        data_buf[13] = word_to_byte(crc32,4,1);

        /*Convert buffer to char*/
        hex2char(data_buf,commandPacket_TxBuffer,mem_write_cmd_total_len);

        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,mem_write_cmd_total_len*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);

        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex[0]!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
            //printf("waiting for response\n");
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_MEM_WRITE, replyFromBootloaderHex);
        break;

//        while(bytes_remaining)
//        {
//
//            if(bytes_remaining >= 64)
//            {
//                len_to_read = 64;
//            }else
//            {
//                len_to_read = bytes_remaining;
//            }


//            printf("\n   base mem address = %#.8x\n",base_mem_address);
//
//            /**************************** Preparing the sending buffer ****************************/
//            /* 1 byte len + 1 byte command code + 4 byte mem base address
//             * 1 byte payload len + len_to_read is amount of bytes read from file + 4 byte CRC
//             */
//            //uint32_t mem_write_cmd_total_len = COMMAND_BL_MEM_WRITE_LEN(len_to_read*2);
//
//            //first field is "len_to_follow"
//
//
//            //populate base mem address
//            hex2char(&base_mem_address, &data_buf[2], 4); //assigning the 32bits memory address to 8 bytes to be sent to BL
//
//            //sending double size of payload since we are converting every byte into two bytes
//            data_buf[10] = (uint8_t)(len_to_read*2);
//
//            //get the bytes in to buffer by reading file
//            read_the_file(save_512_from_bin,len_to_read);            //reading 512 bytes from binary file into buffer
//            hex2char(save_512_from_bin,&data_buf[11],len_to_read);    //splitting every byte from the binary file into two bytes and saving them in the sending buffer
//
//            crc32       = get_crc(&data_buf[0],mem_write_cmd_total_len-8);
//            hex2char(&crc32, &data_buf[mem_write_cmd_total_len-8], 4);
//
//            /**************************** Sending data buffer to BL ****************************/
//            delay(20);
//            /*Send length of data to be sent*/
//            Write_to_serial_port(&data_buf[0],1);
//            /*Send an empty frame to empty DR and prevent data length being read again by bootloader as data*/
//            Write_to_serial_port(&emptyFrame,1);
//            /*Wait for bootloader to be ready to receive data*/
//            delay(20);
//            for (iterator=1; iterator<mem_write_cmd_total_len;iterator++)
//            {
//                Write_to_serial_port(&data_buf[iterator],1);
//                //delay (500);
//            }
//
//            /*This will be the last thing sent in the loop*/
//            /*We will send empty frame now to clear DR*/
//            Write_to_serial_port(&emptyFrame,1);
//
//            /**************************** Updating variables for the next loop ****************************/
//            //update base mem address for the next loop
//            base_mem_address += len_to_read;
//            bytes_so_far_sent+= len_to_read;
//            bytes_remaining   = t_len_of_file - bytes_so_far_sent;
//
//            printf("\n\n   Bytes sent so far= %d -- bytes_remaining:%d\n",bytes_so_far_sent,bytes_remaining);
//            /**************************** Receiving reply from BL ****************************/
            /*Get Response for bootloader*/
            //ret_value = read_bootloader_reply(data_buf[1]);
         //}


    case 8:
        printf("\n   Command == > BL_MEM_READ\n");
        printf("\n   This command is not supported");
        break;

    case 9:
        printf("\n   Command == > BL_EN_R_PROTECT");
        data_buf[0] = COMMAND_BL_EN_R_PROTECT_LEN-1;   //command length macro
        data_buf[1] = COMMAND_BL_EN_R_PROTECT; //command code macro

        crc32       = get_crc(data_buf,COMMAND_BL_EN_R_PROTECT_LEN-4);
        data_buf[2] = word_to_byte(crc32,1,1);
        data_buf[3] = word_to_byte(crc32,2,1);
        data_buf[4] = word_to_byte(crc32,3,1);
        data_buf[5] = word_to_byte(crc32,4,1);

         /*Convert buffer to char*/
        hex2char(data_buf,commandPacket_TxBuffer,COMMAND_BL_EN_R_PROTECT_LEN);

        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,COMMAND_BL_MASS_ERASE_LEN*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);

        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex[0]!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
            //printf("waiting for response\n");
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_EN_R_PROTECT, replyFromBootloaderHex);
        break;

    /*Disable Memory Read command*/
    case 10:
        printf("\n   Command == > BL_DIS_R_PROTECT");
        data_buf[0] = COMMAND_BL_DIS_R_PROTECT_LEN-1;   //command length macro
        data_buf[1] = COMMAND_BL_DIS_R_PROTECT; //command code macro

        crc32       = get_crc(data_buf,COMMAND_BL_DIS_R_PROTECT_LEN-4);
        data_buf[2] = word_to_byte(crc32,1,1);
        data_buf[3] = word_to_byte(crc32,2,1);
        data_buf[4] = word_to_byte(crc32,3,1);
        data_buf[5] = word_to_byte(crc32,4,1);
         /*Convert buffer to char*/
        hex2char(data_buf,commandPacket_TxBuffer,COMMAND_BL_DIS_R_PROTECT_LEN);

        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,COMMAND_BL_DIS_R_PROTECT_LEN*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);

        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex[0]!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
            //printf("waiting for response\n");
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_DIS_R_PROTECT, replyFromBootloaderHex);

        break;
        /*End of disable memory read command*/

    /*Enable write protection command*/
    case 11:


        printf("\n   Command == > BL_EN_W_PROTECT");

        data_buf[0] = COMMAND_BL_EN_W_PROTECT_LEN-1;    //command length macro
        data_buf[1] = COMMAND_BL_EN_W_PROTECT;          //command code macro

        printf("\n   Flash memory pages: ");
        printf("\n   Pages0to3    \t-->0 ");
		printf("\n   Pages4to7    \t-->1 ");
		printf("\n   Pages8to11   \t-->2 ");
		printf("\n   Pages12to15  \t-->3 ");
		printf("\n   Pages16to19  \t-->4 ");
		printf("\n   Pages20to23  \t-->5 ");
		printf("\n   Pages24to27  \t-->6 ");
		printf("\n   Pages28to31  \t-->7 ");
		printf("\n   Pages32to35  \t-->8 ");
		printf("\n   Pages36to39  \t-->9 ");
		printf("\n   Pages40to43  \t-->10");
		printf("\n   Pages44to47  \t-->11");
		printf("\n   Pages48to51  \t-->12");
		printf("\n   Pages52to55  \t-->13");
		printf("\n   Pages56to59  \t-->14");
		printf("\n   Pages60to63  \t-->15");
		printf("\n   Pages64to67  \t-->16");
		printf("\n   Pages68to71  \t-->17");
		printf("\n   Pages72to75  \t-->18");
		printf("\n   Pages76to79  \t-->19");
		printf("\n   Pages80to83  \t-->20");
		printf("\n   Pages84to87  \t-->21");
		printf("\n   Pages88to91  \t-->22");
		printf("\n   Pages92to95  \t-->23");
		printf("\n   Pages96to99  \t-->24");
		printf("\n   Pages100to103\t-->25");
		printf("\n   Pages104to107\t-->26");
		printf("\n   Pages108to111\t-->27");
		printf("\n   Pages112to115\t-->28");
		printf("\n   Pages116to119\t-->29");
		printf("\n   Pages120to123\t-->30");
		printf("\n   Pages124to127\t-->31");
		printf("\n   All pages\t-->32"    );
        printf("\n   Please Choose a number that,\n   corresponds to the pages you want to enable \n   their write-protection here: ");
        scanf("%d", &choice);

             if(choice == 0 )wrprot_mask=FLASH_WRProt_Pages0to3    ;
        else if(choice == 1 )wrprot_mask=FLASH_WRProt_Pages4to7    ;
        else if(choice == 2 )wrprot_mask=FLASH_WRProt_Pages8to11   ;
        else if(choice == 3 )wrprot_mask=FLASH_WRProt_Pages12to15  ;
        else if(choice == 4 )wrprot_mask=FLASH_WRProt_Pages16to19  ;
        else if(choice == 5 )wrprot_mask=FLASH_WRProt_Pages20to23  ;
        else if(choice == 6 )wrprot_mask=FLASH_WRProt_Pages24to27  ;
        else if(choice == 7 )wrprot_mask=FLASH_WRProt_Pages28to31  ;
        else if(choice == 8 )wrprot_mask=FLASH_WRProt_Pages32to35  ;
        else if(choice == 9 )wrprot_mask=FLASH_WRProt_Pages36to39  ;
        else if(choice == 10)wrprot_mask=FLASH_WRProt_Pages40to43  ;
        else if(choice == 11)wrprot_mask=FLASH_WRProt_Pages44to47  ;
        else if(choice == 12)wrprot_mask=FLASH_WRProt_Pages48to51  ;
        else if(choice == 13)wrprot_mask=FLASH_WRProt_Pages52to55  ;
        else if(choice == 14)wrprot_mask=FLASH_WRProt_Pages56to59  ;
        else if(choice == 15)wrprot_mask=FLASH_WRProt_Pages60to63  ;
        else if(choice == 16)wrprot_mask=FLASH_WRProt_Pages64to67  ;
        else if(choice == 17)wrprot_mask=FLASH_WRProt_Pages68to71  ;
        else if(choice == 18)wrprot_mask=FLASH_WRProt_Pages72to75  ;
        else if(choice == 19)wrprot_mask=FLASH_WRProt_Pages76to79  ;
        else if(choice == 20)wrprot_mask=FLASH_WRProt_Pages80to83  ;
        else if(choice == 21)wrprot_mask=FLASH_WRProt_Pages84to87  ;
        else if(choice == 22)wrprot_mask=FLASH_WRProt_Pages88to91  ;
        else if(choice == 23)wrprot_mask=FLASH_WRProt_Pages92to95  ;
        else if(choice == 24)wrprot_mask=FLASH_WRProt_Pages96to99  ;
        else if(choice == 25)wrprot_mask=FLASH_WRProt_Pages100to103;
        else if(choice == 26)wrprot_mask=FLASH_WRProt_Pages104to107;
        else if(choice == 27)wrprot_mask=FLASH_WRProt_Pages108to111;
        else if(choice == 28)wrprot_mask=FLASH_WRProt_Pages112to115;
        else if(choice == 29)wrprot_mask=FLASH_WRProt_Pages116to119;
        else if(choice == 30)wrprot_mask=FLASH_WRProt_Pages120to123;
        else if(choice == 31)wrprot_mask=FLASH_WRProt_Pages124to127;
		else if(choice == 32)wrprot_mask=FLASH_WRProt_AllPages     ;
		else {printf("\n\n   Invalid Input!!\r\n");return;}

        data_buf[2]=wrprot_mask;
        data_buf[3]=wrprot_mask>>8;
        data_buf[4]=wrprot_mask>>16;
        data_buf[5]=wrprot_mask>>24;

        crc32       = get_crc(data_buf,COMMAND_BL_EN_W_PROTECT_LEN-4);
        data_buf[6] = word_to_byte(crc32,1,1);
        data_buf[7] = word_to_byte(crc32,2,1);
        data_buf[8] = word_to_byte(crc32,3,1);
        data_buf[9] = word_to_byte(crc32,4,1);

         /*Convert buffer to char*/
        hex2char(data_buf,commandPacket_TxBuffer,COMMAND_BL_EN_W_PROTECT_LEN);

        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,COMMAND_BL_EN_W_PROTECT_LEN*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);

        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex[0]!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
            //printf("waiting for response\n");
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_EN_W_PROTECT, replyFromBootloaderHex);
        break;
    /*End of enable write protection*/
    /*Disable write protection*/
    case 12:
        printf("\n   Command == > BL_DIS_W_PROTECT");
        data_buf[0] = COMMAND_BL_DIS_W_PROTECT_LEN-1;   //command length macro
        data_buf[1] = COMMAND_BL_DIS_W_PROTECT; //command code macro

        printf("\n   Flash memory pages: ");
        printf("\n   Pages0to3    \t-->0 ");
		printf("\n   Pages4to7    \t-->1 ");
		printf("\n   Pages8to11   \t-->2 ");
		printf("\n   Pages12to15  \t-->3 ");
		printf("\n   Pages16to19  \t-->4 ");
		printf("\n   Pages20to23  \t-->5 ");
		printf("\n   Pages24to27  \t-->6 ");
		printf("\n   Pages28to31  \t-->7 ");
		printf("\n   Pages32to35  \t-->8 ");
		printf("\n   Pages36to39  \t-->9 ");
		printf("\n   Pages40to43  \t-->10");
		printf("\n   Pages44to47  \t-->11");
		printf("\n   Pages48to51  \t-->12");
		printf("\n   Pages52to55  \t-->13");
		printf("\n   Pages56to59  \t-->14");
		printf("\n   Pages60to63  \t-->15");
		printf("\n   Pages64to67  \t-->16");
		printf("\n   Pages68to71  \t-->17");
		printf("\n   Pages72to75  \t-->18");
		printf("\n   Pages76to79  \t-->19");
		printf("\n   Pages80to83  \t-->20");
		printf("\n   Pages84to87  \t-->21");
		printf("\n   Pages88to91  \t-->22");
		printf("\n   Pages92to95  \t-->23");
		printf("\n   Pages96to99  \t-->24");
		printf("\n   Pages100to103\t-->25");
		printf("\n   Pages104to107\t-->26");
		printf("\n   Pages108to111\t-->27");
		printf("\n   Pages112to115\t-->28");
		printf("\n   Pages116to119\t-->29");
		printf("\n   Pages120to123\t-->30");
		printf("\n   Pages124to127\t-->31");
		printf("\n   All pages\t-->32"    );
        printf("\n   Please Choose a number that,\n   corresponds to the pages you want to disable \n   their write-protection here: ");
        scanf("%d", &choice);

             if(choice == 0 )wrprot_mask=FLASH_WRProt_Pages0to3    ;
        else if(choice == 1 )wrprot_mask=FLASH_WRProt_Pages4to7    ;
        else if(choice == 2 )wrprot_mask=FLASH_WRProt_Pages8to11   ;
        else if(choice == 3 )wrprot_mask=FLASH_WRProt_Pages12to15  ;
        else if(choice == 4 )wrprot_mask=FLASH_WRProt_Pages16to19  ;
        else if(choice == 5 )wrprot_mask=FLASH_WRProt_Pages20to23  ;
        else if(choice == 6 )wrprot_mask=FLASH_WRProt_Pages24to27  ;
        else if(choice == 7 )wrprot_mask=FLASH_WRProt_Pages28to31  ;
        else if(choice == 8 )wrprot_mask=FLASH_WRProt_Pages32to35  ;
        else if(choice == 9 )wrprot_mask=FLASH_WRProt_Pages36to39  ;
        else if(choice == 10)wrprot_mask=FLASH_WRProt_Pages40to43  ;
        else if(choice == 11)wrprot_mask=FLASH_WRProt_Pages44to47  ;
        else if(choice == 12)wrprot_mask=FLASH_WRProt_Pages48to51  ;
        else if(choice == 13)wrprot_mask=FLASH_WRProt_Pages52to55  ;
        else if(choice == 14)wrprot_mask=FLASH_WRProt_Pages56to59  ;
        else if(choice == 15)wrprot_mask=FLASH_WRProt_Pages60to63  ;
        else if(choice == 16)wrprot_mask=FLASH_WRProt_Pages64to67  ;
        else if(choice == 17)wrprot_mask=FLASH_WRProt_Pages68to71  ;
        else if(choice == 18)wrprot_mask=FLASH_WRProt_Pages72to75  ;
        else if(choice == 19)wrprot_mask=FLASH_WRProt_Pages76to79  ;
        else if(choice == 20)wrprot_mask=FLASH_WRProt_Pages80to83  ;
        else if(choice == 21)wrprot_mask=FLASH_WRProt_Pages84to87  ;
        else if(choice == 22)wrprot_mask=FLASH_WRProt_Pages88to91  ;
        else if(choice == 23)wrprot_mask=FLASH_WRProt_Pages92to95  ;
        else if(choice == 24)wrprot_mask=FLASH_WRProt_Pages96to99  ;
        else if(choice == 25)wrprot_mask=FLASH_WRProt_Pages100to103;
        else if(choice == 26)wrprot_mask=FLASH_WRProt_Pages104to107;
        else if(choice == 27)wrprot_mask=FLASH_WRProt_Pages108to111;
        else if(choice == 28)wrprot_mask=FLASH_WRProt_Pages112to115;
        else if(choice == 29)wrprot_mask=FLASH_WRProt_Pages116to119;
        else if(choice == 30)wrprot_mask=FLASH_WRProt_Pages120to123;
        else if(choice == 31)wrprot_mask=FLASH_WRProt_Pages124to127;
		else if(choice == 32)wrprot_mask=FLASH_WRProt_AllPages     ;
		else {printf("\n\n   Invalid Input!!\r\n");return;}
        data_buf[2]=wrprot_mask;
        data_buf[3]=wrprot_mask>>8;
        data_buf[4]=wrprot_mask>>16;
        data_buf[5]=wrprot_mask>>24;

        crc32       = get_crc(data_buf,COMMAND_BL_DIS_W_PROTECT_LEN-4);
        data_buf[6] = word_to_byte(crc32,1,1);
        data_buf[7] = word_to_byte(crc32,2,1);
        data_buf[8] = word_to_byte(crc32,3,1);
        data_buf[9] = word_to_byte(crc32,4,1);

         /*Convert buffer to char*/
        hex2char(data_buf,commandPacket_TxBuffer,COMMAND_BL_DIS_W_PROTECT_LEN);

        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,COMMAND_BL_DIS_W_PROTECT_LEN*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);

        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex[0]!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
            //printf("waiting for response\n");
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_DIS_W_PROTECT, replyFromBootloaderHex);
        break;
    /*End of Disable memory write protection*/
    /*Get Read protection status*/
    case 13:
        printf("\n   Command == > BL_GET_RDP_STATUS");

        data_buf[0] = COMMAND_BL_GET_RDP_STATUS_LEN-1;
        data_buf[1] = COMMAND_BL_GET_RDP_STATUS;
        crc32       = get_crc(data_buf,COMMAND_BL_GET_RDP_STATUS_LEN-4);
        data_buf[2] = word_to_byte(crc32,1,1);
        data_buf[3] = word_to_byte(crc32,2,1);
        data_buf[4] = word_to_byte(crc32,3,1);
        data_buf[5] = word_to_byte(crc32,4,1);

                 /*Convert buffer to char to be sent through WIFI*/
        hex2char(data_buf,commandPacket_TxBuffer,COMMAND_BL_GET_RDP_STATUS_LEN);
        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,COMMAND_BL_GET_RDP_STATUS_LEN*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);

        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
            //printf("waiting for response\n");
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_GET_RDP_STATUS, replyFromBootloaderHex);
        break;
    /*End of get read protection*/
    /*Get Write Protection*/
    case 14:
        printf("\n   Command == > BL_PROTECTION_STATUS ");
        data_buf[0] = COMMAND_BL_READ_SECTOR_P_STATUS_LEN-1;    //command length macro
        data_buf[1] = COMMAND_BL_READ_SECTOR_P_STATUS;          //command code macro
        crc32       = get_crc(data_buf,COMMAND_BL_READ_SECTOR_P_STATUS_LEN-4);
        data_buf[2] = word_to_byte(crc32,1,1);
        data_buf[3] = word_to_byte(crc32,2,1);
        data_buf[4] = word_to_byte(crc32,3,1);
        data_buf[5] = word_to_byte(crc32,4,1);

                 /*Convert buffer to char to be sent through WIFI*/
        hex2char(data_buf,commandPacket_TxBuffer,COMMAND_BL_READ_SECTOR_P_STATUS_LEN);
        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,COMMAND_BL_READ_SECTOR_P_STATUS_LEN*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);

        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
            //printf("waiting for response\n");
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_READ_SECTOR_P_STATUS, replyFromBootloaderHex);
        break;
    /*End of get write protection*/
    /*System Reset*/
    case 15:
        printf("\n   Command == > BL_SYSTEM_RESET\n");
        data_buf[0] = COMMAND_BL_MY_SYSTEM_RESET_LEN-1;   //command length macro
        data_buf[1] = COMMAND_BL_MY_SYSTEM_RESET; //command code macro
        crc32       = get_crc(data_buf,COMMAND_BL_MY_SYSTEM_RESET_LEN-4);
        data_buf[2] = word_to_byte(crc32,1,1);
        data_buf[3] = word_to_byte(crc32,2,1);
        data_buf[4] = word_to_byte(crc32,3,1);
        data_buf[5] = word_to_byte(crc32,4,1);

        /*Convert buffer to char to be sent through WIFI*/
        hex2char(data_buf,commandPacket_TxBuffer,COMMAND_BL_MY_SYSTEM_RESET_LEN);
        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,COMMAND_BL_MY_SYSTEM_RESET_LEN*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);
        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
            //printf("waiting for response\n");
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_MY_SYSTEM_RESET, replyFromBootloaderHex);
        break;
        /*End of System Reser*/
case 16:
        printf("\n   Command == > BL_EXISTING_APPS\n");

        data_buf[0] = COMMAND_BL_EXISTING_APPS_LEN-1;   //command length macro
        data_buf[1] = COMMAND_BL_EXISTING_APPS; //command code macro
        crc32       = get_crc(data_buf,COMMAND_BL_EXISTING_APPS_LEN-4);
        data_buf[2] = word_to_byte(crc32,1,1);
        data_buf[3] = word_to_byte(crc32,2,1);
        data_buf[4] = word_to_byte(crc32,3,1);
        data_buf[5] = word_to_byte(crc32,4,1);

        /*Convert buffer to char to be sent through WIFI*/
        hex2char(data_buf,commandPacket_TxBuffer,COMMAND_BL_EXISTING_APPS_LEN);
        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,COMMAND_BL_EXISTING_APPS_LEN*2);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);
        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
            //printf("waiting for response\n");
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
//        for(uint16_t j=0;j<bl_reply_without_ack;j++)
//        {
//            replyFromBootloaderHex[2+j] = replyFromBootloaderChar [4+j];
//        }
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_EXISTING_APPS, replyFromBootloaderHex);
        break;
    case 17:
        printf("\n   Command == > COMMAND_BL_SAVE_APP_INFO\n");

        printf("\n\n   Enter the app name here : ");
        scanf(" %8s", app_name);
        while ((c = fgetc(stdin)) != '\n' && c != EOF); /* Flush stdin */
        printf(  "\n   Enter the app size in bytes here : ");
        scanf(" %d",&app_size_in_bytes);
        printf(  "\n   Enter the app base memory address here : ");
        scanf(" %x",&app_base_address);

        //printf("\n\nname: %s\nsize: %d\nbase memory address: %#x",app_name,app_size_in_bytes,app_base_address);

        data_buf[0] = COMMAND_BL_SAVE_APP_INFO_LEN-1;   //command length macro
        data_buf[1] = COMMAND_BL_SAVE_APP_INFO;         //command code macro

        data_buf[2]  =   word_to_byte(app_base_address,1,1);
        data_buf[3]  =   word_to_byte(app_base_address,2,1);
        data_buf[4]  =   word_to_byte(app_base_address,3,1);
        data_buf[5]  =   word_to_byte(app_base_address,4,1);

        data_buf[6]  =   app_size_in_bytes;
        data_buf[7]  =   app_size_in_bytes>>8;
        data_buf[8]  =   app_size_in_bytes>>16;
        data_buf[9]  =   app_size_in_bytes>>24;

        for(index=0;index<8;index++)
        {
            data_buf[10+index]=app_name[index];
            /*Convert 0 (which represents null) to ascii zero (0x30)*/
            if(data_buf[10+index]==0)data_buf[10+index]=0x30;
        }

        crc32       = get_crc(data_buf,COMMAND_BL_SAVE_APP_INFO_LEN-4);
        data_buf[18] = word_to_byte(crc32,1,1);
        data_buf[19] = word_to_byte(crc32,2,1);
        data_buf[20] = word_to_byte(crc32,3,1);
        data_buf[21] = word_to_byte(crc32,4,1);


        /*Convert buffer to char to be sent through WIFI*/
        //len to follow + command code + base address + app size in bytes
        hex2char(data_buf,commandPacket_TxBuffer,10);
        //name
        commandPacket_TxBuffer[20] = data_buf[10];
        commandPacket_TxBuffer[21] = data_buf[11];
        commandPacket_TxBuffer[22] = data_buf[12];
        commandPacket_TxBuffer[23] = data_buf[13];
        commandPacket_TxBuffer[24] = data_buf[14];
        commandPacket_TxBuffer[25] = data_buf[15];
        commandPacket_TxBuffer[26] = data_buf[16];
        commandPacket_TxBuffer[27] = data_buf[17];
        //crc
        hex2char(&data_buf[18],&commandPacket_TxBuffer[28],4);
        /*Send data to server*/
        HOST_voidSendCommand(commandPacket_TxBuffer,36);
        /*Give bootloader time to receive command and process it*/
        printf("\n   Waiting for bootloader to process request\n");
        delay(5000);
        while (replyFromBootloaderHex[0]!=0xA5 && replyFromBootloaderHex!=0x7f)
        {
            /*Get response from bootloader and through WIFI*/
            HOST_voidReceiveCommand(replyFromBootloaderChar);
            /*Convert 2 variables only from response from char to hex, which represent ack and size of packet*/
            char2hex(replyFromBootloaderChar,replyFromBootloaderHex,2);
            /*If we reached timeout threshold, break from loop, otherwise increase variable*/
            if(timeout_counter==TIMEOUT)break;
            timeout_counter++;
        }
        /*Save the size insize the variable which represents reply without ack size*/
        bl_reply_without_ack = replyFromBootloaderHex[1];
        /*Convert rest of array into hex*/
        char2hex(&replyFromBootloaderChar[4],&replyFromBootloaderHex[2],bl_reply_without_ack);
        //printf("Done receiving\n");

        /*Pass hex array to process it as reply of bootloader*/
        ret_value = read_bootloader_reply(COMMAND_BL_SAVE_APP_INFO, replyFromBootloaderHex);

        break;
    default:
        printf("\n\n  Please input valid command code\n");
        return;

    }

        if(ret_value == -2)
        {
            printf("\n\n   TimeOut : No response from the bootloader");
            printf("\n   Reset the board and Try Again !\n");
            return;
        }


}
