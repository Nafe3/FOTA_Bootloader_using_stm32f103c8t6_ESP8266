/*bl
 * Bootloader.c
 *
 *  Created on: May 24, 2020
 *      Author: Mohamed Nafea
 */

#include "STD_TYPES.h"
#include "Debug.h"
#include "HUART_interface.h"
#include <string.h>


#include "CRC.h"
#include "Flash.h"
#include "Bootloader.h"
#include "Delay_interface.h"

#ifndef  SCB_BASE_ADDRESS
#define  SCB_BASE_ADDRESS       		0xE000ED00
#endif
#define  SCB_VTOR		 				*((volatile u32*)(SCB_BASE_ADDRESS+0x008))

#define  FLASH_BOOTLDR_BASE_ADDRESS		0x08000000
#define  FLASH_USR_APP_BASE_ADDRESS		0x08008000

#define BL_RX_LEN 						2200
u8  	bl_rx_buffer[BL_RX_LEN]      =  {0};

#define FLASH_RX_LEN					1024
u8 		FLASH_src_buffer_1K[FLASH_RX_LEN]=  {0};

/*Boot-loader supported commands*/
#define BL_GET_VER						0X51	/*This command is used to read the boot-loader version from the MCU*/
#define BL_GET_HELP				        0X52    /*This command is used to know what are the commands supported by the boot-loader*/
#define BL_GET_CID			            0X53    /*This command is used to read the MCU chip identification number*/

#define BL_GO_TO_ADDR		            0X55    /*This command is used to jump boot-loader to specified address.*/

#define BL_FLASH_ERASE		            0X56    /*This command is used to mass erase or sector erase of the user flash .*/
#define BL_FLASH_MASS_ERASE			    0X5B    /*This command is used to read the OTP contents.*/

#define BL_MEM_WRITE	                0X57    /*This command is used to write data in to different memories of the MCU*/
#define BL_MEM_READ				        0X59    /*This command is used to read data from different memories of the MCU.*/

#define BL_EN_R_PROTECT					0x5F	/**/
#define BL_DIS_R_PROTECT				0x60	/**/
#define BL_EN_W_PROTECT			        0X58    /*This command is used to enable write protection on different sectors of the user flash .*/
#define BL_DIS_W_PROTECT		        0X5C    /*This command is used to disable write protection on different sectors of the user flash . This command takes the protection status to default state.*/
#define BL_GET_RDP_STATUS	            0X54    /*This command is used to read the FLASH Read Protection level.*/
#define BL_PROTECTION_STATUS		    0X5A    /*This command is used to read all the sector protection status.*/

#define BL_SYSTEM_RESET					0X5D	/**/
#define BL_EXISTING_APPS				0x5E	/**/
#define BL_SAVE_APP_INFO				0x61	/*Set app info*/

/*ACK and NACK bytes*/
#define BL_ACK							0xA5
#define BL_NACK							0x7F

/*Boot-Loader version*/
#define BL_VERSION						0x10

/*CRC Verification return*/
#define VERIFY_CRC_SUCCESS				0U
#define VERIFY_CRC_FAIL					1U

/*MCU Chip ID*/
#define DBGMCU_IDCODE 					*((volatile u32*)0xE0042000)
#define DEV_ID_MASK						0x00000FFF
#define REV_ID_MASK						0xFFFF0000

/*GOTO Address command macros*/
#define ADDR_VALID   					0U
#define ADDR_INVALID  					1U
#define FLASH_START						0x08000000
#define FLASH_SIZE                      128*1024 		/*128K*/
#define FLASH_END                       (FLASH_START+FLASH_SIZE-1)
#define RAM_START                       0x20000000
#define RAM_SIZE                        20*1024 		/*20K*/
#define RAM_END                         (RAM_START+RAM_SIZE-1)


/*Commands handle functions prototypes*/
void bootloader_handle_getver_cmd				(u8* bl_rx_buffer);
void bootloader_handle_gethelp_cmd				(u8* bl_rx_buffer);
void bootloader_handle_getcid_cmd				(u8* bl_rx_buffer);
void bootloader_handle_goto_address_cmd			(u8* bl_rx_buffer);
void bootloader_handle_flash_erase_cmd			(u8* bl_rx_buffer);
void bootloader_handle_flash_mass_erase_cmd		(u8* bl_rx_buffer);///////////
void bootloader_handle_mem_write_cmd			(u8* bl_rx_buffer);
void bootloader_handle_mem_read_cmd				(u8* bl_rx_buffer);
void bootloader_handle_en_read_protect_cmd		(u8* bl_rx_buffer);
void bootloader_handle_dis_read_protect_cmd		(u8* bl_rx_buffer);
void bootloader_handle_en_write_protect_cmd		(u8* bl_rx_buffer);
void bootloader_handle_dis_write_protect_cmd	(u8* bl_rx_buffer);
void bootloader_handle_getrdp_cmd				(u8* bl_rx_buffer);
void bootloader_handle_read_sectors_status_cmd	(u8* bl_rx_buffer);
void bootloader_handle_system_reset_cmd			(u8* bl_rx_buffer);
void bootloader_handle_existing_apps_cmd		(u8* bl_rx_buffer);
void bootloader_handle_save_app_info_cmd		(u8* bl_rx_buffer);




