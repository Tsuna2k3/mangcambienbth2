#include "DHT11.h"




static void DHT11_SetPinOut(DHT11_Name* DHT11)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = DHT11->Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DHT11->PORT, &GPIO_InitStruct);
}

static void DHT11_SetPinIn(DHT11_Name* DHT11)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = DHT11->Pin;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(DHT11->PORT, &GPIO_InitStruct);
}

uint16_t time = 0;

static void DHT11_Start(DHT11_Name* DHT11) {
	DHT11_SetPinOut(DHT11);
	GPIO_WriteBit(DHT11->PORT, DHT11->Pin, 0);
	delay_ms(20);
	GPIO_WriteBit(DHT11->PORT, DHT11->Pin, 1);
	DHT11_SetPinIn(DHT11);
	

	
	//Cho DHT11 phan hoi: DHT11 keo ve 0
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < 100){
		if (!GPIO_ReadInputDataBit(DHT11->PORT, DHT11->Pin)){
			break;
		}
	}
	time = TIM_GetCounter(TIM2);
	if (time < 5 || time > 60) {//neu vi pham -> gui lai
		return DHT11_Start(DHT11);
	}
	
	
	
	//Cho dht11 du muc thap khoang 80us va keo len muc cao
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < 120){
		if (GPIO_ReadInputDataBit(DHT11->PORT, DHT11->Pin)){
			break;
		}
	}
	time = TIM_GetCounter(TIM2);
	if (time <= 50 || time >= 100) {
		return DHT11_Start(DHT11);
	}
	
	//Cho dhdt11 du muc cao khoang 80us va keo xuong muc thap -> bat dau gui du lieu
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < 120){
		if (!GPIO_ReadInputDataBit(DHT11->PORT, DHT11->Pin)){
			break;
		}
	}
	time = TIM_GetCounter(TIM2);
	if (time <= 50 || time >= 100) {
		return DHT11_Start(DHT11);
	}
}


static uint8_t DHT11_Read(DHT11_Name* DHT11) {
	uint8_t data = 0;
  uint8_t i = 0;
	for (i = 0; i < 8; ++i) {
		//Bat dau truyen: tin hieu o muc thap khoang 50us va keo len muc cao
		TIM_SetCounter(TIM2, 0);
		while (TIM_GetCounter(TIM2) < 60) {
			if (GPIO_ReadInputDataBit(DHT11->PORT, DHT11->Pin)) {
				break;
			}
		}
		time = TIM_GetCounter(TIM2);
		if ((time > 70) || (time < 30)) {
			//while(1){};
			return 0xff;
		}

		//Do do dai muc cao cua tin hieu
		TIM_SetCounter(TIM2, 0);
		while (TIM_GetCounter(TIM2) < 90) {
		if (!GPIO_ReadInputDataBit(DHT11->PORT, DHT11->Pin)) {
			break;
			}
		}
		time = TIM_GetCounter(TIM2);
		if ((time >= 80) || (time <= 10)) {
			return 0xff;
		}
	
	
		data <<= 1;
		if (time > 50) {
			data |= 1;  
		}
	}
	return data; 
}

void DHT11_Init(DHT11_Name* DHT11,GPIO_TypeDef* DS_PORT, uint16_t DS_Pin)
{
	DHT11->PORT = DS_PORT;
	DHT11->Pin = DS_Pin;
}

float DHT11_ReadTemp(DHT11_Name* DHT11)
{
	uint8_t Temp1, Temp2;
	uint8_t Humi1, Humi2;
	uint8_t checkRC;
	uint16_t nhietdo16bit;
	uint16_t doam16bit;
	float Temp, Humi;
	DHT11_Start(DHT11);
	//delay_ms(1000);
	Humi1 = DHT11_Read(DHT11);
	Humi2 = DHT11_Read(DHT11);
	Temp1 = DHT11_Read(DHT11);
	Temp2 = DHT11_Read(DHT11);
	checkRC = DHT11_Read(DHT11);
	//nhietdo16bit = (Temp1) + (Temp2/10.0);
	doam16bit = Humi1;
	DHT11_SetPinOut(DHT11);
	GPIO_WriteBit(DHT11->PORT, DHT11->Pin, 1);
	if ((Humi1 + Humi2 + Temp1 + Temp2) == checkRC){
		DHT11->Temp = (float)((Temp1) + (Temp2/10.0));
		DHT11->Humi = (Humi1);
		return DHT11->Temp;
	}else return DHT11_ReadTemp(DHT11);	
}

float DHT11_ReadHumi(DHT11_Name* DHT11)
{
	return DHT11->Humi;
}