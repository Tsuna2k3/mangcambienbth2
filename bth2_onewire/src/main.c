#include "stm32f10x.h"
#include "Delay_Tim.h"
#include "GPIO.h"
#include "DHT11.h"
#include "uart.h"
#include <stdio.h>

/*
UART1 PA9 <-> RX
			PA10 <-> TX
DHT11 PC14 <-> Data
*/


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
