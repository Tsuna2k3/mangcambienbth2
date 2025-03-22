#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"

void UART1_config(void);
void UART_SendChar(char c);
void UART_SendString(char *str);

#endif
