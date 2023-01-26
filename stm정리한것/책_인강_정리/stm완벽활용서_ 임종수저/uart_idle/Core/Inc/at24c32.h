/*
 * at24c32.h
 *
 *  Created on: 2022. 9. 16.
 *      Author: kang
 */

#ifndef INC_AT24C32_H_
#define INC_AT24C32_H_

#include "stm32f4xx_hal.h"

#define AT24_DEVICE_ADDR 0xA0
#define AT24C32_MAX_ADDR 4096

#if 0
at24c04
16byte per page / max page = 32
4kb

at24c32
32byte per page / max page = 128
32kb

#endif


void AT24_Init(I2C_HandleTypeDef *hi2c);

void AT24_writeByte(uint16_t addr ,uint8_t data);
uint8_t AT24_readByte(uint16_t addr);

#endif /* INC_AT24C32_H_ */
