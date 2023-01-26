초기화 함수
RTC 관련 설정시에 protection 관련 설정해두고 레지스터 바꾸는듯...(RTC_WPR)
TR DR PRER 설정시에는 CR의 INIT(설정한값으로 바꾸기 위해 사용) 
/ INITF플래그 활용함(값 바꿔도 되는지 확인용)
설정해줄때 각종 플래그부터 일단 rm 보면서 확인한 뒤에 초기화 함수 구성하자
방식은 약간 CR에서 원하는 기능 enable하고 그 기능에 해당되는 레지스터 설정하는방식..
캘린더 관련해서는 플래그 확인이 필요

레지스터에 값 넣어줄때는 BCD 타입으로 넣어줌
1. msp init부터 시작
PWR->CR |= PWR_CR_DBP; 백업 도메인 enable
RCC->BDCR에서 RTC 클럭 소스 + LSE관련설정 + RTC 클럭 enable
2. RTC 기본 설정
CR에서 FMT(12/24) OSEL(알람A,B wakeup 설정시) / polarity
PRER 설정
3. 시간 설정 : TR 설정
4. 날짜 설정 : DR 설정
5. 알람설정.. 마지막에 CR1 에서 알람 enable
6. wake up 설정

과정이 조금 복잡해서 다시 한번 더 보자...

LSE = 32768
1[s] = 32768(LSE) / (비동기 prescaler * 동기 prescaler)
이 되게끔 동기/비동기 prescaler를 조절함

fRTCCLOCK = LSE = 32.768khz

f비동기주파수는 RTC_SSR subseconds downcounter. 0이 되면 PREDIV_S에 값 채워주는듯.. reload하구
RTCCLK 2클럭마다 calendar 값을 shadow register로 copy / RSF bit set

tamper : Vdd로 전원 공급중이다가 Vbat으로 전원 스위칭 되는 경우인가?

RTC_TR : 현재 시분초 BCD
RTC_DR : 연/월/일/요일 BCD
RTC_CR
RTC_ISR : 초기화 됐는지 안됐는지 확인하는 플래그들이 있다
RTC_PRER : 동기/비동기 prescaler
RTC_WUTR : 16비트
RTC_ALRMAR : 시분초일 요일
RTC_ALRMBR : 시분초일
RTC_WPR : 백업 레지스터 쓰기 보호용 레지스터. protection key
RTC_SSR : synchronous prescaler’s counter
second fraction = (pres_s - ss) / (pres_s +1)
RTC_SHIFTR : ? 1초 더해주는 기능 있음
RTC_TSTR : 타임 스탬프 / 시분초
RTC_TSDR : 타임 스탬프 / 일월 요일
RTC_TSSSR : 타임 스탬프
RTC_ALRMASSR / RTC_ALRMBSSR
RTC_BKPxR
그외는 tamper/calibration 같은 것들임

calendar는 TR DR SSR 같은 현재 시간을 알려주는 레지스터를 말함
이거는 1Hz마다 작동하게 만들어야함

typedef struct
{
  uint32_t HourFormat;      12/24
  uint32_t AsynchPrediv;    
  uint32_t SynchPrediv;    prescaler를 이용해서 1hz 만들어야한다
  uint32_t OutPut;          alarm a,b wakeup output select
  uint32_t OutPutPolarity;  output polarity
  uint32_t OutPutType;    	오픈드레인/푸시풀
} RTC_InitTypeDef;

typedef struct
{
  uint8_t Hours;          
  uint8_t Minutes;       
  uint8_t Seconds;       
  uint8_t TimeFormat;      	AM / PM
  uint32_t SubSeconds;      SSR
  uint32_t SecondFraction;  HAL_GET_TIME에서만 씀
  uint32_t DayLightSaving;  DST에서 사용 //deprecated
  uint32_t StoreOperation;  DST에서 사용 //deprecated
} RTC_TimeTypeDef;

typedef struct
{
  uint8_t WeekDay;  요일
  uint8_t Month;  
  uint8_t Date;    	며칠
  uint8_t Year;    
} RTC_DateTypeDef;