/*Helper functions prototypes*/
void bootloader_send_ack(u8 follow_len);
void bootloader_send_nack(void);
u8   bootloader_verify_crc(u8* pData, u32 len, u32 crc_host);
u8   verify_address(u32 go_address);
void char2hex(u8* inBuffer, u8* outBuffer, u16 NumOfBytesToBeConverted );
void hex2char(u8* inBuffer, u8* outBuffer, u16 NumOfBytesToBeConverted );
u8   supported_commands[] = {
							BL_GET_VER				,
							BL_GET_HELP				,
							BL_GET_CID			    ,
							BL_GO_TO_ADDR			,
							BL_FLASH_ERASE		    ,
							BL_FLASH_MASS_ERASE	    ,
							BL_MEM_WRITE	        ,
							BL_MEM_READ			    ,
							BL_EN_R_PROTECT		    ,
							BL_DIS_R_PROTECT	    ,
							BL_EN_W_PROTECT		    ,
							BL_DIS_W_PROTECT	    ,
							BL_GET_RDP_STATUS	    ,
							BL_PROTECTION_STATUS    ,
							BL_SYSTEM_RESET		    ,
							BL_EXISTING_APPS		,
							BL_SAVE_APP_INFO
							};


/******************* Implementation of Boot-loader application functions **********************/
u8 i=0;
/*This iterator will be used for initializing the data array*/
u16 iterator=0;
GPIO_Pin_t OnBoard_Led;

/*Jumps to the user application code if there is no Boot-loader request*/
extern void bootloader_voidJumpToUserApp(void)
{
	printmsg1("BL_DEBUG_MSG: Button is not pressed .. executing user app \r\n");

	//just a function to hold the address of the reset handler of the user app
	void (*app_reset_handler)(void);

	/*1. configure the MSP by reading the value from the base address of the FLASH sector
	 * that contains the user app*/
	u32 msp_value = *(volatile u32*)FLASH_USR_APP_BASE_ADDRESS;  //getting the user app flash sector
	printmsg1("BL_DEBUG_MSG: MSP value = 0x%x\r\n",msp_value);

	//This function comes from CMSIS
	//__set_MSP(msp_value);										//forcing sp to go to the user app flash sector
	asm volatile ("MSR msp, %0\n" : : "r" (msp_value) : "sp");
	SCB_VTOR = FLASH_USR_APP_BASE_ADDRESS;						//vector table relocation

	/* 2. Now fetch the reset handler address of the user application
	 * from the location FLASH_USR_APP_BASE_ADDRESS
	 * Here, we gave the address of the reset handler to a pointer to function, so that
	 * this pointer to function (app_reset_handler)_ can trigger the user application's reset handler
	 * which jumps to the user's (main) function through (_start) function in (startup.c)
	 * */
	app_reset_handler = (void*)(*((volatile u32*)(FLASH_USR_APP_BASE_ADDRESS+0x04)));

	/*3. Jump to reset handler of the user application*/
	app_reset_handler();
}

