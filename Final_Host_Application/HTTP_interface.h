#ifndef HTTP_INTERFACE_H_INCLUDED
#define HTTP_INTERFACE_H_INCLUDED

/*Types*/
#define u8  unsigned char
#define u16 unsigned short int
#define u32 unsigned long int

#define s8  signed char
#define s16 signed short int
#define s32 signed long int

#define f32 float
#define f64 double
#define f96 long double

#define STATUS_OK			(u8)0
#define STATUS_NOK		(u8)1

#define NULL	((void *)0)
/*End of Types*/


/*This is the server where we should connect*/
#define         HTTP_SERVER_NAME        (u8*)"api.thingspeak.com"
/*This is the format of the request sent for new commands*/
#define         HTTP_SEND_NEW_COMMAND                    (u8*)"GET https://api.thingspeak.com/update?api_key=1H61N46CEZA65MTJ&field1="
#define         HTTP_SEND_NEW_COMMAND_RESPONSE_CHANNEL   (u8*)"GET https://api.thingspeak.com/update?api_key=PCF4VMCRFW340IZ8&field1="
/*This end of line must be added after the command or else the command will not be executed*/
#define         HTTP_EOL                (u8*)"\r\n\r\n"

/*This macro will activate debug mode (debug messages will be displayed on terminal)*/
#define         HTTP_DEBUG_MODE         0

/*Description: This API will be used to send new command to the server
Parameters: New Command (u8*), Desired Size (u16)
Return: Error Status (u8)*/
extern u8 HOST_voidSendCommand (u8* Copy_u8UserCommand, u16 Copy_u16Size);

/*Description: This API will be used to retrieve command from server
Parameters: Buffer to receive data (u8*)
Return: Error Status (u8)*/
extern u8 HOST_voidReceiveCommand (u8* Copy_u8Buffer);

/*Description: This API will be used to send clear responses channel or post something specific to it
Parameters: New Command (u8*), Desired Size (u16)
Return: Error Status (u8)*/
extern u8 HOST_voidSendCommandToResponses (u8* Copy_u8UserCommand, u16 Copy_u16Size);


#endif // HTTP_INTERFACE_H_INCLUDED
