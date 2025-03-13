#ifndef DHT11_H
#define DHT11_H

#include "stm32f10x.h"
#include "Delay_Tim.h"

typedef struct                                 
{   
    GPIO_TypeDef* PORT;
    uint16_t Pin;
    float Temp;
    float Humi;
} DHT11_Name;

void DHT11_Init(DHT11_Name* DHT11, GPIO_TypeDef* DS_PORT, uint16_t DS_Pin);
float DHT11_ReadTemp(DHT11_Name* DHT11);
float DHT11_ReadHumi(DHT11_Name* DHT11);
float DHT11_ReadTemp(DHT11_Name* DHT11);

#endif