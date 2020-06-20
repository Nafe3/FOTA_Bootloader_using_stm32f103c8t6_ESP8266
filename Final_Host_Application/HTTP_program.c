#include<stdio.h>
#include<winsock2.h>

#include "HTTP_interface.h"

/*This object will be used during initialization*/
WSADATA wsa;
/*This object is the one we will use to create socket with*/
SOCKET s;
/*This object will hold the data of the server we want to connect to*/
struct sockaddr_in server;
/*This object will be used to get IP address of Host using its domain name*/
struct hostent *remoteHost;
/*This object will hold the IP address after conversion from domain name*/
struct in_addr addr;


/*This Local Variable will hold user desired command, it will be taken using scanf*/
u8* Local_u8UserCommand[4]={0};

/*Description: This function will be used to create socket and connect to the server
Parameters: Desired Server (u8*)
Return: Error Status (u8)*/
u8 HOST_voidConnectToServer(u8* Copy_u8Server)
{
    /*This local variable will hold the status*/
    u8 Local_u8Status=STATUS_NOK;
    /*This is a simple iterator which we will use in converting domain name to IP address*/
    u16 iterator=0;

    /*Initialise Winsock*/
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");


    /*Create socket*/
    /*Socket object takes these parameters (Source: https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket?redirectedfrom=MSDN)
    1) AF: Address Family (We use IPV4 which corresponds to AF_INET)
    2) Socket Type: We will use the type that works with TCP protocol which is SOCK_STREAM
    3) Protocol: We can leave it at 0 which means choose protocol automatically, or specity TCP (IPPROTO_TCP)*/
    if((s = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    printf("Socket created.\n");

    /*Get information about server using its domain name*/
    remoteHost=gethostbyname(Copy_u8Server);
    /*Get address of server through the information retrieved in the above step*/
    while (remoteHost->h_addr_list[iterator] != 0)
    {
        addr.s_addr = *(u_long *) remoteHost->h_addr_list[iterator++];
        //printf("IP Address #%d: %s\n", i, inet_ntoa(addr));
    }

    /*Fill out server data*/
    /*inet_addr function converts address from string representation to unsigned long representation which is used by default*/
    server.sin_addr.s_addr = inet_addr(inet_ntoa(addr));
    server.sin_family = AF_INET;
    server.sin_port = htons( 80 );

    /*Connect to remote server*/
    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
    }
    else
    {
        printf("Connected to http://%s\n", Copy_u8Server);
        Local_u8Status=STATUS_OK;
    }
    return Local_u8Status;
}


/*Description: This API will be used to send new command to the server
Parameters: New Command (u8*), Desired Size (u16)
Return: Error Status (u8)*/
u8 HOST_voidSendCommand (u8* Copy_u8UserCommand, u16 Copy_u16Size)
{
    /*This local variable will hold the status of the current function*/
    u8 Local_u8Status=STATUS_NOK;
    /*This local variable will hold the status of the connect to server function*/
    u8 Local_u8ConnectionStatus=STATUS_NOK;
    /*This Local variable will hold the final string that will be sent*/
    u8  Local_u8FinalCommand[78]={0};
    /*This variable holds the size of the data that we will receive as a response*/
    u16 recv_size;
    /*This variable will hold the server request*/
    u8 server_reply[2000];
    /*This variable will be used to hold data incoming from user to prevent getting garbage by mistake*/
    u8 Local_u8FinalData[2000]={0};
    /*This variable will be used as an iterator to fill up data variable with proper data*/
    u16 Local_u16Iterator=0;

    /*Fill up data array with proper data by receiving data from incoming buffer until we reach desired size passed*/
    for (Local_u16Iterator=0; Local_u16Iterator<Copy_u16Size; Local_u16Iterator++)
    {
        Local_u8FinalData[Local_u16Iterator]=Copy_u8UserCommand[Local_u16Iterator];
    }

    /*Connect to server*/
    Local_u8ConnectionStatus=HOST_voidConnectToServer(HTTP_SERVER_NAME);

    /*If connection to server was successful, proceed with sending the command*/
    if (Local_u8ConnectionStatus==STATUS_OK)
    {
//        /*Get Desired command from user*/
//        printf("Enter your desired command: ");
//        scanf("%s",Local_u8UserCommand);
//        printf("Command to be sent is %s\n", Local_u8UserCommand);

        /*Formulate command request in the request format*/
        strcpy(Local_u8FinalCommand,HTTP_SEND_NEW_COMMAND);
        strcat(Local_u8FinalCommand,Local_u8FinalData);
        strcat(Local_u8FinalCommand,HTTP_EOL);

        printf("Final Command is %s\n", Local_u8FinalCommand);
        /*Send new command*/
        if( send(s , Local_u8FinalCommand , strlen(Local_u8FinalCommand) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        puts("Data Sent\n");

        if((recv_size = recv(s , server_reply , 2000 , 0)) == SOCKET_ERROR)
        {
            puts("recv failed");
        }
        puts("Reply received\n");

        //Add a NULL terminating u8acter to make it a proper string before printing
        server_reply[recv_size] = '\0';
        puts(server_reply);

        /*After we are done with the code, close socket and unload library from RAM*/
        closesocket(s);
        WSACleanup();
    }
    return Local_u8Status;
}

/*Description: This API will be used to retrieve command from server
Parameters: Buffer to receive data (u8*)
Return: Error Status (u8)*/
u8 HOST_voidReceiveCommand (u8* Copy_u8Buffer)
{
    /*This local variable will hold the status of the current function*/
    u8 Local_u8Status=STATUS_NOK;
    /*This local variable will hold the status of the connect to server function*/
    u8 Local_u8ConnectionStatus=STATUS_NOK;
    /*This variable holds the size of the data that we will receive as a response*/
    u16 recv_size;
    /*This variable will hold the server reply*/
    u8 server_reply[2000]={0};
    /*This local Variable will hold request to receive current command*/
    u8 *Local_u8GetCurrentCommand="GET https://api.thingspeak.com/channels/1082594/fields/1/last.txt?api_key=GL3M7JAK48BR8RRA\r\n\r\n";
    /*This local variable will be used as iterator for passing reply to passed buffer*/
    u16 Local_u16Iterator=0;
    /*This local variable will be used as a flag for the while loop which will be used to pass the received buffer to passed buffer*/
    u8 Local_u8TransferFlag=1;


    /*Connect to server*/
    Local_u8ConnectionStatus=HOST_voidConnectToServer(HTTP_SERVER_NAME);

    /*If connection to server was successful, proceed with sending the command*/
    if (Local_u8ConnectionStatus==STATUS_OK)
    {

        /*Send new command*/
        if( send(s , Local_u8GetCurrentCommand , strlen(Local_u8GetCurrentCommand) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        puts("Data Sent\n");

        if((recv_size = recv(s , server_reply , 2000 , 0)) == SOCKET_ERROR)
        {
            puts("recv failed");
        }
        puts("Reply received\n");

        //Add a NULL terminating character to make it a proper string before printing
        server_reply[recv_size] = '\0';
        puts(server_reply);

        /*Pass local Buffer to passed buffer*/
        while(Local_u8TransferFlag==1)
        {
            /*If character inside buffer is not equal to one*/
            if (server_reply[Local_u16Iterator]!=0)
            {
                Copy_u8Buffer[Local_u16Iterator]=server_reply[Local_u16Iterator];
                Local_u16Iterator++;
            }
            /*If character is equal to null, stop transferring*/
            else
            {
                Local_u8TransferFlag=0;
            }

        }

        /*After we are done with the code, close socket and unload library from RAM*/
        closesocket(s);
        WSACleanup();
    }
    return Local_u8Status;
}

