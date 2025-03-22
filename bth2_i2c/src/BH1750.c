#include "BH1750.h"
uint16_t tick = Timeout;



/*====================================================================================================================
======================================================CONFIG==========================================================
====================================================================================================================*/
void I2C2_Config(void) {
    I2C_InitTypeDef I2C_InitStruct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);   

    // Cau hình I2C2
    I2C_InitStruct.I2C_ClockSpeed = 200000;  // 100kHz
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x0B;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C2, &I2C_InitStruct);
    I2C_Cmd(I2C2, ENABLE);
}

/*====================================================================================================================
======================================================BH1750==========================================================
====================================================================================================================*/
uint16_t BH1750_Read(uint8_t add) {
    uint8_t data_re[2] = {0, 0};
    uint16_t lux = 0;

    // Start I2C
    I2C_GenerateSTART(I2C2, ENABLE);
    while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))&&tick>0){tick--;};
		if (tick <= 0) return 0xffff;
		tick = Timeout;

    I2C_Send7bitAddress(I2C2, add, I2C_Direction_Receiver);
    while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))&&tick>0){tick--;};
		if (tick <= 0) return 0xffff;
		tick = Timeout;

    for (int i = 0; i < 2; i++) {
        while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))&&tick>0){tick--;};
				if (tick <= 0) return 0xffff;
				tick = Timeout;
        data_re[i] = I2C_ReceiveData(I2C2);
    }

    I2C_GenerateSTOP(I2C2, ENABLE);
    lux = (data_re[0] << 8) | data_re[1];
    lux = lux / 1.2;
		delay_ms(100);
    return lux;
}

void BH1750_Cmd_Write(uint8_t add ,uint8_t cmd) {
	
  I2C_GenerateSTART(I2C2, ENABLE);
  while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))&&tick > 0){tick--;};
	if (tick <= 0) return;
	tick = Timeout;
	
  I2C_Send7bitAddress(I2C2, add, I2C_Direction_Transmitter);
  while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))&&tick > 0){tick--;};
	if (tick <= 0) return;
	tick = Timeout;

  I2C_SendData(I2C2, cmd);
  while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&&tick > 0){tick--;};
	if (tick <= 0) return;
	tick = Timeout;
	
  I2C_GenerateSTOP(I2C2, ENABLE);
	delay_ms(100);
}

void BH1750_Start(uint8_t add) {
    BH1750_Cmd_Write(add ,BH1750_ON);    // Bat nguon
    BH1750_Cmd_Write(add ,BH1750_RESET); // Reset cam bien
    BH1750_Cmd_Write(add ,BH1750_CONT_H_RES_MODE);  // Mode
}



