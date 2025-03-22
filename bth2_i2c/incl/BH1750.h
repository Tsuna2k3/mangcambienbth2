#ifndef _BH1750_H
#define _BH1750_H

#include "stm32f10x.h"
#include "delay_timer.h"


#define BH1750_POWER_DOWN        0x00

#define BH1750_ON                0x01  
#define BH1750_RESET             0x07 
#define BH1750_CONT_H_RES_MODE   0x10  


#define Timeout 50000

void I2C2_Config(void);
uint16_t BH1750_Read(uint8_t add);
void BH1750_Cmd_Write(uint8_t add ,uint8_t cmd);
void BH1750_Start(uint8_t add);

#endif