
#include "STD_TYPES.h"
#include "RCC.h"
#include "GPIO.h"

#include "HUART_interface.h"
#include <diag/Trace.h> // trace_printf
#include "string.h"
#include "Delay_interface.h"



#define Alarm_Switch	0U
#define Inc_Switch		1U
#define Dec_Switch		2U
#define LED1			0U


u8 State; 						/*Global variable to store     switch state in it*/
u8 G_Switch_State_Inc_Switch;  	/*Global variable to store Inc_switch state in it*/
u8 G_Switch_State_Dec_Switch;	/*Global variable to store Dec_switch state in it*/
Switch_t Switches[3];
LED_t G_Led;
LCD_t   LCD1;

u8 G_Counter=0;	/*counter to be incremented or decremented by pressing switches*/

TaskInterface_t switch_task;
TaskInterface_t app_task;
TaskInterface_t lcd_task;
TaskInterface_t uart_send_task;
TaskInterface_t uart_recieve_task;

u8* UART_Buffer = NULL;

//extern void Switch_Task(void); //SO2AAAAAAAAAAAAAAAAAAAAL
//application should be written like this

/*TX callback function */
void txDone (void)
{
	trace_printf("Transmission done\n");
}
/*RX Callback Function*/
void rxDone(void)
{
	trace_printf("Receiving Done\n");
}

void LCD_CB(void)
{
	trace_printf("LCD callback\n");
}

void APPRunnable(void)
{
//	HCLCD_write("Hello!", 6);
	State = HSwitch_getSwitchState(Alarm_Switch);
	if(State == Switches[Alarm_Switch].Pressed_State)
	{
		HLED_SetLedOFF(LED1); //Turn LED ON
	}
	else
		HLED_SetLedOn(LED1);
}

void UART_Send_Task(void)
{
	G_Switch_State_Inc_Switch = HSwitch_getSwitchState(Inc_Switch);
	G_Switch_State_Dec_Switch = HSwitch_getSwitchState(Dec_Switch);
	if(G_Switch_State_Inc_Switch == Switches[Inc_Switch].Pressed_State)
	{
		G_Counter++;
	}
	if(G_Switch_State_Inc_Switch == Switches[Inc_Switch].Pressed_State)
	{
		if(G_Counter!=0)
		{
			G_Counter++;
		}
	}
 	HUART_u8Send(&G_Counter,1);/*error*/
 	UART_Buffer = &G_Counter;

	HUART_u8Receive(UART_Buffer,1);

	//HCLCD_write(UART_Buffer, (u8)strlen(UART_Buffer));
	//HCLCD_clear();
	//delay_ms(20000);
	HCLCD_write(UART_Buffer, 1);

}
void UART_Recieve_Task(void)
{
}

