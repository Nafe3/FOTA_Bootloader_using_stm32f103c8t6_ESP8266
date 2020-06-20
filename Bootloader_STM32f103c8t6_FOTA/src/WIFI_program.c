/*
 * WIFI_program.c
 *
 *  Created on: Jun 6, 2020
 *      Author: Mahmoud
 *      Version: 1.0
 */

#include "WIFI_interface.h"
#include "HUART_interface.h"
#include <string.h>
#include "Delay_interface.h"




/*This static variable will hold the address for the initialized object of UART peripheral used for WIFI
 * If it is null (meaning uninitialized), then no function will be carried out
 * */
/*This static variable will hold the address for the initialized object of UART peripheral used for displaying output
 * If it is null (meaning uninitialized), then no function will be carried out
 * */
static UART_GPIO_t Static_UART_PERIPHERAL = {.BaseAddress = NULL};
static UART_GPIO_t Static_OUTPUT_PERIPHERAL = {.BaseAddress = NULL};


/*This static variable will hold the response*/
static volatile u8 static_u8Response=0;
/*This variable will contain the previous character before the one we are currently receiving*/
static volatile u8 static_u8PreviousChar=0;
/*This is the flag that will keep us sending and receiving until the end of the data*/
static volatile u8 static_u8ReceiveFlag=1;
/*This static variable will be used as a flag so that we know whether the data incoming should be saved inside a variable or not*/
static volatile u8 static_u8DataToArray=0;
/*This variable will be used to store incoming data*/
volatile u8 Global_u8DataReceivedArray[2048]={0};
/*This static variable will be used as iterator for array that will be used to save data*/
static volatile u16 static_u16DataArrayIterator=0;
/*This static variable will be used as a flag to prompt start receiving data inside the array after the desired char is received ':'*/
static volatile u8 static_u8StartSavingData=0;

/*This callback will send the received data to the screen*/
void callBackRX (void)
{
	/*If display is specified (address is not null), we are free to send char to them*/
	if (Static_OUTPUT_PERIPHERAL.BaseAddress!=NULL)
	{
		/*Send character that has been received*/
		HUART_u8SendAsync(Static_OUTPUT_PERIPHERAL, &static_u8Response, sizeof(static_u8Response));
	}

	/*If data to array flag is on, send data to flag too*/
	if (static_u8DataToArray==1)
	{
		if (static_u8StartSavingData)
		{
			if (static_u16DataArrayIterator<sizeof(Global_u8DataReceivedArray))
			{
				if (static_u8Response=='<')
				{
					static_u8StartSavingData=0;
				}
				if (static_u8Response!=' ' && static_u8Response!='\n'&& static_u8Response!='<')
				{
					Global_u8DataReceivedArray[static_u16DataArrayIterator]=static_u8Response;
					static_u16DataArrayIterator++;
				}
			}

			/*We check for end of receiving through the word "CLOSED", we will check for the last two chars of the word*/
			if (static_u8PreviousChar=='C' && static_u8Response=='L')
			{
				static_u8ReceiveFlag=0;
			}
			else
			{
				/*Save the last received character in the previous char to make comparison later for receiving flag*/
				static_u8PreviousChar=static_u8Response;
			}
		}
		if ((static_u8Response==':') | (static_u8Response=='>'))
		{
			static_u8StartSavingData=1;
		}

	}
	/*Do the check according to whether send flag is set or reset
	 * It case it is set, we check for CLOSED, otherwise we check for OK*/
	else
	{
		if ((static_u8PreviousChar=='O' && static_u8Response=='K'))
		{
			static_u8ReceiveFlag=0;
		}
		else
		{
			/*Save the last received character in the previous char to make comparison later for receiving flag*/
			static_u8PreviousChar=static_u8Response;
		}
	}
}


/*Description: This static function will be used to handle sending request and receiving its response
 * parameters: Data to send (u8*)
 * Return: void*/
static void WIFI_voidHandleRequest (u8* Copy_u8Request)
{
	/*Send data to WIFI peripheral*/
	HUART_u8SendAsync(Static_UART_PERIPHERAL, Copy_u8Request, strlen(Copy_u8Request));

	/*Enter the loop for sending and receiving data from UART*/
	while(static_u8ReceiveFlag)
	{
		/*Receive Response*/
		HUART_u8ReceiveAsync(Static_UART_PERIPHERAL, &static_u8Response, sizeof(static_u8Response));
	}
}


/*Description: This API will be used to initialize WIFI module on specified UART
 * parameters: UART peripheral used for WIFI (UART_GPIO_t)
 * Return: Error Status*/
