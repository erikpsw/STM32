#include "stm32f10x.h" 
#include "item.h"            
#include "func.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>

// 定义一个变量
uint32_t counter = 0;
uint32_t Sensor_counter = 0;


char str1[10];
char str[10];

void task_function0(void *pvParameters)
{


	GPIO_SetBits(GPIOC,GPIO_Pin_13);
    while(1)
    {
        // 在任务中增加变量的值
        counter++;
		
		sprintf(str, "%d", counter);
		// vTaskDelay(1000);
		// OLED_ShowStr(1,5,(unsigned char *)str,1);
		if(Serial_RxFlag==1){
			Serial_SendString("OK");
			if(strcmp(Serial_Received,"A")==0)
				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			OLED_ShowStr(25,5,(unsigned char *)Serial_Received,1);
			Serial_RxFlag=0;
		}
		
	}
}

void task_function1(void *pvParameters)
{
	while(1){
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==1){

			GPIO_ResetBits(GPIOB,GPIO_Pin_11);
			
		}
		else{
			GPIO_SetBits(GPIOC,GPIO_Pin_13);
			GPIO_SetBits(GPIOB,GPIO_Pin_11);
		}

	}
}




int main(void){

	I2C_Configuration();
	OLED_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//AFIO 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//C外设时钟控制函数
	Item_Init('A',GPIO_Pin_0,GPIO_Speed_50MHz,GPIO_Mode_Out_PP);//LED
	Item_Init('B',GPIO_Pin_11,GPIO_Speed_50MHz,GPIO_Mode_Out_PP);//Buzzer
	Item_Init('C',GPIO_Pin_13,GPIO_Speed_50MHz,GPIO_Mode_Out_PP);
	Item_Init('A',GPIO_Pin_5,GPIO_Speed_50MHz,GPIO_Mode_IPD);//Lightsensor
	Item_Init('A',GPIO_Pin_9,GPIO_Speed_50MHz,GPIO_Mode_AF_PP);
	Item_Init('A',GPIO_Pin_10,GPIO_Speed_50MHz,GPIO_Mode_IN_FLOATING);
	USART1_Init(9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,USART_Mode_Rx | USART_Mode_Tx,USART_HardwareFlowControl_None);


	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);//中断信号
	Exti_Init(EXTI_Line5,EXTI_Mode_Interrupt,EXTI_Trigger_Rising_Falling);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC模式
	Nvic_Init(EXTI9_5_IRQn,1,1);

	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
	OLED_Fill(0x00);
		
	OLED_ShowStr(1,1,"Count:",2);
	// Serial_SendByte(0x41);
	Serial_SendString("Hello_world");
	 // 创建定时器
	xTaskCreate(task_function0, "OLED_Screen", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    // xTaskCreate(task_function1, "Buzzer", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	vTaskStartScheduler();

	while(1){

	}

	}

void EXTI9_5_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line5)==1){
			delay_ms(20);
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			Sensor_counter ++;
			if(Sensor_counter%2==1)
				sprintf(str1, "%d", (Sensor_counter+1)/2);
			else
				sprintf(str1, "%d", Sensor_counter/2);
			OLED_ShowStr(50,1,(unsigned char *)str1,2);
		EXTI_ClearITPendingBit(EXTI_Line5);
		delay_ms(20);
	};
}




