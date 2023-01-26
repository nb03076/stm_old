ABC of ADC ppt 자료 참고하자
+-1/2 lsb offset 넣자. 반올림 개념 적용한거 pdf9페이지
offset error : 아날로그-디지털 코드 기울기 변화?
gain error : 아날로그 값 offset 생김. 일차함수 그래프 좌,우측 이동
DNL : 입력 전압 offset 같은 느낌? 1LSB가 안지켜지는것
DNL이 1넘어가면 특정 디지털 코드 사라짐. 0에서 벗어날수록 안좋은거
INL : 디지털 코드 값 변화? 같은 느낌.

adc 전달 함수의 비선형성은 고조파를 생성
순수 sine wave라면 특정 주파수에서만 power가 생기지만
adc 비선형때문에 고조파 성분에도 power가 존재하는걸 볼 수 있음



양자화 오차
ADC에서 DAC로 신호 복원할때 
ADC sampling rate가 아날로그 신호 최고 주파수의 2배보다 커야함(나이퀴스트 샤논 샘플링)
그래야 정상적으로 복원이 되고, 디지털신호처리 수업에서는 경험상 5배정도면 잘 복원된다고 하였음
만일 2배이하면 aliasing 발생

시그마-델타 ADC 원리
양자화 에러를 줄이기 위해 오버샘플링, 양자화 에러처리(DAC이용해서 오차 피드백), LPF필터

anti aliasing filter > Sample&Hold > summation > LPF > OSR
아날로그											디지털

summation 부분은 아날로그 지령값이랑 ADC한 후에 다시 DAC를 해서 지령값이랑 피드백값 오차적용

정밀한 값 얻을때 사용. 속도는 SAR에 비해 느리다


SAR 원리
V-Vref / (2 ^(i+1)) 0.5 0.75 0.875.... 이런 순서로 계속 비교
변환 수행하는데 3clock + 12 clock(12bit)
Vsample이랑 Vdac(0.5 0.75 0.875... 이값인듯)랑 비교기로 비교함


ADC 데이터시트
DC노이즈 : 전자회로가 가지고 있는 비선형성
양자화노이즈 : 아날로그 입력 신호의 진폭을 제한된 bit resolution 으로 변환하는 과정에서 생김

AC사양 : 샘플링 신호, 노이즈 특성. 시간에 대해서 연속적인 
아날로그 신호를 샘플링하는 과정에서 발생하는 신호와 잡음에 대한 특성
SNR THD SFDR IMD effective resolution SINAD ENOb bandwidth
DC사양 : 디지털 코드(정확도)  평균적인 의미의 정확도
resolution, offset error, gain error, DNL, INL

SNR : signal to noise ratio
inband(실제 신호가 있는 부분을 말하는듯?)신호에서 불필요한 노이즈가 껴있는거 (power비)
SNR = 20log(S/N) = 6.02n + 1.76 db
SINAD = 20log(S/(N+D)) = 6.02 * ENOB + 1.76 db D:고조파 왜곡
ENOB(ADC가 표현할수 있는 bits갯수) : (SINAD-1.76)/6.02 [bits]

DC
offset error : calibration 가능. 전압 offset
RLC같은 수동소자 보정하기 위한 calibration
gain error : offset error calibration 하고 나면 gain error calibration 가능
offset + gain error = full scale error
integral NonLinearity : 
Differential Nonlinearity :
INL DNL은 캘리브레이션 하기힘듦...

STM32
data alignment : left right인데 right가 default
Scan : in0-16사이에 지정된것들 순서대로 변환 multi channel
Continuous : 연속변환

-single channel single conversion
scan x continuous x
ex)배터리 전압 레벨 측정

-multi channel single conversion
scan o continuous x
보통 DMA사용함(circular)
group 변환임 / 한번 group 변환 후 멈춤
일반적으로 많이 사용하는 방법

-single channel continous
채널 1개 계속 변환
ex) 배터리 전압/온도 측정

-multi channel continous


prescaler/fclk(adc) * sampleClk * channel
10ms = 위의 공식 * x   x는 10ms동안 받는 샘플 갯수

싱크 맞춰서 샘플링 : dual/triple simultaneous(내부적으로 master/slave 모드 사용)
독립적인 타이밍으로 동작해도 된다면 independent

릴세후 1번 캘리브레이션 해줘야 정확한 adc 읽을 수 있다. HAL_ADCEx_Calibration_Start

트리거 : 타이머 / EXTI11

Interleaved mode
하나의 물리적인 ADC 입력 채널(핀)을 다수의 ADC block 이 교대로 샘플링하는 방식을
의미한다. 하나의 ADC block 이 샘플링이후 컨버전을 하는 딜레이 동안에 다른 ADC
block 에서 샘플링을 하는 방식으로 SPS (sampling per second) 를 최대로 올려야 할때
사용한다. 

Alternate trigger mode
interleaved injected판.. 


=============================================================

임종수의 stm32 책 정리

1. dual regular simultaenous mode only
adc1(마스터) + 2or3(슬레이브) 내부적으로 동기화하여 샘플링
ex) 전압/전류 측정
