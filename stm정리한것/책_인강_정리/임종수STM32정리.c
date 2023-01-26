/* GPIO */
vss-0.3 ~vdd +3 까지 허용 전압범위
넘어서면 device 손상됨
총 전류 합 100mA 넘어가면 문제가 생김.. 그래서 bjt 같은거 이용해서
트랜지스터 혹은 게이트 드라이버에 맡김

단락되면 골치아프니 10, 100k옴 넣음...
ex) 이거는 disc에서 pa0이 푸시버튼인데
일반적인 gpio로 사용하고싶다면 100k옴 줘서 쇼트안나게 해줌.

SN54/74HCT573
reset시 이전 값을 유지하기위해 사용함
/OE가 0이라면 동작하고 1이면 HI-Z
이전값 유지할거면 /OE 0 LE 0하면 됨..
평소에는 일반 D플립플롭처럼 작동함

Tsu : setup time
Tw : pulse duration
Th : hold time

input : output driver과 연결 끊어짐.
슈미트 트리거 작동, 풀업, APB2 clock cycle 마다 IDR갱신


/* 인터럽트 */
수십 us 정도 실행될정도로 코딩해야함
ms단위면 골치아픔
event : mcu 내부에서 설정한 조건에 의해 발생
인터럽트 : 외부 변화

EXTI에서 PA0 PB0이 동시에 인터럽트 소스로 사용 못함
둘 중 하나만 인터럽트 소스로 설정해야함

추가적인 EXTI는 wake up 용도로 사용함

버튼 인터럽트 걸때 풀업 저항 다는 이유
- 버튼 떼면 floating 상태 인데 이러면 잘못된 값이 갈수도 있음
- 풀업 저항을 달면 이것을 해결할수 있다.(floating는 저항 무한대인걸 생각!)

진짜 중요한게 풀업 풀다운 저항가지고 idle state 1 or 0으로 만들수있음
이게 진짜 중요한듯...
floating으로 두면 잘못된 값 들어올수 있어서
무조건 풀업이든 풀다운이든 사용해야함!


/* 전원과 Clock 설정 방법 */
mcu가 정상 동작하기 위해 2~3.6v의 외부전원 VDD가 공급되어야함
내장된 레귤레이터가 1.8v로 변환하여 공급
PVD(Programmable Voltage Detector) 인터럽트로 처리가능
공급되는 전원 감시용도임
그니깐 Vdd깂이 PVD로 설정한 값 이하로 떨어지면 위험한 상태니깐 이때 인터럽트걸고
특정 행동을 하게끔 인터럽트문을 이용해서 처리하는것

Run : 기본적으로 동작하는것
Sleep : cpu clock 정지, 주변장치들은 정상 동작
		인터럽트, 이벤트 발생하면 cpu 깨울수 있음
Stop  : 모든 clock 정지, SRAM과 registers 데이터 값 보존
		EXTI 인터럽트 발생시 cpu 깨울 수 있다(event는 가능한가??)
Standby : 모든 clock 정지. backup영역, 대기모드 회로내의 레지스터
		  제외하고 다른 데이터 값들 다 지워짐
		  외부 reset(NRST), IWDG reset, RTC기능 상승엣지 발생하면
		  빠져나올 수 있다.

사용안하는 핀들을 다 아날로그로 바꾸면 전력 소모 수백 uA 줄일 수 있다
cubemx에 이런 설정 있음...


PLL : PLLON = 0 하고 PLL disable
	  PLLRDY = 0 될때까지 기다림
	  PLLRDY = 0이면 원하는 값으로 바꿈
	  PLLON = 1
	  
IWDG 는 lsi로 동작
LSE는 RTC 사용할거면 있는게 좋은데 그거아니면 굳이?


/* 타이머 */
UI : EGR의 UG = 1 하면 발생함. CR1의 UDIS = 1 혹은 ARR = 0 하면 UI 발생안함
	ARR = 0이면 카운팅 안하고 UDIS = 1 인 경우는 오버플로우시 그냥 멈출듯?(이건 예상)
	그래서 나중에 동작시키고 싶다면 ARR값 바꿔주면 됨
	
일반적인 업,다운 카운터는 period = (1+ARR) * clock period 인데
center aligned모드는 (1+2*ARR) * clock_period 임.

DIR은 center aligned 모드일때만 읽을수 있다. 현재 올라가면 0 내려가면 1로 표시됨
단순 up,down counter면 처음에 값이 정해지고 값 안바뀜..

CR1의 CMS로 center aligned 설정하고 CEN한게 다임..


와치도그 : 임의의 시간내에 와치독 timer clear 안하면 system이 hang상태에 빠짐
IWDG : main clock과는 관계없이 동작 LSI사용.
다운카운터이고 reload value 최대 4095 prescaler 2^2~2^9(2^(2+prescaler[0:2]))

tms = (1/32khz) * prescaler * (RL[11:0] +1) [ms]
RL[11:0] = (tms *32khz) / (prescaler * 1000)   - 1

이게 LSI로 동작하다보니 오차가 좀 심한듯... 
그리고 refresh무조건 인위적으로 해줘야하는듯?
안해주니깐 그시간 지나면 작동 이상하게 됨.. 왜이런건지 모르겟슴

IWDG작동 후에는 항상 일정 간격 내로 refresh안하면 멈추게됨...
그래서 정말 중요한 부분에만 실행시켜서 측정하는게 맞는듯?
그리고 만약 timeout되면 핸들러 호출해서 reset하게 만들어야할듯..


WWDG : APB clock으로 동작


