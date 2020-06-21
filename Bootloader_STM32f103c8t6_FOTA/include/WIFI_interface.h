/*
 * WIFI_interface.h
 *
 *  Created on: June 6, 2020
 *      Author: Mahmoud Hamdy
 *      Version: 1.0
 */
#ifndef WIFI_INTERFACE_H_
#define WIFI_INTERFACE_H_

#include "STD_TYPES.h"
#include "HUART_interface.h"

/*Macros*/
#define		 WIFI_COMMAND_GET_VERSION 						(u8*)"AT+GMR\r\n"
#define		 WIFI_COMMAND_GET_IP_ADDRESS 					(u8*)"AT+CIFSR\r\n"
#define		 WIFI_COMMAND_LIST_AP		 					(u8*)"AT+CWLAP\r\n"
#define		 WIFI_COMMAND_LIST_AP_NAMES_ONLY_CONFIG		 	(u8*)"AT+CWLAPOPT=0,0002\r\n"
#define 	 WIFI_COMMAND_SET_MODE_STATION					(u8*)"AT+CWMODE_CUR=1\r\n"
#define		 WIFI_COMMAND_GET_DATA_FROM_SERVER				(u8*)"GET https://api.thingspeak.com/apps/thinghttp/send_request?api_key=Y4JOXUDQZBLGOMHJ\r\nHost:api.thingspeak.com\r\n\r\n\r\n\r\n\r\n"
#define		 WIFI_COMMAND_GET_COMMAND_FROM_SERVER			(u8*)"GET https://api.thingspeak.com/channels/1082594/fields/1/last.txt?api_key=GL3M7JAK48BR8RRA\r\nHost:api.thingspeak.com\r\n"



/*Description: This API will be used to initialize WIFI module on USART 2
 * parameters: UART peripheral used for WIFI (UART_GPIO_t)
 * Return: Error Status*/
extern u8 WIFI_u8Init (UART_GPIO_t UART_Peripheral);

/*Description: This API will be used to set UART peripheral that will be used to display output
 * parameters: UART peripheral used for output (UART_GPIO_t)
 * Return: void*/
extern void WIFI_u8SetOutput (UART_GPIO_t Copy_UART_Peripheral);

/*Description: This API will be used to send AT command to the module
 * parameters: Desired Command
 * Return: Error Status*/
extern u8 WIFI_u8SendCommand(u8* Copy_u8DesiredCommand);

/*Description: This API will be used to connect to a specific WIFI access point
 * parameters: Access point name (u8*), password(u8*)
 * Return: Error Status*/
extern u8 WIFI_u8ConnectToAccessPoint (u8* Copy_u8SSID, u8* Copy_u8Password);

/*Description: This API will be used to receive data
 * parameters: Desired Command
 * Return: Error Status*/
extern u8 WIFI_u8ReceiveData (u8* Copy_u8DesiredCommand);

/*Description: This API will be used to send data our server (This is a specific function for the server we are using)
 * parameters: Desired Command (u8), Size to be sent (u16)
 * Return: Error Status*/
extern u8 WIFI_u8SendCommandToServer (u8* Copy_u8commandNumber, u16 Copy_u16Size);

/*Description: This API will be used to receive command passed to server
 * parameters: Desired Command
 * Return: Error Status*/
extern u8 WIFI_u8ReceiveCommand (u8* Copy_u8Command);



#endif