/*Reads the command packet which comes from the host application*/
extern void bootloader_voidUARTReadData (void)
{

	OnBoard_Led.port = PORTC;
	OnBoard_Led.mode = GPIO_MODE_OUTPUT_PUSH_PULL;
	OnBoard_Led.pin  = 13;
	OnBoard_Led.speed= GPIO_OUTPUT_SPEED_50MHz;

	GPIO_Init(&OnBoard_Led);

	volatile u8 rcv_len = 0;
	/*This variable will be used to prevent from continuing until it receives all data*/
	u8 receiveFlag='0';

	//u8 var= 0x05;
	printmsg1("BL_DEBUG_MSG: Button is pressed .. going to BL mode\r\n");
	while(1)
	{
		/*Reinitialize these variables because they will be used as flags, this is to work well with the while loop inside*/
		rcv_len=0;
		receiveFlag=0;

		/*Reinitialize data buffer to zeros again*/
        memset(bl_rx_buffer,0,1000);
		/*here we will read and and decode the commands coming from host
		 * Frist read only one byte from the host, which is the "length" field of the command packet*/

		/**********Code that should be executed***************/
		//HUART_u8ReceiveSync(HUART_USART2,bl_rx_buffer,1); //reading the first byte from host
		//rcv_len = bl_rx_buffer[0];
		//HUART_u8ReceiveSync(HUART_USART2,&bl_rx_buffer[1],rcv_len);

		HUART_u8ReceiveAsync(HUART_USART2,bl_rx_buffer,1);
		//bl_rx_buffer[0]='0';
		/*If we dont make this while loop, the system will never add the bl_rx_buffer value to rcv_len*/
		while (rcv_len ==0)
		{
			//printmsg1("waiting for length\r\n");
			rcv_len = bl_rx_buffer[0];
		}
		printmsg1("rcv_len = %d\r\n", rcv_len);


		//delay_ms(500);
        HUART_u8ReceiveAsync(HUART_USART2,(bl_rx_buffer+1),rcv_len);
        /*Wait until the last character has been received through a while loop*/
        /*We use this while loop to make sure we have received all of the data till the last byte, and to prevent the code from advancing
         * before finishing*/
        while (receiveFlag==0)
        {
        	//printmsg1("We are waiting\r\n");

        	/*We made this if condition because for some reason, the check didn't work inside while loop condition*/
        	if (bl_rx_buffer[rcv_len]!= 0)
        	{
        		receiveFlag=1;
        	}
        }

		/*****************************FOR DEBUGGING*****************************/
		// 1) Array that should be received from BL_GET_VER
		//bl_rx_buffer[0]=	0x05;
		//bl_rx_buffer[1]=	0x51;
		//bl_rx_buffer[2]=	0xe7;
		//bl_rx_buffer[3]=	0xe9;
		//bl_rx_buffer[4]=	0xab;
		//bl_rx_buffer[5]=	0x7c;
		/************************************************************************/
		// 2) Array that should be received from BL_GET_HELP
		//bl_rx_buffer[0]=	0x05;
		//bl_rx_buffer[1]=	0x52;
		//bl_rx_buffer[2]=	0x3e;
		//bl_rx_buffer[3]=	0xcf;
		//bl_rx_buffer[4]=	0xe8;
		//bl_rx_buffer[5]=	0x71;
		/************************************************************************/
		// 3) Array that should be received from BL_GET_CID
		//bl_rx_buffer[0]=	0x05;
		//bl_rx_buffer[1]=	0x53;
		//bl_rx_buffer[2]=	0x89;
		//bl_rx_buffer[3]=	0xd2;
		//bl_rx_buffer[4]=	0x29;
		//bl_rx_buffer[5]=	0x75;
		/************************************************************************/
		// 4) Array that should be received from BL_GET_RDP_STATUS
		//bl_rx_buffer[0]=	0x05;
		//bl_rx_buffer[1]=	0x54;
		//bl_rx_buffer[2]=	0x8c;
		//bl_rx_buffer[3]=	0x82;
		//bl_rx_buffer[4]=	0x6e;
		//bl_rx_buffer[5]=	0x6b;
		/************************************************************************/
		// 5) Array that should be received from BL_GO_TO_ADDR
		//bl_rx_buffer[0]=	0x09;
		//bl_rx_buffer[1]=	0x55;
		//bl_rx_buffer[2]=	0x;
		//bl_rx_buffer[3]=	0x;
		//bl_rx_buffer[4]=	0x;
		//bl_rx_buffer[5]=	0x;
        //bl_rx_buffer[6]=	0x;
        //bl_rx_buffer[7]=	0x;
        //bl_rx_buffer[8]=	0x;
        //bl_rx_buffer[9]=	0x;
        /************************************************************************/
        // 6) Array that should be received from BL_FLASH_ERASE
        //bl_rx_buffer[0]=	0x07;
        //bl_rx_buffer[1]=	0x56;
        //bl_rx_buffer[2]=	0x;
        //bl_rx_buffer[3]=	0x;
        //bl_rx_buffer[4]=	0x;
        //bl_rx_buffer[5]=	0x;
        //bl_rx_buffer[6]=	0x;
        //bl_rx_buffer[7]=	0x;
        /************************************************************************/
        // 7) Array that should be received from BL_MEM_WRITE
        //bl_rx_buffer[0]=	0x0A+X; //10+x
        //bl_rx_buffer[1]=	0x57;
        //bl_rx_buffer[2]=	0x;
        //bl_rx_buffer[3]=	0x;
        //bl_rx_buffer[4]=	0x;
        //bl_rx_buffer[5]=	0x;
        //bl_rx_buffer[6]=	0x;
        //bl_rx_buffer[7]=	0x;
        /************************************************************************/
        // 8) Array that should be received from BL_MEM_READ
        //bl_rx_buffer[0]=	0x0A; //10 following bytes
        //bl_rx_buffer[1]=	0x59;
        //bl_rx_buffer[2]=	0x;
        //bl_rx_buffer[3]=	0x;
        //bl_rx_buffer[4]=	0x;
        //bl_rx_buffer[5]=	0x;
        //bl_rx_buffer[6]=	0x;
        //bl_rx_buffer[7]=	0x;
        //bl_rx_buffer[8]=	0x;
        //bl_rx_buffer[9]=	0x;
        //bl_rx_buffer[10]=	0x;
        /************************************************************************/
        // 9) Array that should be received from BL_READ_SECTOR_STATUS
        //bl_rx_buffer[0]=	0x05;
        //bl_rx_buffer[1]=	0x5A;
        //bl_rx_buffer[2]=	0x;
        //bl_rx_buffer[3]=	0x;
        //bl_rx_buffer[4]=	0x;
        //bl_rx_buffer[5]=	0x;
        /************************************************************************/
        // 10) Array that should be received from BL_EN_R_W_PROTECT
        //bl_rx_buffer[0]=	0x07;
        //bl_rx_buffer[1]=	0x58;
        //bl_rx_buffer[2]=	0x;
        //bl_rx_buffer[3]=	0x;
        //bl_rx_buffer[4]=	0x;
        //bl_rx_buffer[5]=	0x;
        //bl_rx_buffer[6]=	0x;
        //bl_rx_buffer[7]=	0x;
        /************************************************************************/
        // 11) Array that should be received from BL_DIS_R_W_PROTECT
        //bl_rx_buffer[0]=	0x05;
        //bl_rx_buffer[1]=	0x5C;
        //bl_rx_buffer[2]=	0x;
        //bl_rx_buffer[3]=	0x;
        //bl_rx_buffer[4]=	0x;
        //bl_rx_buffer[5]=	0x;

		//HUART_u8SendSync(HUART_USART1,bl_rx_buffer,rcv_len,1);
		//for(i=0;i<=rcv_len;i++){
		//printmsg1("0x%x\r\n",bl_rx_buffer[i]);
		//}

		/***************************************************************************/
		switch(bl_rx_buffer[1]) //checking for the received command and then executing its code
		{
			case BL_GET_VER:
				bootloader_handle_getver_cmd(bl_rx_buffer);
				break;
			case BL_GET_HELP:
				bootloader_handle_gethelp_cmd(bl_rx_buffer);
				break;
			case BL_GET_CID:
				bootloader_handle_getcid_cmd(bl_rx_buffer);
				break;

			case BL_GO_TO_ADDR:
				bootloader_handle_goto_address_cmd(bl_rx_buffer);
				break;

			case BL_FLASH_ERASE:
				bootloader_handle_flash_erase_cmd(bl_rx_buffer);
				break;
			case BL_FLASH_MASS_ERASE:
				bootloader_handle_flash_mass_erase_cmd(bl_rx_buffer);
				break;

			case BL_MEM_WRITE:
				bootloader_handle_mem_write_cmd(bl_rx_buffer);
				break;
			case BL_MEM_READ:
				bootloader_handle_mem_read_cmd(bl_rx_buffer);
				break;

			case BL_EN_R_PROTECT:
				bootloader_handle_en_read_protect_cmd(bl_rx_buffer);
				break;
			case BL_DIS_R_PROTECT:
				bootloader_handle_dis_read_protect_cmd(bl_rx_buffer);
				break;
			case BL_EN_W_PROTECT:
				bootloader_handle_en_write_protect_cmd(bl_rx_buffer);
				break;
			case BL_DIS_W_PROTECT:
				bootloader_handle_dis_write_protect_cmd(bl_rx_buffer);
				break;
			case BL_GET_RDP_STATUS:
				bootloader_handle_getrdp_cmd(bl_rx_buffer);
				break;
			case BL_PROTECTION_STATUS:
				bootloader_handle_read_sectors_status_cmd(bl_rx_buffer);
				break;

			case BL_SYSTEM_RESET:
				bootloader_handle_system_reset_cmd(bl_rx_buffer);
				break;
			case BL_EXISTING_APPS:
				bootloader_handle_existing_apps_cmd(bl_rx_buffer);
				break;
			case BL_SAVE_APP_INFO:
				bootloader_handle_save_app_info_cmd(bl_rx_buffer);
				break;
			default:
				printmsg1("BL_DEBUG_MSG: Invalid command code received from host \r\n");
				break;
		}
	}
}