u8 WIFI_u8Init (UART_GPIO_t UART_Peripheral)
{
	/*This local variable will hold error status for the current function*/
	u8 Local_u8Status = STATUS_NOK;
	/*This local variable will be sent to wifi module so that it will be reset*/
	u8 Local_u8Send[]="AT+RST\r\n";
	/*Initialization will be done by calling initialize function of HUART*/
	Local_u8Status= HUART_u8Init(UART_Peripheral, 115200, UART_STOP_BIT1, UART_PARITY_DISABLED);
	/*If initialization was successful, save uart address in the static variable*/
	if (Local_u8Status == STATUS_OK)
	{
		Static_UART_PERIPHERAL = UART_Peripheral;
		/*Send reset to the Module*/
		HUART_u8SendSync(Static_UART_PERIPHERAL, Local_u8Send, strlen(Local_u8Send), 1);
	}
	/*This delay is needed for initialization to be done*/
	delay_ms(10000);
	/*Return Status*/
	return Local_u8Status;
}

/*Description: This API will be used to set UART peripheral that will be used to display output
 * parameters: UART peripheral used for output (UART_GPIO_t)
 * Return: void*/
void WIFI_u8SetOutput (UART_GPIO_t Copy_UART_Peripheral)
{
	Static_OUTPUT_PERIPHERAL=Copy_UART_Peripheral;
}

/*Description: This API will be used to send AT command to the module
 * parameters: Desired Command
 * Return: Error Status*/
u8 WIFI_u8SendCommand (u8* Copy_u8DesiredCommand)
{
	/*This local variable will hold status of function*/
	u8 Local_u8Status = STATUS_NOK;

	/*Reset receive flag if it has not been initialized*/
	static_u8ReceiveFlag=1;

	/*If wifi peripheral has been initialized, proceed with the code*/
	if (Static_UART_PERIPHERAL.BaseAddress!=NULL)
	{
		/*Set callback function*/
		HUART_u8SetRXCallBack(callBackRX, Static_UART_PERIPHERAL.BaseAddress);
		/*Send data using static send request*/
		WIFI_voidHandleRequest(Copy_u8DesiredCommand);
		/*Return request as OK because the previous function contained while loop*/
		Local_u8Status=STATUS_OK;
	}
	/*Return status*/
	return Local_u8Status;
}

/*Description: This API will be used to connect to a specific WIFI access point
 * parameters: Access point name (u8*), password(u8*)
 * Return: Error Status*/
u8 WIFI_u8ConnectToAccessPoint (u8* Copy_u8SSID, u8* Copy_u8Password)
{
	/*This local variable will hold the status of the function*/
	u8 Local_u8Status = STATUS_NOK;
	/*These local arrays will hold the data that will be sent as command*/
	u8 Local_u8SendPart1[]="AT+CWJAP_CUR=\"";
	u8 Local_u8SendPart2[]="\",\"";
	u8 Local_u8SendPart3[]="\"\r\n";
	/*Reset receive flag if it has not been initialized*/
	static_u8ReceiveFlag=1;

	/*If wifi peripheral has been initialized, proceed with the code*/
	if (Static_UART_PERIPHERAL.BaseAddress!=NULL)
	{
		/*Set Callback function*/
		HUART_u8SetRXCallBack(callBackRX, Static_UART_PERIPHERAL.BaseAddress);

		HUART_u8SendSync(Static_UART_PERIPHERAL, Local_u8SendPart1, strlen(Local_u8SendPart1),1);
		/*Wait until we reach the point where we should send the name*/
		HUART_u8SendSync(Static_UART_PERIPHERAL, Copy_u8SSID, strlen(Copy_u8SSID),1);
		/*Send part between name and password*/
		HUART_u8SendSync(Static_UART_PERIPHERAL, Local_u8SendPart2, strlen(Local_u8SendPart2),1);
		/*Send Password*/
		HUART_u8SendSync(Static_UART_PERIPHERAL, Copy_u8Password, strlen(Copy_u8Password),1);
		/*Send last part of command*/
		HUART_u8SendSync(Static_UART_PERIPHERAL, Local_u8SendPart3, strlen(Local_u8SendPart3),1);

		/*Enter the loop for sending and receiving data from UART*/
		while(static_u8ReceiveFlag)
		{
			/*Receive Response*/
			HUART_u8ReceiveAsync(Static_UART_PERIPHERAL, &static_u8Response, sizeof(static_u8Response));
		}
		/*Since we reached here, set status as ok*/
		Local_u8Status=STATUS_OK;
	}
	/*Return status*/
	return Local_u8Status;
}


/*Description: This API will be used to receive data
 * parameters: Desired Command
 * Return: Error Status*/
