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
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define TXBUF_MAX		303			// 303 bytes
#define RXBUF_MAX		303			// 303 bytes
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
uint32_t UartRxDMANum = 0;
bool bUartTxDMACpltFlg = true,bUartDataUpdateFlg = false;

uint8_t UartTxDMA_Buf[TXBUF_MAX];
uint8_t UartRxDMA_Buf[RXBUF_MAX];

typedef struct UARTCircBufTag {
	uint8_t	RxBuf1[RXBUF_MAX];
	uint8_t	RxBuf2[RXBUF_MAX];
	int32_t	m_iHead,m_iTail,QueueSize;
	uint32_t OverFlowNum,UnderFlowNum;
} UARTCircBufTyp;

UARTCircBufTyp UARTBuf;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
// Circular Queue(Buffer) :
void	QueueClear(void);
int		QueueGetSize(void);
void QueueDisplayFun(void);
bool	QueuePutByte(uint8_t b);		// 1 byte 넣기
bool	QueueGetByte(uint8_t *pb);		// 1 byte 꺼내기
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */



  SET_BIT(huart1.Instance->CR3,USART_CR3_DMAT);
  SET_BIT(huart1.Instance->CR3,USART_CR3_DMAR);
  __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart1, UartRxDMA_Buf, RXBUF_MAX);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  /* DMA2_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PG13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	int32_t i = 0,j = 0;

	if (huart->Instance==USART1) {
		QueueClear();
		UartRxDMANum = RXBUF_MAX-huart->hdmarx->Instance->NDTR;  // UartRxDMA_Buf_pW is the amount of received data.
		for (i = 0; i<UartRxDMANum; i++) {
			QueuePutByte(UartRxDMA_Buf[i]);
		}
		QueueDisplayFun();
	}
}

// Queue를 초기화
void QueueClear() {
	UARTBuf.m_iHead = UARTBuf.m_iTail = 0;	// When Head_ptr=Tail_ptr, circular queue is empty.
	UARTBuf.OverFlowNum = UARTBuf.UnderFlowNum = 0;

	memset(UARTBuf.RxBuf1,0,TXBUF_MAX);
	memset(UARTBuf.RxBuf2,0,RXBUF_MAX);
}
// Queue에 들어 있는 자료 개수.
int QueueGetSize() {  // Head_Ptr is larger than Tail_ptr all the time.
	return (UARTBuf.m_iHead-UARTBuf.m_iTail+RXBUF_MAX)%RXBUF_MAX;
}
// Queue에 1 byte 넣음.
// When Head_ptr+1=Tail_ptr, circular queue is full. The maximum queue size is
// BUFF_SIZE-1. so when lower than (BUFF_SIZE-1), new data can be put.
bool QueuePutByte(uint8_t b) {
	if (QueueGetSize()==(RXBUF_MAX-1)) {
		UARTBuf.OverFlowNum++;
		return false;
	}
	// For using "0" index, buff[m_iHead++] is used instead of buff[++m_iHead].
	UARTBuf.RxBuf1[UARTBuf.m_iHead++] = b;
	UARTBuf.m_iHead %= RXBUF_MAX;	// Thanks to "%" operator, Head_ptr can be circular.
	return true;
}
// Queue에서 1 byte 꺼냄.
bool QueueGetByte(uint8_t* pb) {
	if (QueueGetSize()==0) {
		UARTBuf.UnderFlowNum++;
		return false;
	}

	*pb = UARTBuf.RxBuf1[UARTBuf.m_iTail++];
	UARTBuf.m_iTail %= RXBUF_MAX;	// Thanks to "%" operator, Head_ptr can be circular.
	return true;
}

void QueueDisplayFun(void) {
	int InitFlag = 0,i,j,k,QueueSize;
	for (j = 0; j<RXBUF_MAX; j++) {
		if (InitFlag==0) {
			for (i = UARTBuf.m_iHead; i>0; i--,j++) {
				UARTBuf.RxBuf2[j] = UARTBuf.RxBuf1[i-1];
			}
			InitFlag = 1;
		}
		k = (RXBUF_MAX-1)-(j-UARTBuf.m_iHead);
		UARTBuf.RxBuf2[j] = UARTBuf.RxBuf1[k];
	}
	QueueSize = QueueGetSize();
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