void main(void)
{
	//sysclock
	//init modules
	//start os

	//RCC_voidSelectSysClk(HSE);
	//RCC_voidEnablePeripheral(APB2,RCC_APB2ENR_IOPAEN,ON);
	//RCC_voidEnablePeripheral(APB2,RCC_APB2ENR_IOPBEN,ON);
	//RCC_voidEnablePeripheral(APB2,RCC_APB2ENR_IOPCEN,ON);

	RCC_EnableAPB2Peripheral(IOPA);
	RCC_EnableAPB2Peripheral(IOPB);
	RCC_EnableAPB2Peripheral(IOPC);

	HUART_u8SetTXCallBack(txDone);
	HUART_u8SetRXCallBack(rxDone);

	Switches[Alarm_Switch].Switch_Port      = PORTB;
	Switches[Alarm_Switch].Switch_Pin.pin   = GPIO_PIN_11;
	Switches[Alarm_Switch].Switch_Pin.mode  = GPIO_MODE_INPUT_PULLUP_PULLDOWN;
	Switches[Alarm_Switch].Switch_Pin.speed = GPIO_INPUT_MODE_RESET_STATE;
	Switches[Alarm_Switch].Pressed_State    = 0;

	Switches[Inc_Switch].Switch_Port      = PORTB;
	Switches[Inc_Switch].Switch_Pin.pin   = GPIO_PIN_10;
	Switches[Inc_Switch].Switch_Pin.mode  = GPIO_MODE_INPUT_PULLUP_PULLDOWN;
	Switches[Inc_Switch].Switch_Pin.speed = GPIO_INPUT_MODE_RESET_STATE;
	Switches[Inc_Switch].Pressed_State    = 0;

	Switches[Dec_Switch].Switch_Port      = PORTB;
	Switches[Dec_Switch].Switch_Pin.pin   = GPIO_PIN_1;
	Switches[Dec_Switch].Switch_Pin.mode  = GPIO_MODE_INPUT_PULLUP_PULLDOWN;
	Switches[Dec_Switch].Switch_Pin.speed = GPIO_INPUT_MODE_RESET_STATE;
	Switches[Dec_Switch].Pressed_State    = 0;

	G_Led.LED_Port = PORTC;
	G_Led.LED_Pin.pin   = GPIO_PIN_13;
	G_Led.LED_Pin.mode  = GPIO_MODE_OUTPUT_PUSH_PULL;
	G_Led.LED_Pin.speed = GPIO_OUTPUT_SPEED_10MHz;


	LCD1.d0_Port = PORTA;
	LCD1.d1_Port = PORTA;
	LCD1.d2_Port = PORTA;
	LCD1.d3_Port = PORTA;
	LCD1.d4_Port = PORTA;
	LCD1.d5_Port = PORTA;
	LCD1.d6_Port = PORTA;
	LCD1.d7_Port = PORTA;
	LCD1.rs_Port = PORTA;
	LCD1.rw_Port = PORTB;
	LCD1.e_Port  = PORTB;

	LCD1.d0.pin = GPIO_PIN_0;
	LCD1.d1.pin = GPIO_PIN_1;
	LCD1.d2.pin = GPIO_PIN_2;
	LCD1.d3.pin = GPIO_PIN_3;
	LCD1.d4.pin = GPIO_PIN_4;
	LCD1.d5.pin = GPIO_PIN_5;
	LCD1.d6.pin = GPIO_PIN_6;
	LCD1.d7.pin = GPIO_PIN_7;
	LCD1.rs.pin = GPIO_PIN_8;
	LCD1.rw.pin = GPIO_PIN_12;
	LCD1.e.pin  = GPIO_PIN_13;

	LCD1.d0.speed = GPIO_OUTPUT_SPEED_10MHz;
	LCD1.d1.speed = GPIO_OUTPUT_SPEED_10MHz;
	LCD1.d2.speed = GPIO_OUTPUT_SPEED_10MHz;
	LCD1.d3.speed = GPIO_OUTPUT_SPEED_10MHz;
	LCD1.d4.speed = GPIO_OUTPUT_SPEED_10MHz;
	LCD1.d5.speed = GPIO_OUTPUT_SPEED_10MHz;
	LCD1.d6.speed = GPIO_OUTPUT_SPEED_10MHz;
	LCD1.d7.speed = GPIO_OUTPUT_SPEED_10MHz;
	LCD1.rs.speed = GPIO_OUTPUT_SPEED_10MHz;
	LCD1.rw.speed = GPIO_OUTPUT_SPEED_10MHz;
	LCD1.e. speed = GPIO_OUTPUT_SPEED_10MHz;

	LCD1.d0.mode = GPIO_MODE_OUTPUT_PUSH_PULL;
	LCD1.d1.mode = GPIO_MODE_OUTPUT_PUSH_PULL;
	LCD1.d2.mode = GPIO_MODE_OUTPUT_PUSH_PULL;
	LCD1.d3.mode = GPIO_MODE_OUTPUT_PUSH_PULL;
	LCD1.d4.mode = GPIO_MODE_OUTPUT_PUSH_PULL;
	LCD1.d5.mode = GPIO_MODE_OUTPUT_PUSH_PULL;
	LCD1.d6.mode = GPIO_MODE_OUTPUT_PUSH_PULL;
	LCD1.d7.mode = GPIO_MODE_OUTPUT_PUSH_PULL;
	LCD1.rs.mode = GPIO_MODE_OUTPUT_PUSH_PULL;
	LCD1.rw.mode = GPIO_MODE_OUTPUT_PUSH_PULL;
	LCD1.e.mode  = GPIO_MODE_OUTPUT_PUSH_PULL;


	switch_task.Runnable = Switch_Task;
	switch_task.PeriodicTimeMS = 5;

	app_task.Runnable = APPRunnable;
	app_task.PeriodicTimeMS = 5;

	lcd_task.Runnable = LCD_Task;
	lcd_task.PeriodicTimeMS = 2;

	uart_send_task.Runnable = UART_Send_Task;
	uart_send_task.PeriodicTimeMS=1000;

	uart_recieve_task.Runnable = UART_Recieve_Task;
	uart_recieve_task.PeriodicTimeMS=1000;

	HSwitch_init(Switches);

	GPIO_Pin_Write(Switches[Alarm_Switch].Switch_Port,&Switches[Alarm_Switch].Switch_Pin,1);//enable pull-up

	HLED_init(G_Led);

	HCLCD_init(&LCD1);
	HCLCD_voidSetWriteDoneNotification(LCD_CB);

	//HCLCD_clear();
	//HCLCD_write("Hello!", 7);
	/*Initialize UART Peripheral with desired options*/
	HUART_u8Init(UART_USART1, UART_BAUDRATE_9600, UART_STOP_BIT2, UART_PARITY_EVEN);

	/*Enable Interrupts for the TX and RX*/
	HUART_u8EnableInterrupt(UART_INTERRUPT_TX_COMPLETE, UART_INTERRUPT_ENABLE);
	HUART_u8EnableInterrupt(UART_INTERRUPT_RX_NOT_EMPTY, UART_INTERRUPT_ENABLE);

	SCHEDULAR_Init();
	SCHEDULAR_Start();
}
