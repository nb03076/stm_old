/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
void Channel1Func(void);
void Channel2Func(void);
void Channel3Func(void);
void Channel4Func(void);
void Channel5Func(void);
void Channel6Func(void);

void (*fptr[6])(void) = {
	Channel1Func, 			// 0
	Channel2Func, 			// 1
	Channel3Func, 			// 2
	Channel4Func, 			// 3
	Channel5Func, 			// 4
	Channel6Func, 			// 5
};

int16_t Ra_Con(char* Cmd);
unsigned char hextoint(unsigned char *hex);
void Report500msForPC(uint16_t RetNum, uint8_t *RxInd);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
__IO ITStatus Uart3TxReady = RESET;
__IO ITStatus Uart3RxReady = RESET;

uint8_t UART3RxBuf[17] = {0,0,0,0,0,0,0,0,}, UART3RxBuf1 = 0;
uint8_t RcvData[7] = { 0, };
int32_t gCntRx1 = 0;
bool bFromPCCmdFailCplt = true;
bool gCmdRX1Info = false;

uint16_t SinDat[100]={1000,1063,1125,1187,1249,1309,1368,1426,1482,1536
					,1588,1637,1685,1729,1771,1809,1844,1876,1905,1930
					,1951,1969,1982,1992,1998,2000,1998,1992,1982,1969
					,1951,1930,1905,1876,1844,1809,1771,1729,1685,1637
					,1588,1536,1482,1426,1368,1309,1249,1187,1125,1063
					,1000,937 ,875 ,813 ,751 ,691 ,632 ,574 ,518 ,464
					,412 ,363	,315 ,271 ,229 ,191 ,156 ,124 ,95  ,70
					,49  ,31  ,18  ,8   ,2   ,0   ,2   ,8   ,18  ,31
					,49  ,70  ,95  ,124 ,156 ,191 ,229 ,271 ,315 ,363
					,412 ,464 ,518 ,574 ,632 ,691 ,751 ,813 ,875 ,937};
int32_t gi=0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	int16_t i=0, RetNum = 0;
		uint8_t RxInd = 0xFF;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_UART_Receive_IT(&huart3,&UART3RxBuf1,1);

	  		if (Uart3RxReady==SET) {
	  			if (UART3RxBuf1==0x73) {
	  				Report500msForPC(RetNum,&RxInd);
	  			} else {

	  				if ((UART3RxBuf1=='q')||(gCmdRX1Info==true)) {
	  					gCmdRX1Info = true;
	  					UART3RxBuf[gCntRx1] = UART3RxBuf1;
	  					Uart3RxReady = RESET;
	  					if (gCntRx1==16) {
	  						gCntRx1 = 0;
	  						RetNum = Ra_Con((char*)UART3RxBuf);
	  						if (RetNum==-1) {
	  							bFromPCCmdFailCplt = false; // There is something wrong with received data.
	  						} else {
	  							bFromPCCmdFailCplt = true;
	  							RxInd = RcvData[0];
	  							fptr[RxInd]();
	  						}
	  						for (i = 0; i<18; i++) {
	  							UART3RxBuf[i] = 0;
	  						}
	  						for (i = 0; i<8; i++) {
	  							RcvData[i] = 0;
	  						}
	  						gCmdRX1Info = false;
	  					} else {
	  						gCntRx1++;
	  					}
	  					UART3RxBuf1 = 0;
	  				}

	  			}
	  		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PG13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void Report500msForPC(uint16_t RetNum, uint8_t *RxInd) {
	uint16_t i = 0, Tmp = 0, YearMon = 0, DayHour = 0, MinuteSecond = 0;
	uint8_t ch = 0, STX = 0x2, ETX=0x3, ChkSum=0;
		HAL_UART_Transmit(&huart3, (uint8_t*)&STX, 1, 10);
		for (i = 0; i<1; i++) {
			switch (i) {
			case 0:
				Tmp = SinDat[gi];
				gi++;
				gi%=100;
				break;
			default:
				break;
			}
			ch = (uint8_t)Tmp;
			ChkSum +=ch;
			HAL_UART_Transmit(&huart3, (uint8_t*)&ch, 1, 10);
			ch = Tmp>>8;
			ChkSum +=ch;
			HAL_UART_Transmit(&huart3, (uint8_t*)&ch, 1, 10);
		}
		HAL_UART_Transmit(&huart3, (uint8_t*)&ETX, 1, 10);
		HAL_UART_Transmit(&huart3, (uint8_t*)&ChkSum, 1, 10);
		Uart3RxReady = RESET;
		UART3RxBuf1 = 0;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance==USART3) {
		Uart3RxReady = SET;
	}
}

int16_t Ra_Con(char* Cmd) {
	int i, ChkSum = 0;
	unsigned char Tmp = 0;

	if ((Cmd[0]=='q')&&(Cmd[15]=='w')) {
		for (i = 1; i<15; i++)
			ChkSum += Cmd[i];
		Tmp = (unsigned char)ChkSum;
		if (Tmp==((unsigned char)Cmd[16])) {
			for (i = 0; i<7; i++) {
				RcvData[i] = hextoint((unsigned char *)(Cmd+2*i+1));
			}
		} else {
			return -1;
		}
		return RcvData[0];
	} else {
		return -1;
	}
}

unsigned char hextoint(unsigned char *hex) {
	unsigned char 	r = 0;
	unsigned char		d;
	int i = 0;

	for (i = 0; i<2; i++)	{
		d = *hex++;
		r <<= 4;
		if ((d>='0')&&(d<='9'))
			r += d-'0';
		else if ((d>='A')&&(d<='F'))
			r += d-'A'+10;
		else if ((d>='a')&&(d<='f'))
			r += d-'a'+10;
		else
			return -1;
	}
	return r;
}

// Channel 1 function :
void Channel1Func(void) {

}

// Channel 2 function :
void Channel2Func(void) {

}

// Channel 3 function :
void Channel3Func(void) {

}

// Channel 4 function :
void Channel4Func(void) {

}

// Channel 5 function :
void Channel5Func(void) {

}

// Channel 6 function :
void Channel6Func(void) {

}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