u8 WIFI_u8ReceiveData (u8* Copy_u8DesiredCommand)
{
	/*This local variable will hold status of function*/
	u8 Local_u8Status = STATUS_NOK;
	/*This local array will hold the data that will be sent*/
	u8 Local_u8SendConnectionType[]="AT+CIPMUX=0\r\n";
	u8 Local_u8SendStartConnection[]="AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
	u8 Local_u8SendSize[]="AT+CIPSEND=90\r\n";

	/*Reinitialize array so that we recieve new data successfully*/
	memset(Global_u8DataReceivedArray,0,sizeof(Global_u8DataReceivedArray));

	/*Reinitialize other flags in case they were used in something else*/
	static_u16DataArrayIterator=0;
	static_u8StartSavingData=0;


	/*If wifi peripheral has been initialized, proceed with the code*/
	if (Static_UART_PERIPHERAL.BaseAddress!=NULL)
	{

		HUART_u8SetRXCallBack(callBackRX, Static_UART_PERIPHERAL.BaseAddress);
		/*Send first part to WIFI peripheral, which specifies the number of connections we will be using (which is 1)*/
		WIFI_u8SendCommand(Local_u8SendConnectionType);
		delay_ms(1000);

		/*Reset flag again and send following data*/
		//static_u8ReceiveFlag=1;
		/*Send second part to WIFI peripheral, which is to connect to specific server*/
		WIFI_u8SendCommand(Local_u8SendStartConnection);
		delay_ms(1000);

		/*Reset flag again and send following data*/
		//static_u8ReceiveFlag=1;
		/*Send third part to WIFI peripheral, which is to specify size of command*/
		WIFI_u8SendCommand(Local_u8SendSize);

		delay_ms(1000);

		/*Reset flag again and send following data*/
		//static_u8ReceiveFlag=1;
		/*Set data to array flag*/
		static_u8DataToArray=1;
		/*Send final part to WIFI peripheral, which is the request*/
		WIFI_u8SendCommand(Copy_u8DesiredCommand);
		/*Set data to array flag*/
		static_u8DataToArray=0;

		/*The last two chars in the buffer now contain C and L, so we need to make them equal to null*/
		Global_u8DataReceivedArray[static_u16DataArrayIterator-1]=0;
		Global_u8DataReceivedArray[static_u16DataArrayIterator-2]=0;

		/*Since we reached here, set status as ok*/
		Local_u8Status=STATUS_OK;
	}

	/*Return status*/
	return Local_u8Status;
}

/*Description: This API will be used to send data our server (This is a specific function for the server we are using)
 * parameters: Desired Command (u8), Size to be sent (u16)
 * Return: Error Status*/
u8 WIFI_u8SendCommandToServer (u8* Copy_u8commandNumber, u16 Copy_u16Size)
{

	/*This local variable will hold status of function*/
	u8 Local_u8Status = STATUS_NOK;
	/*This local array will hold the data that will be sent*/
	u8 Local_u8SendConnectionType[]="AT+CIPMUX=0\r\n";
	u8 Local_u8SendStartConnection[]="AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
	u8 Local_u8SendSize[]="AT+CIPSEND=99\r\n";
	//u8 Local_u8SendSize[]="AT+CIPSEND=118\r\n";
	/*These local variables will contain the command that we want to send to our server. It is divided as two parts so that we can concatenate command to it*/
	u8 Local_u8SendRequest[150]="GET https://api.thingspeak.com/update?api_key=1H61N46CEZA65MTJ&field1=";
	u8 Local_u8SendRequest2[]="\r\nHost:api.thingspeak.com\r\n\r\n\r\n\r\n\r\n";
	/*This Local variable will be used as an iterator for concatenating string, we will use it so that we can avoid garbage better*/
	u16 Local_u16Iterator=0;
	/*This Local Variable will contain the command after being converted to string*/
	//u8 Local_u8CommandNumber[12]={0};

	/*Convert command to string*/
	//sprintf(Local_u8CommandNumber, "%x", Copy_u8commandNumber);

	/*Concatenate command and strings together*/
	for (Local_u16Iterator=0; Local_u16Iterator<Copy_u16Size;Local_u16Iterator++)
	{
		/*We will append each character to the string, the length of the original string before the appended one is 70*/
		Local_u8SendRequest[Local_u16Iterator+70]=Copy_u8commandNumber[Local_u16Iterator];
	}
	strcat(Local_u8SendRequest,Local_u8SendRequest2);
	//u8 Local_u8SendRequest[]="GET https://api.thingspeak.com/channels/1082594/fields/1/last.txt?api_key=GL3M7JAK48BR8RRA\r\nHost:api.thingspeak.com\r\n\r\n\r\n\r\n\r\n";

	/*If wifi peripheral has been initialized, proceed with the code*/
	if (Static_UART_PERIPHERAL.BaseAddress!=NULL)
	{

		HUART_u8SetRXCallBack(callBackRX, Static_UART_PERIPHERAL.BaseAddress);
		/*Send first part to WIFI peripheral, which specifies the number of connections we will be using (which is 1)*/
		WIFI_u8SendCommand(Local_u8SendConnectionType);
		delay_ms(1000);

		/*Reset flag again and send following data*/
		//static_u8ReceiveFlag=1;
		/*Send second part to WIFI peripheral, which is to connect to specific server*/
		WIFI_u8SendCommand(Local_u8SendStartConnection);
		delay_ms(1000);

		/*Reset flag again and send following data*/
		//static_u8ReceiveFlag=1;
		/*Send third part to WIFI peripheral, which is to specify size of command*/
		WIFI_u8SendCommand(Local_u8SendSize);

		delay_ms(1000);

		/*Set data to array flag*/
		static_u8DataToArray=1;
		/*Send final part to WIFI peripheral, which is the request*/
		WIFI_u8SendCommand(Local_u8SendRequest);
		/*Set data to array flag*/
		static_u8DataToArray=0;

		/*The last two chars in the buffer now contain C and L, so we need to make them equal to null*/
		Global_u8DataReceivedArray[static_u16DataArrayIterator-1]=0;
		Global_u8DataReceivedArray[static_u16DataArrayIterator-2]=0;

		/*Since we reached here, set status as ok*/
		Local_u8Status=STATUS_OK;
	}
	return Local_u8Status;

}

