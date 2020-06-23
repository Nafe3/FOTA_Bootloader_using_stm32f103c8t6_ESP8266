#include "main.h"
#include "HTTP_interface.h"
#define WINDOWS_HOST

int main()
{

     /*----------------------------- Ask Menu implementation----------------------------------------*/
    printf("\n\n +==========================================+");
    printf("\n |           STM32F4 BootLoader v1           |");
    printf("\n +==========================================+\n");
            //Serial_Port_Configuration();
    printf("   INITIALIZING SERVER, Please Wait");
    HOST_voidSendCommand("EMPTY",5);
    HOST_voidSendCommandToResponses("EMPTY",5);
    delay(15000);
    printf("   INITIALIZATION Complete");
    printf("\n +==========================================+\n");

    while(1)
    {
#if 1
        printf("\n\n +==========================================+");
        printf("\n |                   Menu                   |");
        printf("\n +==========================================+\n");
#endif
        printf("\n\n   Which BL command do you want to send ??\n");

        printf("\n   Get Boot-loader Version        --> 1");
        printf("\n   Boot-loader Help               --> 2");
        printf("\n   Get Chip ID                    --> 3");
        printf("\n------------------------------------------");
        printf("\n   Jump to application            --> 4");
        printf("\n------------------------------------------");
        printf("\n   Flash Memory Sectors Erase     --> 5");
        printf("\n   Flash Memory Mass Erase        --> 6");
        printf("\n------------------------------------------");
        printf("\n   Flash New Application          --> 7");
        printf("\n------------------------------------------");
        printf("\n   Memory Read                    --> 8");
        printf("\n------------------------------------------");
        printf("\n   Enable  Read Protection        --> 9");
        printf("\n   Disable Read Protection        --> 10");
        printf("\n   Enable  Write Protection       --> 11");
        printf("\n   Disable Write Protection       --> 12");
        printf("\n   Get Read Protection Status     --> 13");
		printf("\n   Get Write Protection Status    --> 14");
        printf("\n------------------------------------------");
        printf("\n   System Reset                   --> 15");
		printf("\n   Existing Apps Details          --> 16");
        printf("\n------------------------------------------");
        printf("\n   MENU_EXIT                      --> 0");

        printf("\n\n   Type the command code here : ");

        uint32_t command_code;
        scanf(" %d",&command_code);

        decode_menu_command_code(command_code);
        printf("\n   Waiting for server to be ready again\n");
        delay(15000);
        HOST_voidSendCommand("EMPTY",5);
        HOST_voidSendCommandToResponses("EMPTY",5);
        delay(15000);


#if 0
        printf("\n\n   Do you want to continue(y/n) ?:");
        uint8_t proceed = 0;
        scanf(" %c",&proceed);
        proceed -= 'y';
        if ( proceed)
        {
            printf("\n  ****** Thank you ! Exiting ******\n");
            break;
        }
#endif
        printf("\n\n   Press any key to continue  :");
        uint8_t ch = getch();
        purge_serial_port();
   }


}
