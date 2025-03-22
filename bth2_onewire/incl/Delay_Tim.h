#ifndef DELAY_TIM_H
#define DELAY_TIM_H

#include "stm32f10x.h"

void TIM2_Config(void);
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);

#endif