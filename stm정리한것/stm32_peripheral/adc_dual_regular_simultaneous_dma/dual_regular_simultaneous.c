설정
1. adc 모드 dual regular simultaneous
dma access
scan/continous enable(이건 하고싶은대로..)
rank 설정해주는데 adc master와 slave 끼리 서로 샘플링 시간 일치하는게좋다
여기선 adc in1-4 샘플링 3cycle로 통일 시켰다 number of conversion = 4
adc2에서는 in5 샘플링 3cycle인데 number of conversion을 adc1처럼 4개로
통일 시키는게 핵심이다! 이거때문에 헤맸음..

2. adc dma 설정
circular는 무조건 해야하는지는 모르겠고..
정말 중요한게 periph/memory 둘다 data width를 word로 해줘야한다
adc1/2 둘다 이렇게 설정해야함! 안그러면 데이터가 이상하게 받아짐

3. 코드 작성
  HAL_ADC_Start(&hadc2);
  HAL_ADCEx_MultiModeStart_DMA(&hadc1, temp, 4);
코드 자체는 간단하다
 
4. 결과
temp[0] = In5결과 << 16 | In1결과
temp[0] = In5결과 << 16 | In2결과
temp[0] = In5결과 << 16 | In3결과
temp[0] = In5결과 << 16 | In4결과
이런식으로 나오게 된다. In5는 adc2에서 하나의 채널만 선택했으므로 In5만 나오는거고
만약 adc1처럼 여러개 하고싶으면 adc1처럼 설정하면 됨.