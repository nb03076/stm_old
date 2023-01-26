ATOMIC_SET_BIT(REG, BIT)
CR 레지스터 설정할때 자주 쓰는걸 봄
race현상 방지하려고 쓰는듯? 인터럽트 설정 CR 관련해서 자주씀

MspInit은
rcc clock enable 
gpio pin / dma / interrupt 설정

__HAL_LINKDMA(huart,hdmatx,hdma_usart6_tx);
처럼 huart->hdmatx = &hdma_usart6_tx
hdma_usart6_tx.Parent = huart6
이런식으로 연결 시켜줌

공통으로 쓰는 CMSIS 라이브러리 PPP
#define __HAL_UART_GET_FLAG(__HANDLE__, __FLAG__)  PPP_FLAG_XXX
#define __HAL_UART_ENABLE_IT(__HANDLE__, __INTERRUPT__) PPP_IT_XXX
#define __HAL_UART_GET_IT_SOURCE(__HANDLE__, __IT__)
#define __HAL_UART_ENABLE(__HANDLE__)