/******************* Implementation of Boot-loader Command Handle Functions *******************/

/*Helper function to handle BL_GET_VER command*/
void bootloader_handle_getver_cmd				(u8* bl_rx_buffer)
{

	u8  bl_version;											/*variable to store BL version*/
	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-4;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;
	crc_host= *((u32*)(bl_rx_buffer+command_packet-4));          /*Extract the CRC32 sent by host*/

	printmsg1("BL_DEBUG_MSG: bootloader_handle_getver_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//Stating that a reply of one byte is going to be sent
		bootloader_send_ack(1);
		//Sending boot-loader version
		bl_version = BL_VERSION;
		printmsg1("BL_DEBUG_MSG: BL_VER : 0x%x \r\n",bl_version);
		HUART_u8SendSync(HUART_USART2,&bl_version,1,10); //sending version to host
	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}

}

/*Handle function to handle BL_GET_HELP command
 * Bootloader sends out all supported command codes*/
void bootloader_handle_gethelp_cmd				(u8* bl_rx_buffer)
{
		u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
		u32 command_length_without_crc = command_packet-4;      /*Length to be sent to (bl_verify_crc) function*/
		u32 crc_host;
		crc_host= *((u32*)(bl_rx_buffer+command_packet-4));          /*Extract the CRC32 sent by host*/

		printmsg1("BL_DEBUG_MSG: bootloader_handle_gethelp_cmd \r\n");
		// 1) verify the checksum
		if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
		{
			//checksum is correct
			printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
			//Stating that a reply of eight bytes is going to be sent
			bootloader_send_ack(sizeof(supported_commands));
			//Sending boot-loader supported commands
			HUART_u8SendSync(HUART_USART2,supported_commands,sizeof(supported_commands),10);
			for(i=0;i<sizeof(supported_commands);i++)
			printmsg1("0x%x\r\n",supported_commands[i]);//debugging
		}
		else
		{
			//checksum is wrong send nack
			printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
			bootloader_send_nack();
		}

}

/*Handle function to handle BL_GET_CID command*/
void bootloader_handle_getcid_cmd				(u8* bl_rx_buffer)
{
	u16 device_id 	= DBGMCU_IDCODE & DEV_ID_MASK;
	u16 revision_id = DBGMCU_IDCODE & REV_ID_MASK;
	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-4;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;
	crc_host= *((u32*)(bl_rx_buffer+command_packet-4));     /*Extract the CRC32 sent by host*/

	printmsg1("BL_DEBUG_MSG: bootloader_handle_gethelp_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//Stating that a reply of four bytes is going to be sent
		bootloader_send_ack(4);
		//Sending device ID and Revision ID
		HUART_u8SendSync(HUART_USART2,(u8*)&device_id,2,10);
		HUART_u8SendSync(HUART_USART2,(u8*)&revision_id,2,10);
		//for debugging
		printmsg1("Device ID:   %#x\r\n",device_id);
		printmsg1("Revision ID: %#x\r\n",revision_id);

	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}
}

