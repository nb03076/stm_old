/*
 * L298_DCmotor.h
 *
 *  Created on: Sep 7, 2022
 *      Author: kang
 */

#ifndef INC_L298_DCMOTOR_H_
#define INC_L298_DCMOTOR_H_

#include "stm32f4xx_hal.h"


typedef enum
{
	FORWARD = 0,
	BACKWARD = 1
}MOTOR_DIR;

void L298_Init(TIM_HandleTypeDef *htim,GPIO_TypeDef *ena_port, uint16_t ena_pin);
void L298_Enable();
void L298_Disable();
void L298_RotateMotor(MOTOR_DIR direction, float speed);


#endif /* INC_L298_DCMOTOR_H_ */
