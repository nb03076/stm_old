RTC calendar fields
- RTC_DR 날짜
- RTC_TR  시간
- RTC_SSR

RTC캘린더에는 1hz가 가게끔 설정해줘야 한다

RTC에는 캘린더/알람/tamper detection/wake up 기능이 있다

레지스터 설정할때는 BCD로 설정 해야한다
예를 들어 콘솔로 시간을 내보내고 싶다면 RTC_FORMAT_BIN을 이용하자(bcd_to_decimal 적용됨)

rtc_init 함수에는 설정한 알람/캘린더로 초기화 시켜주니깐 이부분 생각하자

RTC 클럭 설정할 때
1. RTC hse/lsi/lse 설정
2. 클럭 소스(lse) 선택하고 RCC_BDCR register 설정해줌(RTC는 특이하게 여기서 설정함..)
보통은 RCC_AHBENR? 이런식으로 레지스터 접근해서 clock enable해주는데 rtc는 조금 다름

알람
요일/일 mask  시간 mask 분 mask 초 mask 가능하다

tamper detection
time stamp기능은 rtc 출력 핀에다 엣지 설정해준대로 전압 레벨이 바꼈다면
그때 이벤트가 발생한 시간을 RTC_TSDR / RTC_TSTR / RTC_TSSSR 에 저장하는것

wakeup은 standby같은 저전력 모드랑 관련 있는듯 
RTCCLK을 prescaler한 뒤에 RTC_WUTR(16bit downcount) 설정해주면
이 레지스터 값이 0될때 wakeup flag 날려줌
이거는 HAL_RTCEx_SetWakeUpTimer 설정 후 HAL_RTCEx_PollForWakeUpTimerEvent
폴링/인터럽트 모드 둘다 사용가능


=============================================================

프로그램 설명


버튼 인터럽트 발생시 현재 시간을 화면에 출력하고 알람 설정해준다
알람은 초가 일치하면 알람 콜백함수 호출하게끔 설정하였다
알람은 초 단위로 설정하였으므로 사용하지않는 시분요일 비트 필드를 or해서 마스크해주었다