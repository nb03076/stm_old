gpio 초기화함수
GPIO_InitTypeDef GPIO_InitStruct = {0};

__HAL_RCC_GPIOA_CLK_ENABLE();
(SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN) 하는 매크로)

gpio pin / mode /pull 초기화
즉 MODER(input output alternate analog) 
OTYPER(푸시풀/오픈드레인) 
PUPDR OSPEEDR AFR 설정

alternate function은 레퍼런스 메뉴얼 보고 참고해야함..!

exti 설정시에
SYSCFG->EXTICR 설정
EXTI-> RTSR FTSR EMR IMR (설정한대로)설정
HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0) (NVIC->IPR에 값넣음 systick같은건 SCB->SHPR)
HAL_NVIC_EnableIRQ(EXTI0_IRQn) (NVIC->ISER)



InitTypeDef는
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PG13 PG14 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
이런식으로 설정되어있다.

Pos 는 n번째 비트다 라는 뜻
Msk나 USART_CR1_M같은건 1<<12 이런식


==============================================
LL 드라이버
LL_GPIO_IsInputPinSet
LL_GPIO_IsOutputPinSet
LL_GPIO_SetOutputPin
LL_GPIO_ResetOutputPin
LL_GPIO_TogglePin