/*Handle function to handle BL_GO_TO_ADDR command*/
void bootloader_handle_goto_address_cmd			(u8* bl_rx_buffer)
{
	u32 go_address=0;
	u8 addr_valid   = ADDR_VALID;
	u8 addr_invalid = ADDR_INVALID;

	u8  command_packet = bl_rx_buffer[0]+1;                 	/*Total length of command packet*/
	u32 command_length_without_crc = command_packet-4;      	/*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;
	u8 Local_u8FinalAddress[4];									/*This local variable will hold the final address value that should be passed*/

	crc_host= *((u32*)(bl_rx_buffer+command_packet-4));         /*Extract the CRC32 sent by host*/

	printmsg1("BL_DEBUG_MSG: bootloader_handle_goto_address_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//Stating that a reply of one byte is going to be sent
		bootloader_send_ack(1);
        //extract the go address
		char2hex(&bl_rx_buffer[2],Local_u8FinalAddress,4);
		go_address = *((u32*)Local_u8FinalAddress);

        printmsg1("BL_DEBUG_MSG: GO addr: 0x%02x%02x%02x%02x\r\n",Local_u8FinalAddress[3],Local_u8FinalAddress[2],Local_u8FinalAddress[1],Local_u8FinalAddress[0]);

        //processing
        if( verify_address(go_address) == ADDR_VALID )
        	{
				//tell host that address is fine
				HUART_u8SendSync(HUART_USART2,&addr_valid,1,10);

				/*jump to "go" address.
				we dont care what is being done there.
				host must ensure that valid code is present over there
				Its not the duty of bootloader. so just trust and jump */

				/* Not doing the below line will result in hardfault exception for ARM cortex M */
				//watch : https://www.youtube.com/watch?v=VX_12SjnNhY

				//  go_address+=1; //make T bit =1

				void (*lets_jump)(void) = (void *)(*(volatile u32*)go_address);
			   // void (*lets_jump)(void) = (void *)Local_u8FinalAddress;

				printmsg1("BL_DEBUG_MSG: jumping to go address! \n");

				lets_jump();

			}
        else
			{
				printmsg1("BL_DEBUG_MSG:GO addr invalid ! \n");
				//tell host that address is invalid
				HUART_u8SendSync(HUART_USART2,&addr_invalid,1,10);
			}

	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}
}

