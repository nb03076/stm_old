/*
 * TLV5638.h
 *
 *  Created on: Aug 12, 2022
 *      Author: kang
 */

#ifndef INC_TLV5638_H_
#define INC_TLV5638_H_

#include "stm32f4xx_hal.h"

#define TLV5638_CS_Port			GPIOA
#define TLV5638_CS 				GPIO_PIN_4

#define TLV5638_CS_Set 			HAL_GPIO_WritePin(TLV5638_CS_Port, TLV5638_CS,GPIO_PIN_SET)
#define TLV5638_CS_Reset 		HAL_GPIO_WritePin(TLV5638_CS_Port, TLV5638_CS,GPIO_PIN_RESET)

void TLV5638_Init(SPI_HandleTypeDef* hspi);
void TLV5638_Write_Data(uint16_t Data);
void TLV5638_Control_Reg(void);

/* voltage data PFP */
void TLV5638_DAC_A_Output_Voltage(float Voltage);
void TLV5638_DAC_B_Output_Voltage(float Voltage);
void TLV5638_DAC_A_AND_B_Output_Voltage(float VoltageA, float VoltageB);

/* 12bit DAC data PFP */
void TLV5638_DAC_A_Output(uint16_t dat);
void TLV5638_DAC_B_Output(uint16_t dat);
void TLV5638_DAC_A_AND_B_Output(uint16_t datA, uint16_t datB);

#endif /* INC_TLV5638_H_ */