typedef struct
{
  RTC_TimeTypeDef AlarmTime;     	알람 시간
  uint32_t AlarmMask;           	알람할때 안쓰는 것을 마스크함(요일/일/시/분/초)
				만약 초/분만 사용한다 치자 그러면 일/시간은 마스크해야한다
  uint32_t AlarmSubSecondMask;   	보통 사용안하는거 같음. mask해주면 된다
  uint32_t AlarmDateWeekDaySel;  	일/요일 선택
  uint8_t AlarmDateWeekDay;      	요일
  uint32_t Alarm;                	알람 A/B
} RTC_AlarmTypeDef;

typedef struct
{
  RTC_TypeDef                 *Instance;  레지스터
  RTC_InitTypeDef             Init;     
  HAL_LockTypeDef             Lock;       
  __IO HAL_RTCStateTypeDef    State;      
#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)
  void (* AlarmAEventCallback)      (struct __RTC_HandleTypeDef *hrtc);  /*!< RTC Alarm A Event callback         */
  void (* AlarmBEventCallback)      (struct __RTC_HandleTypeDef *hrtc);  /*!< RTC Alarm B Event callback         */
  void (* TimeStampEventCallback)   (struct __RTC_HandleTypeDef *hrtc);  /*!< RTC Timestamp Event callback       */
  void (* WakeUpTimerEventCallback) (struct __RTC_HandleTypeDef *hrtc);  /*!< RTC WakeUpTimer Event callback     */
  void (* Tamper1EventCallback)     (struct __RTC_HandleTypeDef *hrtc);  /*!< RTC Tamper 1 Event callback        */
#if defined(RTC_TAMPER2_SUPPORT)
  void (* Tamper2EventCallback)     (struct __RTC_HandleTypeDef *hrtc);  /*!< RTC Tamper 2 Event callback        */
#endif /* RTC_TAMPER2_SUPPORT */
  void (* MspInitCallback)          (struct __RTC_HandleTypeDef *hrtc);  /*!< RTC Msp Init callback              */
  void (* MspDeInitCallback)        (struct __RTC_HandleTypeDef *hrtc);  /*!< RTC Msp DeInit callback            */
#endif /* USE_HAL_RTC_REGISTER_CALLBACKS */
} RTC_HandleTypeDef;

HAL_StatusTypeDef HAL_RTC_Init(RTC_HandleTypeDef *hrtc);

HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format);
HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format);
HAL_StatusTypeDef HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format);
HAL_StatusTypeDef HAL_RTC_GetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format);

HAL_StatusTypeDef HAL_RTC_SetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Format);
HAL_StatusTypeDef HAL_RTC_SetAlarm_IT(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Format);
HAL_StatusTypeDef HAL_RTC_DeactivateAlarm(RTC_HandleTypeDef *hrtc, uint32_t Alarm);
HAL_StatusTypeDef HAL_RTC_GetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Alarm, uint32_t Format);
void              HAL_RTC_AlarmIRQHandler(RTC_HandleTypeDef *hrtc);
HAL_StatusTypeDef HAL_RTC_PollForAlarmAEvent(RTC_HandleTypeDef *hrtc, uint32_t Timeout);
void              HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc);

HAL_StatusTypeDef   HAL_RTC_WaitForSynchro(RTC_HandleTypeDef *hrtc);

void              HAL_RTC_DST_Add1Hour(RTC_HandleTypeDef *hrtc);
void              HAL_RTC_DST_Sub1Hour(RTC_HandleTypeDef *hrtc);
void              HAL_RTC_DST_SetStoreOperation(RTC_HandleTypeDef *hrtc);
void              HAL_RTC_DST_ClearStoreOperation(RTC_HandleTypeDef *hrtc);
uint32_t          HAL_RTC_DST_ReadStoreOperation(RTC_HandleTypeDef *hrtc);

HAL_RTCStateTypeDef HAL_RTC_GetState(RTC_HandleTypeDef *hrtc);