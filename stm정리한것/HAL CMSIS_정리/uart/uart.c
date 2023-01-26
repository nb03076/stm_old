WordLength 9bit / none parity 로 설정했을 경우의 api 분석은 하지않았음
방식은 8bit와 크게 다를거 없음

레지스터 : SR,DR,BRR,CR1,CR2,CR3,GTPR
CR1 : 일반적인것들 제어하는 레지스터
CR2 : usart,stop bit,lin 같은 것들
CR3 : dma, error, rs232

typedef struct
{
  uint32_t BaudRate;   - IntegerDivider = ((PCLKx) / (8 * (OVR8+1) * (huart->Init.BaudRate)))
                             - FractionalDivider = ((IntegerDivider - ((uint32_t) IntegerDivider)) * 8 * (OVR8+1)) + 0.5
  uint32_t WordLength;    데이터 길이 8/9
  uint32_t StopBits;        스탑 비트 수  
  uint32_t Parity;           패리티 설정
  uint32_t Mode;             TE / RE  
  uint32_t HwFlowCtl;        RS232 관련 설정         
  uint32_t OverSampling;     8 / 16
} UART_InitTypeDef;

typedef struct __UART_HandleTypeDef
{
  USART_TypeDef                 *Instance;       레지스터 위치
  UART_InitTypeDef              Init;            초기화 할 것들
  const uint8_t                 *pTxBuffPtr;     tx버퍼
  uint16_t                      TxXferSize;      사이즈
  __IO uint16_t                 TxXferCount;     api에서 사용할 버퍼 전송할 남은 횟수
  uint8_t                       *pRxBuffPtr;     rx버퍼
  uint16_t                      RxXferSize;      
  __IO uint16_t                 RxXferCount;     
  __IO HAL_UART_RxTypeTypeDef ReceptionType;     standard / idle
  DMA_HandleTypeDef             *hdmatx;         msp에서 link 해줌. dma 설정 안해주면 null
  DMA_HandleTypeDef             *hdmarx;         msp에서 link 해줌. dma 설정 안해주면 null
  HAL_LockTypeDef               Lock;            
  __IO HAL_UART_StateTypeDef    gState;          tx / 전반적인 상태
  __IO HAL_UART_StateTypeDef    RxState;         rx 상태
  __IO uint32_t                 ErrorCode;       

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  void (* TxHalfCpltCallback)(struct __UART_HandleTypeDef *huart);        /*!< UART Tx Half Complete Callback        */
  void (* TxCpltCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Tx Complete Callback             */
  void (* RxHalfCpltCallback)(struct __UART_HandleTypeDef *huart);        /*!< UART Rx Half Complete Callback        */
  void (* RxCpltCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Rx Complete Callback             */
  void (* ErrorCallback)(struct __UART_HandleTypeDef *huart);             /*!< UART Error Callback                   */
  void (* AbortCpltCallback)(struct __UART_HandleTypeDef *huart);         /*!< UART Abort Complete Callback          */
  void (* AbortTransmitCpltCallback)(struct __UART_HandleTypeDef *huart); /*!< UART Abort Transmit Complete Callback */
  void (* AbortReceiveCpltCallback)(struct __UART_HandleTypeDef *huart);  /*!< UART Abort Receive Complete Callback  */
  void (* WakeupCallback)(struct __UART_HandleTypeDef *huart);            /*!< UART Wakeup Callback                  */
  void (* RxEventCallback)(struct __UART_HandleTypeDef *huart, uint16_t Pos); /*!< UART Reception Event Callback     */

  void (* MspInitCallback)(struct __UART_HandleTypeDef *huart);           /*!< UART Msp Init callback                */
  void (* MspDeInitCallback)(struct __UART_HandleTypeDef *huart);         /*!< UART Msp DeInit callback              */
#endif  /* USE_HAL_UART_REGISTER_CALLBACKS */
} UART_HandleTypeDef;


매크로
__HAL_UART_GET_FLAG(__HANDLE__, __FLAG__)  UART_FLAG_XXX
__HAL_UART_ENABLE_IT(__HANDLE__, __INTERRUPT__) UART_IT_XXX
__HAL_UART_GET_IT_SOURCE(__HANDLE__, __IT__)
__HAL_UART_ENABLE(__HANDLE__)
__HAL_UART_DISABLE(__HANDLE__)


api분석
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);
msp 설정 > UE clear > InitTypeDef 설정 > UE set


HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout);
사이즈 카운트 huart에 설정 / 버퍼는 아님을 유의!
- 카운트가 0이 될때까지 반복
	TXE가 set될때 까지 기다림
	DR = 버퍼 데이터값
	count--
TC가 set될때까지 기다림


HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
사이즈 카운트 huart에 설정
- 카운트가 0이 될때까지 반복
	RXNE set될때 까지 기다림
	버퍼에다 DR값 저장
	count--


HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size);
사이즈 카운트 버퍼 huart에 저장 > TXEIE set


HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
사이즈 카운트 버퍼 huart에 저장 > 각종 인터럽트 set(에러 포함)


HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size);
사이즈 카운트 버퍼 huart에 저장
DMA 설정(인터럽트 포함. dma enable)
DMAT set


HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
사이즈 카운트 버퍼 huart에 저장
DMA 설정(인터럽트 포함. dma enable)
에러 인터럽트 set
DMAR set


HAL_StatusTypeDef HAL_UART_DMAPause(UART_HandleTypeDef *huart);
DMAT clear
DMAR clear(에러 인터럽트도 같이 clear)


HAL_StatusTypeDef HAL_UART_DMAResume(UART_HandleTypeDef *huart);
DMAT set
DMAR set(에러 인터럽트도 같이 set)


HAL_StatusTypeDef HAL_UART_DMAStop(UART_HandleTypeDef *huart);
- Uart DMA Tx 처리 중일때
	DMAT clear
	disable dma interrupt
	DMA_SxCR_EN clear
	TCIE/TXEIE clear
- Uart DMA Rx 처리 중일때
	DMAR clear
	disable dma interrupt
	DMA_SxCR_EN clear
	RXNEIE 에러 인터럽트 clear


/* abort 함수 */
HAL_StatusTypeDef HAL_UART_Abort(UART_HandleTypeDef *huart);
폴링모드
그냥 인터럽트 clear / dma request 중 일시 clear
카운트 = 0


HAL_StatusTypeDef HAL_UART_AbortTransmit(UART_HandleTypeDef *huart);
폴링모드
HAL_UART_Abort랑 거의 유사한데 tx중심으로


HAL_StatusTypeDef HAL_UART_AbortReceive(UART_HandleTypeDef *huart);
폴링모드
HAL_UART_Abort랑 거의 유사한데 rx중심으로


HAL_StatusTypeDef HAL_UART_Abort_IT(UART_HandleTypeDef *huart);
폴링모드랑 별 다를거 없음
dma때 HAL_DMA_Abort_IT 사용한다는 것 정도만 다른듯?
DMA_SxCR_EN이거 clear를 polling으로 하냐 인터럽트로 처리하냐 차이인듯...

HAL_StatusTypeDef HAL_UART_AbortTransmit_IT(UART_HandleTypeDef *huart);
폴링모드랑 별 다를거 없음


HAL_StatusTypeDef HAL_UART_AbortReceive_IT(UART_HandleTypeDef *huart);
폴링모드랑 별 다를거 없음


void HAL_UART_IRQHandler(UART_HandleTypeDef *huart)
tx부분은 핸들러 제일 뒷부분에 rx는 앞부분에 있다.