/*Description: This API will be used to receive command passed to server
 * parameters: Pointer to buffer where command will be received
 * Return: Error Status*/
u8 WIFI_u8ReceiveCommand (u8* Copy_u8Buffer)
{
	/*This local variable will hold status of function*/
	u8 Local_u8Status = STATUS_NOK;
	/*This local array will hold the data that will be sent*/
	u8 Local_u8SendConnectionType[]="AT+CIPMUX=0\r\n";
	u8 Local_u8SendStartConnection[]="AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
	u8 Local_u8SendSize[]="AT+CIPSEND=99\r\n";
	u8 Local_u8SendRequest[]="GET https://api.thingspeak.com/channels/1082594/fields/1/last.txt?api_key=GL3M7JAK48BR8RRA\r\nHost:api.thingspeak.com\r\n\r\n\r\n\r\n\r\n";
	/*This is an iterator which will be used to transfer data to buffer*/
	u16 Local_u16Iterator=0;
	/*This is a flag which we will use to make transferring of data to buffer*/
	u8 Local_u8Flag=1;

	/*Reinitialize array so that we recieve new data successfully*/
	memset(Global_u8DataReceivedArray,0,sizeof(Global_u8DataReceivedArray));

	/*Reinitialize other flags in case they were used in something else*/
	static_u16DataArrayIterator=0;
	static_u8StartSavingData=0;

	/*If wifi peripheral has been initialized, proceed with the code*/
	if (Static_UART_PERIPHERAL.BaseAddress!=NULL)
	{

		HUART_u8SetRXCallBack(callBackRX, Static_UART_PERIPHERAL.BaseAddress);
		/*Send first part to WIFI peripheral, which specifies the number of connections we will be using (which is 1)*/
		WIFI_u8SendCommand(Local_u8SendConnectionType);
		delay_ms(1000);

		/*Reset flag again and send following data*/
		//static_u8ReceiveFlag=1;
		/*Send second part to WIFI peripheral, which is to connect to specific server*/
		WIFI_u8SendCommand(Local_u8SendStartConnection);
		delay_ms(1000);

		/*Reset flag again and send following data*/
		//static_u8ReceiveFlag=1;
		/*Send third part to WIFI peripheral, which is to specify size of command*/
		WIFI_u8SendCommand(Local_u8SendSize);

		delay_ms(1000);

		/*Set data to array flag*/
		static_u8DataToArray=1;
		/*Send final part to WIFI peripheral, which is the request*/
		WIFI_u8SendCommand(Local_u8SendRequest);
		/*Set data to array flag*/
		static_u8DataToArray=0;

		/*The last two chars in the buffer now contain C and L, so we need to make them equal to null*/
		Global_u8DataReceivedArray[static_u16DataArrayIterator-1]=0;
		Global_u8DataReceivedArray[static_u16DataArrayIterator-2]=0;

		/*Pass received chars to buffer*/
		while (Local_u8Flag==1)
		{
			/*Tranfer data from buffer here to buffer passed by user*/
			Copy_u8Buffer[Local_u16Iterator]=Global_u8DataReceivedArray[Local_u16Iterator];
			/*Increment iterator*/
			Local_u16Iterator++;
			/*Check whether the new character will be null or not, if it is null, then get out*/
			if (Global_u8DataReceivedArray[Local_u16Iterator]==0)
			{
				Local_u8Flag=0;
			}
		}
		/*Since we reached here, set status as ok*/
		Local_u8Status=STATUS_OK;
	}

	/*Return status*/
	return Local_u8Status;
}
