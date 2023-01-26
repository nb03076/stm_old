rm에 있는 그림보면서 이해하는게 제일 좋다
preload 설정해두면 값을 바꿔도 바꾸기 전의 값까지는 이용하고 바뀜

초기화함수
1. 해당 타이머의 기본적인 설정
prescaler / period / counter mode(up down center aligned)
clock division / auto reload

init함수에서
CR1 = division / arpe / mode 설정
ARR = period PSC = prescaler 
마지막에 EGR해서 초기화

2. SMCR 레지스터 일부만 설정. clock source만 설정
TIM_ClockConfigTypeDef
그냥 timer 컨트롤러의 master되는 클럭이 누군가를 정해주는거
clock source : ETR2 / internal clock 이거만 설정

3. trigger가 있는 경우(TRGI) SMCR 설정
TIM_SlaveConfigTypeDef
slave mode : reset / gated / trigger / external clock mode1
trigger source : ITR0-3 / TI... / ETR1

4. TRGO 설정
CR2의 MMS 필드 채움(reset/enable/update/compare
SMCR의 MSM 필드 채움(TRGO 사용시 1 아니면 0)

5. 채널 설정(실제로는 2단계로 나누긴한데 그냥 하나로 합침)
	해당 채널을 출력으로 내보낼 경우 msp 설정해줌
	
	- OC : mode(frozen / active / inactive / toggle / force)
	pulse / preload / polarity(high가 default)
	CCER 채널 enable / polarity
	CCMR1 selection(output input indirect? 근데 output이라 딱히 안건드림 indirect는 ic기능) 
	preload / mode(frozen toggle 같은거)
	CCR pulse
	
	- IC : 방식은 OC랑 비슷함... CCMR의 selection + filter + prescaler 만 다름
	
	- PWM : preload fast(preload fast는 설정안해도 고정으로 설정되어있음)
	mode(pwm1,2모드) 설정 외에는 동일



api
- HAL_TIM_Base_Start
CR1 CEN enable

- HAL_TIM_Base_Start_IT
DIER update interrupt enable
CR1 CEN enable

- HAL_TIM_Base_Start_DMA
HAL_DMA_Start_IT(ARR 이용. 버퍼값을 ARR로 이동)
DIER TIM_DMA_UPDATE
CEN

=======================
채널 api

- HAL_TIM_OC_Start
CCER CCxE enable 
CEN enable

- HAL_TIM_OC_Start_IT
DIER interrupt enable
CCER CEN 설정

- HAL_TIM_OC_Start_DMA
HAL_DMA_Start_IT(CCR 이용. 버퍼값을 CCR로 이동)
TIM_DMA_CC1
TIM_CCx_ENABLE
CEN

pwm도 oc랑 그냥 같음

- ic dma 경우 CCR 값을 버퍼로 이동함





timebase 설정
typedef struct
{
  uint32_t Prescaler; 
  uint32_t CounterMode; up down aligned
  uint32_t Period; 
  uint32_t ClockDivision;  
  uint32_t RepetitionCounter; 
  uint32_t AutoReloadPreload;  
} TIM_Base_InitTypeDef;


typedef struct
{
  uint32_t OCMode;  timing / active / active / toggle / pwm
  uint32_t Pulse;  1
  uint32_t OCPolarity; 2
-------------------------- 
  advanced timer
  uint32_t OCNPolarity; 
  uint32_t OCFastMode; 
  uint32_t OCIdleState; 
  uint32_t OCNIdleState;
} TIM_OC_InitTypeDef;


typedef struct
{
  uint32_t OCMode;  
  uint32_t Pulse; 
  uint32_t OCPolarity;
  uint32_t OCNPolarity; 
  uint32_t OCIdleState; 
  uint32_t OCNIdleState; 
  uint32_t ICPolarity;   
  uint32_t ICSelection; 
  uint32_t ICFilter;
} TIM_OnePulse_InitTypeDef;

typedef struct
{
  uint32_t ICPolarity; 2
  uint32_t ICSelection; 3
  uint32_t ICPrescaler; 4
  uint32_t ICFilter; 1
} TIM_IC_InitTypeDef;

input capture 2개
typedef struct
{
  uint32_t EncoderMode; 
  uint32_t IC1Polarity; 
  uint32_t IC1Selection; 
  uint32_t IC1Prescaler;
  uint32_t IC1Filter; 
  uint32_t IC2Polarity; 
  uint32_t IC2Selection;
  uint32_t IC2Prescaler; 
  uint32_t IC2Filter;     
} TIM_Encoder_InitTypeDef;


내부 클럭소스 제외하고 slave 타이머라고 보면됨
external clock mode1(itr0-3)
external clock mode1 ti1 ti2
ETRF
typedef struct
{
  uint32_t ClockSource;    트리거 소스..  internal만 clock source
  uint32_t ClockPolarity;   
  uint32_t ClockPrescaler;
  uint32_t ClockFilter;
} TIM_ClockConfigTypeDef;

typedef struct
{
  uint32_t  MasterOutputTrigger;  en,uev,oc1-4
  uint32_t  MasterSlaveMode;  set되면 현재타이머와 
  trgo를 받는(slave)거끼리 동기화때문에 trgi 딜레이됨. 보통안하는듯..
} TIM_MasterConfigTypeDef;


typedef struct
{
  uint32_t  SlaveMode;     slave mode  
  uint32_t  InputTrigger;      
  uint32_t  TriggerPolarity;   
  uint32_t  TriggerPrescaler; 
  uint32_t  TriggerFilter;    
} TIM_SlaveConfigTypeDef;



typedef struct
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
{
  TIM_TypeDef                        *Instance;         /*!< Register base address                             */
  TIM_Base_InitTypeDef               Init;              /*!< TIM Time Base required parameters                 */
  HAL_TIM_ActiveChannel              Channel;           채널1,2,3,4
  DMA_HandleTypeDef                  *hdma[7];          /*!< DMA Handlers array
                                                             This array is accessed by a @ref DMA_Handle_index */
  HAL_LockTypeDef                    Lock;              /*!< Locking object                                    */
  __IO HAL_TIM_StateTypeDef          State;             /*!< TIM operation state                               */
  __IO HAL_TIM_ChannelStateTypeDef   ChannelState[4];   /*!< TIM channel operation state                       */
  __IO HAL_TIM_ChannelStateTypeDef   ChannelNState[4];  /*!< TIM complementary channel operation state         */
  __IO HAL_TIM_DMABurstStateTypeDef  DMABurstState;     /*!< DMA burst operation state                         */

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)
  void (* Base_MspInitCallback)(struct __TIM_HandleTypeDef *htim);              /*!< TIM Base Msp Init Callback                              */
  void (* Base_MspDeInitCallback)(struct __TIM_HandleTypeDef *htim);            /*!< TIM Base Msp DeInit Callback                            */
  void (* IC_MspInitCallback)(struct __TIM_HandleTypeDef *htim);                /*!< TIM IC Msp Init Callback                                */
  void (* IC_MspDeInitCallback)(struct __TIM_HandleTypeDef *htim);              /*!< TIM IC Msp DeInit Callback                              */
  void (* OC_MspInitCallback)(struct __TIM_HandleTypeDef *htim);                /*!< TIM OC Msp Init Callback                                */
  void (* OC_MspDeInitCallback)(struct __TIM_HandleTypeDef *htim);              /*!< TIM OC Msp DeInit Callback                              */
  void (* PWM_MspInitCallback)(struct __TIM_HandleTypeDef *htim);               /*!< TIM PWM Msp Init Callback                               */
  void (* PWM_MspDeInitCallback)(struct __TIM_HandleTypeDef *htim);             /*!< TIM PWM Msp DeInit Callback                             */
  void (* OnePulse_MspInitCallback)(struct __TIM_HandleTypeDef *htim);          /*!< TIM One Pulse Msp Init Callback                         */
  void (* OnePulse_MspDeInitCallback)(struct __TIM_HandleTypeDef *htim);        /*!< TIM One Pulse Msp DeInit Callback                       */
  void (* Encoder_MspInitCallback)(struct __TIM_HandleTypeDef *htim);           /*!< TIM Encoder Msp Init Callback                           */
  void (* Encoder_MspDeInitCallback)(struct __TIM_HandleTypeDef *htim);         /*!< TIM Encoder Msp DeInit Callback                         */
  void (* HallSensor_MspInitCallback)(struct __TIM_HandleTypeDef *htim);        /*!< TIM Hall Sensor Msp Init Callback                       */
  void (* HallSensor_MspDeInitCallback)(struct __TIM_HandleTypeDef *htim);      /*!< TIM Hall Sensor Msp DeInit Callback                     */
  void (* PeriodElapsedCallback)(struct __TIM_HandleTypeDef *htim);             /*!< TIM Period Elapsed Callback                             */
  void (* PeriodElapsedHalfCpltCallback)(struct __TIM_HandleTypeDef *htim);     /*!< TIM Period Elapsed half complete Callback               */
  void (* TriggerCallback)(struct __TIM_HandleTypeDef *htim);                   /*!< TIM Trigger Callback                                    */
  void (* TriggerHalfCpltCallback)(struct __TIM_HandleTypeDef *htim);           /*!< TIM Trigger half complete Callback                      */
  void (* IC_CaptureCallback)(struct __TIM_HandleTypeDef *htim);                /*!< TIM Input Capture Callback                              */
  void (* IC_CaptureHalfCpltCallback)(struct __TIM_HandleTypeDef *htim);        /*!< TIM Input Capture half complete Callback                */
  void (* OC_DelayElapsedCallback)(struct __TIM_HandleTypeDef *htim);           /*!< TIM Output Compare Delay Elapsed Callback               */
  void (* PWM_PulseFinishedCallback)(struct __TIM_HandleTypeDef *htim);         /*!< TIM PWM Pulse Finished Callback                         */
  void (* PWM_PulseFinishedHalfCpltCallback)(struct __TIM_HandleTypeDef *htim); /*!< TIM PWM Pulse Finished half complete Callback           */
  void (* ErrorCallback)(struct __TIM_HandleTypeDef *htim);                     /*!< TIM Error Callback                                      */
  void (* CommutationCallback)(struct __TIM_HandleTypeDef *htim);               /*!< TIM Commutation Callback                                */
  void (* CommutationHalfCpltCallback)(struct __TIM_HandleTypeDef *htim);       /*!< TIM Commutation half complete Callback                  */
  void (* BreakCallback)(struct __TIM_HandleTypeDef *htim);                     /*!< TIM Break Callback                                      */
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
} TIM_HandleTypeDef;