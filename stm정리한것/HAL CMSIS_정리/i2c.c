i2c는 rm에 있는 전송 프레임을 보는게 중요한듯
i2c는 오히려 ll 라이브러리가 더 이해하기 편한듯
다른사람이 짜둔 ll이용한 i2c 전송하는 함수를 보는게 이해하기 더 편함
항상 transfer / receive 할때는 busy flag를 확인하자

master transfer
S > SB=1(start bit) > address > Ack > ADDR=1 > SR에 비어있으니 DR에 채워넣음(TXE=1)
SR에는 차있지만 TXE=1(DR에 채워넣음) > Ack > 반복... > 마지막 데이터 Ack > 
TXE=1 BTF=1 > stop

master receive
s > sb=1 > address > ack > addr = 1 > data(받은데이터) > ack
> rxne > ack > 반복... > 마지막 데이터 RXNE(wait until btf=1) > nack > stop


CR1 일반적인거
CR2 dma + interrupt + pclk clock

초기화 함수
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  
  이긴한데 솔직히 speed, duty cycle, addressingmode정도만 하면될듯?
  FLTR레지스터 이용해서 아날로그/디지털 필터 enable
  
  CCR이나 TRISE는 rm보면서 계산하면 됨
  
  
- HAL_I2C_Master_Transmit
 busy flag=0일때 까지 기다림 > PE enable > 버퍼, 사이즈 채움
 start bit set > wait until sb=1 > DR = address > wait until addr=1
 > clear addr flag > while(size만큼) { wait until txe set >
DR = 버퍼 > 만약 BTF=1이고 사이즈가 0이 아니라면 한번더 보냄. 아마도 2번만 보낼때 그런듯
> 만약 BTF = 1이라면 stop bit 

- HAL_I2C_Master_Receive
 전송방식이 위와 거의 유사함
 
- HAL_I2C_Master_Transmit_IT
  uart같은 통신과 방식이 같은데 처음에 busy flag =0 일때까지 기다리고 마지막에 I2C_CR1_START해줌
  약간 polling 같은 느낌이 들긴함... busy flag를 while문으로 기다림
  
- HAL_I2C_Master_Receive_IT
  마지막에 I2C_CR1_ACK / I2C_CR1_START해줌 나머지는 위와 동일
  
- HAL_I2C_Master_Transmit_DMA / receive도 동일
  여기도 busy flag = 0 일때 까지 기다림. 나머지는 다른 통신과 동일..
  마지막에 I2C_CR1_ACK / I2C_CR1_START set
 
  
 