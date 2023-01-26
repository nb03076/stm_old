double buffer 사용시 자동으로 circular 모드
double buffer 사용하면 한 메모리를 dma가 사용하고 있을동안
다른 메모리를 소프트웨어적으로 접근이 가능하게 됨

=================================================

임종수의 stm32 완벽활용서
ex) 48khz 샘플레이트 음악파일기준
48khz * 2(left/right) * 32(비트) = 3.072 MHz

48khz * 256 = 12.288Mhz

16개의 샘플을 보내고 싶은데 i2s의 경우 하나씩만 보내게 되니깐 속도가 느림
더블 버퍼링dma 사용하면 16개의 샘플단위로 전송이 가능해서 속도가 빨라짐

동작 방식은 dma 인터럽트를 이용해서 전송,수신이 끝날때 인터럽트 걸어서
처리하는 것. 책 p452 이미지 참고

==================================================

이미지 같은 큰 크기의 파일 처리할때 유용.. 데이터 처리 더 빨라짐

NDTR 값이 0으로 되면(데이터 전송 완료) 자동으로 다른 버퍼 가리킴
uint8_t rx_buf[2][18]
DMA_InitStruct.DMA_BufferSize = 18
DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)&rx_buf[0][0]
DMA_DoubleBufferModeConfig(DMA1_Stream1,(uint32_t)rx_buffer[1][0],DMA_Memory_0)
DMA_DoubleBufferModeCmd(DMA1_Stream1,ENABLE)



https://chowdera.com/2022/152/202206010154289710.html

ADC를 더블 버퍼링 사용안하고 사용하는법
uint32_t adcBuf[size * 2]
HAL_ADC_Start_DMA(&hadc1,adcBuf,size *2)


===================================================

프로그램 설명

HAL_UART_Receive_DMA(&huart1, (uint8_t*)ptr, RX_BUFLEN * 2);
DMA 설정 중복으로 일어나기는 하지만. 그외에 것들 설정해줌
HAL_UART_DMAPause(&huart1);
dmar clear
HAL_DMA_Abort(&hdma_usart1_rx);
dma en clear 그외 인터럽트 설정 해제..
hdma_usart1_rx.XferM1CpltCallback = M1Callback;
m1ar의 콜백함수 지정안해주면 m1설정안됨... 무조건 따로 지정해서 넣어줘야함
HAL_DMAEx_MultiBufferStart_IT(&hdma_usart1_rx, (uint32_t)&huart1.Instance->DR , (uint32_t)ptr, (uint32_t)(ptr+RX_BUFLEN), RX_BUFLEN);
기존 dma랑 별 다른건 없고 DBM / m1ar 추가로 설정해줌
__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
받아야할 시간에 데이터 못받으면 idle 인터럽트 걸리게 해놓음  
HAL_UART_DMAResume(&huart1);
dmar set 해줘서 동작하게끔 만듦


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		if(__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) == 1){
			__HAL_UART_CLEAR_FLAG(huart,UART_FLAG_IDLE);
			UartRxDMANum = RX_BUFLEN - huart->hdmarx->Instance->NDTR;
			return;
		}
		dmabufNum = 1; // 이번 더블 버퍼라서 idle 인터럽트 걸렸을때 어느 버퍼를 사용하고 있는지 알려주려고 사용
	}
}

void M1Callback(struct __DMA_HandleTypeDef * hdma)
{
	dmabufNum = 0;
}