초기화함수
1. PWR 클럭인가
2. __HAL_PWR_VOLTAGESCALING_CONFIG(PWR->CR)
최대 클럭 주파수 정해주는것.. 당연히 높을수록 전력소모 큼
3.   
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 10;

m(나눔) n(곱함) p (나눔2,4,6,8만 가능) Q는 i2s usb같은 특수한 곳에 줄때
클럭 소스(hse? hsi?) / pll / hse lse 상태
(일단 InitStruct로 정의만 해둠)

4. CFGR의 SWS필드이용해서 시스템클럭 알아낸뒤에 
RCC_HSE_ON
RCC_FLAG_HSERDY 처럼 RDY플래그 이용
lsi/hsi경우 클럭인가해줘야함

5. 
pll off하고 rdy플래그 이용
PLLCFGR에서 pll관련해서 설정
pll on

6. 168mHz이상의 클럭을 이용하려면 오버드라이브 이용해야함
전압을 180mhz 구동만족하려면 전압변경해야함
그다음 오버드라이브 enable 하고 오버드라이브 rdy 기다리고
오버드라이브 스위칭하고 rdy 기다리면 됨



7.
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  
  이것은 pll 이후의 prescaler를 어떻게 설정할것인가를 나타냄

8. FLASH_ACR의 flash latency 설정(아마 24mhz단위로 1씩 증가했었던 거 같은데 rm 참고하자)
9. HCLK SYSCLK PCLK등을 설정 RCC->CFGR여기에 있는거 + RDY 플래그 이용함
10. HAL_InitTick (타이머 6사용하면 타이머 설정해두고 인터럽트 걸어둠)
근데 그냥 systick 쓴다면 systick 관련 레지스터 설정하고 인터럽트 설정함


rcc->cr pll, hse, hsi 설정
PLLCFGR : m,n,p,q
CFGR : mco1,2 + 각종 클럭 prescaler 설정
SWS: System clock switch status 이걸로 현재 동작하는 
시스템 클럭(pll,hse등) 알 수 있다.