us단위로 delay 하려면...
1us 단위로 counter값이 올라가게끔 설정하고 cnt 레지스터값 읽어서
원하는 시간 지나가면 반복문에서 빠져나오게끔 만듦

ex) void delay(uint16_t us){
	__HAL_TIM_SET_COUNTER(&htim1,0);
	while(__HAL_TIM_GET_COUNTER(&htim1) < us);
}



/* PWM 생성 및 사용 방법 */
CR1 cen
CCMR1 OC1M을 pwm모드로 설정, OC1PE preload 1
CCR값 정해줌. CCER CC1E enable CC1P polarity

L298쓸때
IN2를 LOW 로 고정해두고 IN1에다 PWM... (값이 1이면 모터 돌아감 정방향)
반대로 하면 역방향. 둘다 LOW면 전원 연결X 둘다 HIGH면 브레이크

타이머 동기화
- reset mode : trgi 발생시 counter,prescaler 초기화 후 다시 counting 시작
- gated mode : 선택한 입력의 level 구간에 대해서만 counting
- triggered mode : 선택한 입력에 대한 이벤트 발생시 counting

reset mode 사용하려면
우선 slave mode는 reset mode
trigger input : TI1FP1
clock source : internal
channel : input capture(이거 이용해서 reset mode 제어하는것)

채널에서는 rising edge면 TI1FP1에서 나오는 파형이 rising edge시 reset mode 작동함
반대로 falling edge면 falling edge때 작동

여기도 설정법은 아래 gated랑 별 차이 없음.

예제)
TI1FP1 과 버튼을 연결함. 10ms 간격으로 led 토글링함.
동작은 버튼을 누르면 버튼 파형은 rising edge가 되는데 이때 reset mode 동작함
PeriodElapsedCallback 함수 호출해서 수행한뒤에
count = 0 으로 만들고 다시 counting 시작함

gated mode 사용하려면
slave mode : gated mode
trigger input : TI1FP1,ETR1,ITR0-3 
clock source : internal
channel : 채널 설정은 TI1FP1하면 알아서 설정됨.(여기서는 input capture라고 설정해주는거아님)
rising edge시 TI1FP1에 값이 1이 들어가면 타이머 작동하고 0이 들어가면 타이머 멈춤(카운팅안해)
falling edge면 반대

실험결과 rising edge일때 바로 callback 함수 호출하는건 아님 딜레이가 좀 있음
40ms마다 callback 함수 호출하게 만들었는데 0~40ms 후에 반응이 옴....

CR1 에서는 CEN
SMCR 의 TS는 TI1FP1(101)  SMS(101 gated mode)
gated 모드에서만 TIF와 관련없다.
falling edge면 CCER 채널1의 CC1P값 1로 바꿈		


trigger 모드
tim3 		> 		tim4 		> 		tim5 이렇게 이었고
no slave 			trigger 			trigger
oc2ref				oc2ref				X


output compare no ch를 이용해서 트리거 함 (oc2ref TRGO)
TIM1 CR2 MMS 0x7 OC4REF(TRGO)
그왼 딱히 볼거 없는듯.. ITR이용해서 설정한거임



/* 타이머 레지스터 정리 */
CR1 일반적인 타이머 기능 컨트롤(basic timer 생각하면될듯?)
CR2 주로 MMS(TRGO 어떤 소스이용해서 보낼것인가)
SMCR slave mode 관련해서 설정(MSM 마스터 슬레이브 모드도 여기서 설정)
DIER 트리거, dma, 업데이트 인터럽트설정
SR
EGR 주로 UG,TG 이용
CCMR1 채널1,2설정 selection,모드, preload, 
input capture일 경우 selection,prescaler,filter
CCER polarity, enable


/* UART */
int __io_putchar (int ch) cubeide
int fputc(int ch, FILE *f) keil,iar

CR1 일반적인 제어
CR2 usart관련. stop비트 설정
CR3 dma,error ie, rs232관련
BRR 속도 조절하는거
SR

해보니깐 1비트 start 1비트 데이터 8비트 stop 1비트 이렇게 구성되는듯?
마프 처리속도에 따라서 idle state 구간 너비가 다른듯
근데 9600bps로 하니깐 규격이 1start 8data 1stop 1idle 이렇게 11비트처럼 있음

이부분은 다시 봐야할듯 p254부터 다시 보자..

/* 9장 RTC 사용방법 */
HAL_RTC_SetDate / SetTime / GetDate / GetTime
HAL_RTCEx_BKUPWrite / Read

/* 10장 I2C */


/* 11장 DAC 사용방법 */
2*REF(참고할 전압) * DR / 0x1000 (12비트 DAC라서 그럼)
DAC 내부 REF는 1.024 2.048 사용가능하고 외부 전압은 3-5 Vdd사용가능한듯??

상위 4비트가 CR 하위 12비트가 DR임
상위 4비트 가지고 DAC on/off, A,B출력 낼지, CR레지스터 write할지 등을 정할수 있음





SCB->SCR의 0번째 비트에 sleep on exit
이거 쓰면 인터럽트 걸고 나서 sleep mode로 빠지고
인터럽트 다시 걸리면 다시 wake up 했다가 isr 끝나고 나면 다시 sleep

reduce power cosumption
- stop clock
- reduce clock frequency
- recude voltage
- turn off power supply
- 사용 안하는 핀 analog mode로
- uart 보드레이트 증가




/* 오타 */
85페이지 3,4번 WOE > BOE
167페이지 Period = (1+ARR) * Clock_Period 부분에서 1을 없애야함