/*Handle function to handle BL_FLASH_ERASE command*/
void bootloader_handle_flash_erase_cmd			(u8* bl_rx_buffer)
{
	u8  status;
	u8  Local_u8FinalAddress[4];								/*This local variable will hold the concatenated address  value that should be passed*/
	u8  Local_u8FinalHostCRC[4];								/*This local variable will hold the concatenated host crc value that should be passed*/
	u8  number_of_sectors_to_be_erased = 0 ;
	u32 sector_start_address = 0 ;


	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-8;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;


	char2hex(&bl_rx_buffer[command_length_without_crc], Local_u8FinalHostCRC, 4);
	crc_host= *((u32*)Local_u8FinalHostCRC);     /*Extract the CRC32 sent by host*/

	printmsg1("------------------------------------------------\r\n");
	printmsg1("BL_DEBUG_MSG: bootloader_handle_flash_erase_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//processing
		char2hex(&bl_rx_buffer[2],Local_u8FinalAddress,4);
		sector_start_address = *((u32*)Local_u8FinalAddress);

		number_of_sectors_to_be_erased = bl_rx_buffer[10];
		FLASH_Unlock();
		status = FLASH_MultiplePageErase(sector_start_address,number_of_sectors_to_be_erased);

		//Stating that a reply of one byte is going to be sent
		bootloader_send_ack(1);
		//Sending status to HOST application
		HUART_u8SendSync(HUART_USART2,&status,1,10);
	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_flash_mass_erase_cmd		(u8* bl_rx_buffer)
{
	u8  Local_u8FinalHostCRC[4];

	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-8;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;


	char2hex(&bl_rx_buffer[command_length_without_crc], Local_u8FinalHostCRC, 4);
	crc_host= *((u32*)Local_u8FinalHostCRC);     /*Extract the CRC32 sent by host*/

	printmsg1("------------------------------------------------\r\n");
	printmsg1("BL_DEBUG_MSG: bootloader_handle_flash_mass_erase_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//processing
		//Stating that a reply of zero bytes is going to be sent
		bootloader_send_ack(0);

		FLASH_Unlock();
		FLASH_MassErase();
	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}
}

/*Handle function to handle BL_MEM_WRITE command*/
void bootloader_handle_mem_write_cmd			(u8* bl_rx_buffer)
{
	GPIO_Pin_Write(&OnBoard_Led,LOW);
	u32 destination_address = 0 ;
	u8 addr_valid   = ADDR_VALID;
	u8 addr_invalid = ADDR_INVALID;

	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-8;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;
	u32 len_to_read = bl_rx_buffer[10];
	u8 Local_u8FinalAddress[4];								/*This local variable will hold the concatenated address  value that should be passed*/
	u8 Local_u8FinalHostCRC[4];								/*This local variable will hold the concatenated host crc value that should be passed*/

	char2hex(&bl_rx_buffer[11+len_to_read], Local_u8FinalHostCRC, 4);
	crc_host= *((u32*)Local_u8FinalHostCRC);     			/*Extract the CRC32 sent by host*/

	printmsg1("------------------------------------------------\r\nBL_DEBUG_MSG: bootloader_handle_mem_write_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");

		//processing

		//extracting the destination address
		char2hex(&bl_rx_buffer[2],Local_u8FinalAddress,4);
		destination_address = *((u32*)Local_u8FinalAddress);

		printmsg1("BL_DEBUG_MSG: destination address: 0x%02x%02x%02x%02x\r\n",Local_u8FinalAddress[3],Local_u8FinalAddress[2],Local_u8FinalAddress[1],Local_u8FinalAddress[0]);
		 if( verify_address(destination_address) == ADDR_VALID )
		 {

			 	//FLASH_MultiplePageErase   			(u32 pageAddress, 8); //since the file's size is 7992 bytes and that's about 8KB
			    char2hex(&bl_rx_buffer[11],FLASH_src_buffer_1K,64);

			    FLASH_Unlock();
			   // FLASH_PageErase((u32)0x08008000);
			    FLASH_WriteProgram	((u32*)FLASH_src_buffer_1K, (u32*)destination_address, 64);
				//Stating that a reply of one byte is going to be sent
				bootloader_send_ack(1);
				//tell host that address is fine
				HUART_u8SendSync(HUART_USART2,&addr_valid,1,10);
		 }
		 else
		{
			printmsg1("BL_DEBUG_MSG:GO addr invalid ! \n");
			//tell host that address is invalid
			HUART_u8SendSync(HUART_USART2,&addr_invalid,1,10);
		}

	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}
	GPIO_Pin_Write(&OnBoard_Led,HIGH);
}

/*Handle function to handle BL_MEM_READ command*/
void bootloader_handle_mem_read_cmd				(u8* bl_rx_buffer)
{
	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-4;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;
	crc_host= *((u32*)(bl_rx_buffer+command_packet-4));          /*Extract the CRC32 sent by host*/

	printmsg1("BL_DEBUG_MSG: bootloader_handle_gethelp_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//Stating that a reply of one byte is going to be sent
		bootloader_send_ack(1);
		//processing
		HUART_u8SendSync(HUART_USART2,supported_commands,sizeof(supported_commands),10);

	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_en_read_protect_cmd		(u8* bl_rx_buffer)
{
	u8  Local_u8FinalHostCRC[4];

	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-8;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;

	char2hex(&bl_rx_buffer[command_length_without_crc], Local_u8FinalHostCRC, 4);
	crc_host= *((u32*)Local_u8FinalHostCRC);     /*Extract the CRC32 sent by host*/

	printmsg1("------------------------------------------------\r\n");
	printmsg1("BL_DEBUG_MSG: bootloader_handle_en_read_protect_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//processing
		//Stating that a reply of zero bytes is going to be sent
		bootloader_send_ack(0);

		FLASH_OPT_Unlock();
		FLASH_OPT_ReadProtection_Enable();
	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_dis_read_protect_cmd		(u8* bl_rx_buffer)
{
	u8  Local_u8FinalHostCRC[4];

	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-8;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;


	char2hex(&bl_rx_buffer[command_length_without_crc], Local_u8FinalHostCRC, 4);
	crc_host= *((u32*)Local_u8FinalHostCRC);     /*Extract the CRC32 sent by host*/

	printmsg1("------------------------------------------------\r\n");
	printmsg1("BL_DEBUG_MSG: bootloader_handle_dis_read_protect_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//processing
		//Stating that a reply of zero bytes is going to be sent
		bootloader_send_ack(0);

		FLASH_OPT_Unlock();
		FLASH_OPT_ReadProtection_Disable();
	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_en_write_protect_cmd		(u8* bl_rx_buffer)
{
	u8  Local_u8FinalHostCRC[4];
	u8  Local_u8FinalWRProt_mask[4];
	u32 WRProt_mask = 0;

	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-8;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;


	char2hex(&bl_rx_buffer[command_length_without_crc], Local_u8FinalHostCRC, 4);
	crc_host= *((u32*)Local_u8FinalHostCRC);     /*Extract the CRC32 sent by host*/

	printmsg1("------------------------------------------------\r\n");
	printmsg1("BL_DEBUG_MSG: bootloader_handle_en_write_protect_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//processing
		char2hex(&bl_rx_buffer[2], Local_u8FinalWRProt_mask, 4);
		WRProt_mask= *((u32*)Local_u8FinalWRProt_mask);     /*Extract the WRProt_mask sent by host*/

		FLASH_OPT_Unlock();
		FLASH_OPT_WriteProtection_Enable(WRProt_mask);

		//Stating that a reply of zero bytes is going to be sent
		bootloader_send_ack(0);

	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_dis_write_protect_cmd	(u8* bl_rx_buffer)
{
	u8  Local_u8FinalHostCRC[4];
	u8  Local_u8FinalWRProt_mask[4];
	u32 WRProt_mask = 0;

	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-8;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;


	char2hex(&bl_rx_buffer[command_length_without_crc], Local_u8FinalHostCRC, 4);
	crc_host= *((u32*)Local_u8FinalHostCRC);     /*Extract the CRC32 sent by host*/

	printmsg1("------------------------------------------------\r\n");
	printmsg1("BL_DEBUG_MSG: bootloader_handle_dis_write_protect_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//processing
		char2hex(&bl_rx_buffer[2], Local_u8FinalWRProt_mask, 4);
		WRProt_mask= *((u32*)Local_u8FinalWRProt_mask);     /*Extract the WRProt_mask sent by host*/

		FLASH_OPT_Unlock();
		FLASH_OPT_WriteProtection_Disable(WRProt_mask);

		//Stating that a reply of zero bytes is going to be sent
		bootloader_send_ack(0);

	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}
}

/*Handle function to handle BL_GET_RDP_STATUS command*/
void bootloader_handle_getrdp_cmd				(u8* bl_rx_buffer)
{
	u8  RDP_status;
	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-4;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;
	crc_host= *((u32*)(bl_rx_buffer+command_packet-4));          /*Extract the CRC32 sent by host*/

	printmsg1("BL_DEBUG_MSG: bootloader_handle_getrdp_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//Stating that a reply of one byte is going to be sent
		bootloader_send_ack(1);
		//processing
		RDP_status = FLASH_OPT_GetRDPStatus();
		HUART_u8SendSync(HUART_USART2,&RDP_status,1,10);

	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}
}

/*Handle function to handle BL_READ_SECTOR_STATUS command*/
void bootloader_handle_read_sectors_status_cmd	(u8* bl_rx_buffer)
{
	#define REPLY_LEN 10
	u8  RDP_status;
	u32 WRP_status;
	u8  Local_u8FinalHostCRC[4];
	u8  Local_u8Tx_buffer[REPLY_LEN];//10 bytes of reply

	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-8;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;


	char2hex(&bl_rx_buffer[command_length_without_crc], Local_u8FinalHostCRC, 4);
	crc_host= *((u32*)Local_u8FinalHostCRC);     /*Extract the CRC32 sent by host*/

	printmsg1("------------------------------------------------\r\n");
	printmsg1("BL_DEBUG_MSG: bootloader_handle_read_sectors_status_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//processing
		RDP_status = FLASH_OPT_GetRDPStatus();
		WRP_status = FLASH_OPT_GetWRPStatus();
		hex2char(&RDP_status, &Local_u8Tx_buffer[0], 1);
		hex2char((u8*)&WRP_status, &Local_u8Tx_buffer[2], 4);
		//Stating that a reply of 10 bytes is going to be sent
		bootloader_send_ack(REPLY_LEN);
		HUART_u8SendSync(HUART_USART2,Local_u8Tx_buffer,REPLY_LEN,10);
	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_system_reset_cmd			(u8* bl_rx_buffer)
{
	u8  Local_u8FinalHostCRC[4];

	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-8;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;

	char2hex(&bl_rx_buffer[command_length_without_crc], Local_u8FinalHostCRC, 4);
	crc_host= *((u32*)Local_u8FinalHostCRC);     /*Extract the CRC32 sent by host*/

	printmsg1("------------------------------------------------\r\n");
	printmsg1("BL_DEBUG_MSG: bootloader_handle_system_reset_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//processing
		//Stating that a reply of zero bytes is going to be sent
		bootloader_send_ack(0);

		FLASH_SystemReset();
	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}
}
void bootloader_handle_existing_apps_cmd		(u8* bl_rx_buffer)
{
	u8  index;
	//u32 application_info_block_start_address = FLASH_MEMORY_PAGE_19;
	u8  number_of_apps = *((u8*)FLASH_MEMORY_PAGE_19);
	u8  Local_u8FinalHostCRC[4];

	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-8;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;

	char2hex(&bl_rx_buffer[command_length_without_crc], Local_u8FinalHostCRC, 4);
	crc_host= *((u32*)Local_u8FinalHostCRC);     /*Extract the CRC32 sent by host*/

	printmsg1("------------------------------------------------\r\n");
	printmsg1("BL_DEBUG_MSG: bootloader_handle_existing_apps_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//processing

		//Stating that a reply of zero bytes is going to be sent
		bootloader_send_ack(number_of_apps*16);
		for(index=0;index<number_of_apps;index++)
		{
			HUART_u8SendSync(HUART_USART2,(u8*)(FLASH_MEMORY_PAGE_19+16+(index*16))  ,4,10);//sending app base memory address
			HUART_u8SendSync(HUART_USART2,(u8*)(FLASH_MEMORY_PAGE_19+16+(index*16)+4),4,10);//sending app size in bytes
			HUART_u8SendSync(HUART_USART2,(u8*)(FLASH_MEMORY_PAGE_19+16+(index*16)+8),8,10);//sending app name
		}


	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}
}

void bootloader_handle_save_app_info_cmd		(u8* bl_rx_buffer)
//void save_app_info(u8 app_num, u32 app_base_address, u32 app_size_in_bytes, u8* app_name)
{
	u8  index;
	u8  status  =0;
	//u8  number_of_apps = *((u8*)FLASH_MEMORY_PAGE_19); //0
	u8  number_of_apps = 0;
	u32 app_base_address=0;
	u8  Local_u8FinalAppBaseAddress[4]={0};
	u8  Local_u8FinalAppSizeInBytes[4]={0};
	//u32 destination_address = FLASH_MEMORY_PAGE_19+16+(16*(number_of_apps-1));
	u32 app_size_in_bytes=0;
	u8  app_name[8]={0};
	u8  Local_u8FinalHostCRC[4];


	u8  command_packet = bl_rx_buffer[0]+1;                 /*Total length of command packet*/
	u32 command_length_without_crc = command_packet-8;      /*Length to be sent to (bl_verify_crc) function*/
	u32 crc_host;


	char2hex(&bl_rx_buffer[command_length_without_crc], Local_u8FinalHostCRC, 4);
	crc_host= *((u32*)Local_u8FinalHostCRC);     /*Extract the CRC32 sent by host*/

	printmsg1("------------------------------------------------\r\n");
	printmsg1("BL_DEBUG_MSG: bootloader_handle_save_app_info_cmd \r\n");
	// 1) verify the checksum
	if(! bootloader_verify_crc(bl_rx_buffer, command_length_without_crc, crc_host))
	{
		//checksum is correct
		printmsg1("BL_DEBUG_MSG: checksum success !! \r\n");
		//processing
		number_of_apps = *((u8*)FLASH_MEMORY_PAGE_19);
		if(number_of_apps==0xFF)number_of_apps =0;
		number_of_apps++;
		char2hex(&bl_rx_buffer[2], Local_u8FinalAppBaseAddress ,4);
		char2hex(&bl_rx_buffer[10],Local_u8FinalAppSizeInBytes ,4);
		app_base_address =*((u32*)Local_u8FinalAppBaseAddress);
		app_size_in_bytes=*((u32*)Local_u8FinalAppSizeInBytes);

		for(index=0;index<8;index++)
			app_name[index]=bl_rx_buffer[18+index];
		printmsg1("\nNumber of apps: %d"    ,number_of_apps);
		printmsg1("\r\nApp name: %s"        ,app_name);
		printmsg1("\r\nApp size: %d bytes"    ,app_size_in_bytes);
		printmsg1("\r\nApp Base address: %#x\r\n"  ,app_base_address);

		FLASH_Unlock();
		status = FLASH_savePage(FLASH_MEMORY_PAGE_19,SAVE_FLASH);		/*save*/
		status = FLASH_PageErase(FLASH_MEMORY_PAGE_19);					/*erase*/
		FLASH_updatePage((u32*)&number_of_apps   ,1,0,SAVE_FLASH);		/*update number of apps			*/
		FLASH_updatePage(&app_base_address     ,4,(16+(16*(number_of_apps-1))),SAVE_FLASH);		/*update app base memory address*/
		FLASH_updatePage(&app_size_in_bytes    ,4,(20+(16*(number_of_apps-1))),SAVE_FLASH);		/*update app size in bytes      */
		FLASH_updatePage((u32*)app_name        ,8,(24+(16*(number_of_apps-1))),SAVE_FLASH);		/*update app name               */
		//FLASH_updatePage((u32*)app_name        ,4,(24+(16*(number_of_apps-1))),SAVE_FLASH);		/*update app name               */
		//FLASH_updatePage((u32*)&app_name[4]    ,4,(28+(16*(number_of_apps-1))),SAVE_FLASH);		/*update app name               */
		FLASH_reloadPage(FLASH_MEMORY_PAGE_19,SAVE_FLASH);              /*reload*/
		FLASH_Lock();
		//Stating that a reply of 10 bytes is going to be sent
		bootloader_send_ack(1);
		HUART_u8SendSync(HUART_USART2,&status,1,10);
	}
	else
	{
		//checksum is wrong send nack
		printmsg1("BL_DEBUG_MSG: checksum fail !! \r\n");
		bootloader_send_nack();
	}




	//status = FLASH_savePage(address,SAVE_FLASH);			  /*save*/
	//status = FLASH_MultiplePageErase(address,1);        	  /*erase*/
	//FLASH_updatePage(new,2,3,SAVE_FLASH);                   /*update*/
	//FLASH_reloadPage(address,SAVE_FLASH);                   /*reload*/

	//FLASH_WriteProgram((u8*)app_name, (u32*)destination_address, 8);
	//destination_address += 8 ;
	//FLASH_WriteWord((u32*)destination_address, app_base_address);
	//destination_address += 4 ;
	//FLASH_WriteWord((u32*)destination_address, app_size_in_bytes);
    //
	//FLASH_Lock();
}
/******************* Implementation Helper functions prototypes **********************************************/

void bootloader_send_ack(u8 follow_len)
{
	//here we send 2 bytes .. first byte is ack and the second byte is the length of the following
	//reply bytes
	u8 ack_buffer[2]={BL_ACK,follow_len};
	HUART_u8SendSync(HUART_USART2,ack_buffer,2,10);
	printmsg1("Sending BL_ACK: 0x%x\r\n",ack_buffer[0]);
	printmsg1("reply length= %d bytes\r\n",ack_buffer[1]);
}

void bootloader_send_nack(void)
{
	HUART_u8SendSync(HUART_USART2,(u8*)BL_NACK,1,10);
}

//This verifies the CRC of the given buffer in pData
u8 bootloader_verify_crc(u8* pData, u32 len, u32 crc_host)
{
	u32 i;
	u32 iData;
	u32 crc_rcv =0;
	CRC_ResetDR();
	for(i=0;i<len;i++)
	{
		iData= pData[i];
		crc_rcv = CRC_CalcBlockCRC(&iData, 1);
	}
	if(crc_rcv==crc_host)
		return VERIFY_CRC_SUCCESS;
	return VERIFY_CRC_FAIL;
}

u8 verify_address(u32 go_address)
{
	if ( go_address >= RAM_START && go_address <= RAM_END)
		{
			return ADDR_VALID;
		}
	else if ( go_address >= FLASH_START && go_address <= FLASH_END)
		{
			return ADDR_VALID;
		}
	else
			return ADDR_INVALID;

}



/* convert (inBuffer) which has (char) elements of double the size of the (outBuffer)
 * merging every two bytes of the (inBuffer) into one byte of (outBuffer)
 * This is done as we had to receive every byte(Hex) as two bytes in their (ASCII) representation
 * for example : (inBuffer) has 1024 bytes
 * 				 (outBuffer) has 512 bytes
 * 				 so we are going to iterate for 512 times*/
void   char2hex(u8* inBuffer, u8* outBuffer, u16 NumOfBytesToBeConverted )
{
	u16 index;
	for(index=0;index<NumOfBytesToBeConverted;index++)
	{
		//outBuffer[NumOfBytesToBeConverted-1-index]  = ( (inBuffer[index*2]<<4) | (inBuffer[index*2+1]&0x0F) );
		outBuffer[index]  = ( (inBuffer[index*2]<<4) | (inBuffer[index*2+1]&0x0F) );
	}
}

void hex2char(u8* inBuffer, u8* outBuffer, u16 NumOfBytesToBeConverted)
{
    u16 index;

    for(index=0;index<NumOfBytesToBeConverted;index++)
    {
        outBuffer[index*2] = inBuffer[index] & 0xF0 ;
        outBuffer[index*2] = outBuffer[index*2] >> 4 ;

        if      ( outBuffer[index*2] >= 0x0 && outBuffer[index*2] <= 0x9 )
                  outBuffer[index*2] |= 0x30;
        else if ( outBuffer[index*2] >= 0xA && outBuffer[index*2] <= 0xF )
                  outBuffer[index*2] |= 0x40;

        outBuffer[index*2+1] = inBuffer[index] & 0x0F ;

        if      ( outBuffer[index*2+1] >= 0x0 && outBuffer[index*2+1] <= 0x9 )
                  outBuffer[index*2+1] |= 0x30;
        else if ( outBuffer[index*2+1] >= 0xA && outBuffer[index*2+1] <= 0xF )
                  outBuffer[index*2+1] |= 0x40;

    }
}
