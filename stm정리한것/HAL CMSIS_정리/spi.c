핀들 pull up 해주는게 좋다

초기화 함수
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
 
마스터,슬레이브 / 모드(rx tx관련) /  데이터 사이즈 / NSS / mode(0-3)
prescaler / msb first / ti mode

CR1에서 다 설정함 bidimode가 1이면 1line(tx 나 rx only)
bidioe에서 tx/rx only 정의
DFF : 8/16비트
SSM: 1 해줘서 gpio로 cs 다룸
LSBFIRST : 0이면 msb
BR : fpclk/n 그냥 prescaler임
MSTR : 보통은 1(master)
CPOL / CPHA

CR2는 인터럽트 / DMA 설정


8비트 기준으로 설명 
그리고 16비트면 사이즈 단위가 16비트라는 점 유의
- HAL_SPI_Transmit
1. 1 line이면 SPI_CR1_BIDIOE 이거 set해줌
2. 전송하는 방식은 uart랑 비슷
3. busy flag 기다림

- HAL_SPI_Receive
1. 1 line이면 SPI_CR1_BIDIOE=0(receive only)
2. 2 line이면 HAL_SPI_TransmitReceive 이용하는데
이거는 그냥 tx한번 rx한번씩 해주는 함수임
그냥 더미 데이터 보내주면됨 보통은 0xFF 보내주는듯...
(여러 센서들 라이브러리에서 0xFF 보내주는거 자주봄)
3. busy flag 기다림

- HAL_SPI_Transmit_IT
1. 버퍼/사이즈 설정
2. 인터럽트 enable


- HAL_SPI_Transmit_DMA
1. 버퍼 / 사이즈 설정
2. DMA 설정
