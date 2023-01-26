/*
 * L298_DCmotor.c
 *
 *  Created on: Sep 7, 2022
 *      Author: kang
 */

#if 0
IN1	IN2	동작방향	전류방향
1	0	forward	OUT1 > OUT2
0	1	reverse	OUT2 > OUT1

채널1 IN1 채널2 IN2
#endif

/* To do : In3,4도 만들자. APB2타이머도 지원가능하게 만들자 mbed Timer처럼 자동으로 생성가능하게 만들어보자 */

#include "L298_DCmotor.h"


static TIM_HandleTypeDef *_L298Tim;
static GPIO_TypeDef *_L298_ENA_PORT;
static uint16_t _L298_ENA_PIN;



void L298_Init(TIM_HandleTypeDef *htim,GPIO_TypeDef *ena_port, uint16_t ena_pin)
{
	_L298Tim = htim;
	_L298_ENA_PORT = ena_port;
	_L298_ENA_PIN = ena_pin;
}

void L298_Enable()
{
	HAL_GPIO_WritePin(_L298_ENA_PORT, _L298_ENA_PIN, GPIO_PIN_SET);
}

void L298_Disable()
{
	HAL_GPIO_WritePin(_L298_ENA_PORT, _L298_ENA_PIN, GPIO_PIN_RESET);
}

/* 현재 APB1 타이머 기준으로 작성하였음 ARR = 1000 - 1 로 두고 작성하였음 */
void L298_RotateMotor(MOTOR_DIR direction, float speed)
{
	uint32_t timerFreq, motor_speed;
	timerFreq = HAL_RCC_GetPCLK1Freq();

	HAL_TIM_PWM_Stop(_L298Tim, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(_L298Tim, TIM_CHANNEL_2);

	__HAL_TIM_SET_PRESCALER(_L298Tim,(timerFreq / 1000000) - 1); //1MHz
	__HAL_TIM_SET_AUTORELOAD(_L298Tim,1000-1); //1KHz
	__HAL_TIM_SET_COUNTER(_L298Tim,0);

	if(direction == FORWARD)
	{
		if((motor_speed = (1000 * speed) -1 ) >1000) motor_speed = 1000-1;
		__HAL_TIM_SET_COMPARE(_L298Tim,TIM_CHANNEL_2,motor_speed);
		HAL_TIM_PWM_Start(_L298Tim, TIM_CHANNEL_2);

	}
	else
	{
		if((motor_speed = (1000 * speed) -1 ) >1000) motor_speed = 1000-1;
		__HAL_TIM_SET_COMPARE(_L298Tim,TIM_CHANNEL_1,motor_speed);
		HAL_TIM_PWM_Start(_L298Tim, TIM_CHANNEL_1);

	}

}
