/*
 * at24c32.c
 *
 *  Created on: 2022. 9. 16.
 *      Author: kang
 */

#include "at24c32.h"


I2C_HandleTypeDef *_at24_i2c;

void AT24_Init(I2C_HandleTypeDef *hi2c)
{
	_at24_i2c = hi2c;
}

void AT24_writeByte(uint16_t addr ,uint8_t data)
{
	HAL_I2C_Mem_Write(_at24_i2c, AT24_DEVICE_ADDR, addr, 2, &data, 1, 1000);
	HAL_Delay(5);
}


uint8_t AT24_readByte(uint16_t addr)
{
	uint8_t result;
	HAL_I2C_Mem_Read(_at24_i2c, AT24_DEVICE_ADDR, addr, 2, &result, 1, 1000);
	HAL_Delay(5);

	return result;
}

