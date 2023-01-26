자료구조 원형큐 생각하면 됨
uint8_t UartRxDMA_Buf[RXBUF_MAX];
uart dma수신때 쓸 버퍼

typedef struct UARTCircBufTag {
	uint8_t	RxBuf1[RXBUF_MAX];
	uint8_t	RxBuf2[RXBUF_MAX];
	int32_t	m_iHead,m_iTail,QueueSize;
	uint32_t OverFlowNum,UnderFlowNum;
} UARTCircBufTyp;

idle 인터럽트 걸리면 QueueClear
idle 걸리기전까지의 데이터 수신한 값을 QueuePutByte

void QueueClear() UARTCircBufTyp 구조체 초기화
int QueueGetSize() (head - tail + max) % max
bool QueuePutByte(uint8_t b)
	- 현재 큐에 들어있는 size가 max-1같다면 오버플로우 > 함수 종료
	- 그외 rxbuf1에다 값 저장하고 head++, head %= max
	
bool QueueGetByte(uint8_t* pb)
	- 사이즈가 0이라면 underflow발생 > 종료
	- rx1버퍼값 반환 > tail++, tail %= max
	
void QueueDisplayFun(void)
	현재 헤드값에서 0일때 까지 rxbuf2에다 값 저장함
	링버퍼의 제일 끝 인덱스부터 -1하면서 값 저장