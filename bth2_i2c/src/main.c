#include "stm32f10x.h"
#include "delay_timer.h"
#include "gpio.h"
#include "BH1750.h"
#include "uart.h"
#include <stdio.h>

#define ADD_BH1750 0x46

uint16_t lux = 0;
char luxstring[8];
int luxint = 0;

int main(){
	SystemInit();
	GPIO_Config();
	UART1_config();
	TIM2_Config();
	I2C2_Config();
	BH1750_Start(ADD_BH1750);
	while(1){
		lux = BH1750_Read(ADD_BH1750);
		luxint = lux;
		sprintf(luxstring, "%dLux", luxint);
		
		UART_SendString("Anh sang: ");
		UART_SendString(luxstring);
		UART_SendString("\n");
		
		delay_ms(1000);
	}
}