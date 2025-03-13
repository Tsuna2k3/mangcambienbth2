#include "stm32f10x.h"
#include "Delay_Tim.h"
#include "DHT11.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include <stdio.h>

/*
UART1 PA9 <-> RX
			PA10 <-> TX
DHT11 PC14 <-> Data
*/
void UART1_config(void){
	
	USART_InitTypeDef USART_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  USART_InitStruct.USART_BaudRate = 9600; 
  USART_InitStruct.USART_WordLength = USART_WordLength_8b; 
  USART_InitStruct.USART_StopBits = USART_StopBits_1; 
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Tx; 
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
}

void UART_SendChar(char c){
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
}

void UART_SendString(char *str){
    while (*str){
        UART_SendChar(*str++);
    }
}

void GPIO_Config(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

float Temp = 1;
float Humi = 1;
DHT11_Name DHT11;
char Tempstring[8], Humistring[8];



int main(){
	
	SystemInit();
	DHT11_Init(&DHT11, GPIOC, GPIO_Pin_14);
	UART1_config();
	GPIO_Config();
	TIM2_Config();
	
	while(1){
		Temp = DHT11_ReadTemp(&DHT11);
		Humi = DHT11_ReadHumi(&DHT11);
		
		sprintf(Tempstring, "%.2fC", Temp);
		sprintf(Humistring, "%.2f", Humi);
		
		UART_SendString("Nhiet do: ");
		UART_SendString(Tempstring);
		UART_SendString("\n");
		UART_SendString("Do am: ");
		UART_SendString(Humistring);
		UART_SendString("\n");
		
		delay_ms(1000);
	}
}
