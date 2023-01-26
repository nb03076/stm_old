초기화 함수
uart를 예로 들자면 
0. MX_DMA_Init함수에서 이미 해당 스트림 인터럽트 설정해둔 상태임
1. gpio 클럭 인가 + dma / 인터럽트 설정
2. uart 설정
dma 는 CR에서 설정 해주는데 초기화함수에서는 M0AR,PAR,NDTR 같은거는 설정안해둠
	hdma_usart1_rx.Instance = DMA2_Stream2; 
    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

	// 이거는 dma handle typedef의 parent를 uart로 두는거고
	// uart의 dmarx를 dma로 두는거임. 서로 연결해서 다루기 쉽게 하려고 그런거
    __HAL_LINKDMA(huart,hdmarx,hdma_usart1_rx);


레지스터 : SR(인터럽트 status) CR(인터럽트 플래그 clear) SxCR(스트림 설정)
SxNDTR(전송할 버퍼수) SxMAR, SxPAR SxFCR(fifo 설정)


spi로 하나씩 전송시 전송할때만 clk 파형이 생김.전송 끝나면 사라짐
여러개 전송할때동안 clk파형이 계속 생기도록 하고싶다면 dma 사용
이유 : 데이터를 읽고 보내고 쓰는데 시간이 걸려서 그럼(D, S버스)

tc / ht / te(bus error) / dme
DMA_SxCR : CR
NDTR : 남아있는 count? 이 레지스터 값만큼 dma 수행해야함
SxFCR : fifo 설정

typedef struct
{
  uint32_t Channel;						dma 채널 number      
  uint32_t Direction;         			m2p p2m m2m
  uint32_t PeriphInc;           		peripheral 자동으로 카운트 증가 여부
  uint32_t MemInc;              		memory 자동으로 카운트 증가 여부
  uint32_t PeriphDataAlignment;			1/2/4바이트 정렬
  uint32_t MemDataAlignment;			1/2/4바이트 정렬
  uint32_t Mode;						circular 여부
  uint32_t Priority;					우선순위
  uint32_t FIFOMode;					fifo 사용 여부
  uint32_t FIFOThreshold;				1/4 2/4 3/4 4/4 
  uint32_t MemBurst;					버스트모드.. 사용하면 속도 증가된다는데 잘 사용안해서 모르겠음
  uint32_t PeriphBurst;
}DMA_InitTypeDef;

typedef struct __DMA_HandleTypeDef
{
  DMA_Stream_TypeDef         *Instance;
  DMA_InitTypeDef            Init;
  HAL_LockTypeDef            Lock;
  __IO HAL_DMA_StateTypeDef  State;
  void                       *Parent; uart에서 dma 사용했다 치면 uart가 parent가 되는것...
  void                       (* XferCpltCallback)( struct __DMA_HandleTypeDef * hdma);         /*!< DMA transfer complete callback         */
  void                       (* XferHalfCpltCallback)( struct __DMA_HandleTypeDef * hdma);     /*!< DMA Half transfer complete callback    */
  void                       (* XferM1CpltCallback)( struct __DMA_HandleTypeDef * hdma);       /*!< DMA transfer complete Memory1 callback */
  void                       (* XferM1HalfCpltCallback)( struct __DMA_HandleTypeDef * hdma);   /*!< DMA transfer Half complete Memory1 callback */
  void                       (* XferErrorCallback)( struct __DMA_HandleTypeDef * hdma);        /*!< DMA transfer error callback            */
  void                       (* XferAbortCallback)( struct __DMA_HandleTypeDef * hdma);        /*!< DMA transfer Abort callback            */  
  __IO uint32_t              ErrorCode;                                                        /*!< DMA Error code                          */
  uint32_t                   StreamBaseAddress;                                                /*!< DMA Stream Base Address                */
  uint32_t                   StreamIndex;                                                      /*!< DMA Stream Index                       */
}DMA_HandleTypeDef;


__HAL_DMA_GET_COUNTER  NDTR 값확인


HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma); 
초기화하는 코드인데 DMA_CalcBaseAndBitshift이 함수에서 채널값 이용해서 StreamIndex에 값넣음


HAL_StatusTypeDef HAL_DMA_Start (DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
NDTR / direction 설정후 DMA_SxCR_EN set


HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
NDTR / direction 설정후 인터럽트 설정 / DMA_SxCR_EN set


HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *hdma);
인터럽트 clear > DMA_SxCR_EN clear


HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *hdma);
그냥 DMA_SxCR_EN clear


static void DMA_SetConfig(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
이거는 전송관련된 함수에서 자주 사용함. MAR PAR NDTR 설정해주는거임
P2M M2M은 src가 PAR dst는 MAR인데 M2P는 반대임

HAL_StatusTypeDef HAL_DMA_PollForTransfer(DMA_HandleTypeDef *hdma, HAL_DMA_LevelCompleteTypeDef CompleteLevel, uint32_t Timeout);
사용 안할 api라 분석 안했음.. dma를 polling모드로 동작할 이유가 없음