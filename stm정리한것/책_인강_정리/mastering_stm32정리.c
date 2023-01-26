primask 0:enable 1:disable
cpsid i cpsie i cpsid f
__enable_irq()
primask에 1이 들어가면 pending되어도 실행이 안됨 그래서 mask

basepri는 irq 번호 +16(16번까지는 exception이고 그뒤가 irq임)
포함해서 그이상인것(우선순위 낮은것)까지 인터럽트 발생 안하게 막는거

mspinit은
__HAL_RCC_USART2_CLK_ENABLE()처럼 클럭 주고
gpio 설정해줌

sprintf(버퍼,내용)
하고 나서 uart_transmit해줌..

링버퍼 사용하자..

void DMATransferComplete(DMA_HandleTypeDef *hdma) {
if(hdma->Instance == DMA1_Channel4) {
//Disable UART DMA mode
huart2.Instance->CR3 &= ~USART_CR3_DMAT;
//Turn LD2 ON
HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}
}

link the UART_HandleTypeDef to the DMA_HandleTypeDef 
using the __HAL_LINKDMA();

void DMA1_Channel4_5_IRQHandler(void) {
HAL_DMA_IRQHandler(&hdma_usart2_rx); 
//This will automatically call the 
//HAL_UART_RxCpltCallback()
}

함수내부 로컬변수는 스택에 쌓이니깐 dma쓸때
글로벌 변수 써야지 접근가능함..!

struct Queue
{
	char buffer[1024];
	int32_t header,tail,size;
	uint32_t overflow,underflow;
}

struct Queue Q;

void QueueInit(void)
{
	Q.header = Q.tail = Q.size = 0;
	Q.overflow = Q.underflow = 0;
	
	memset(Q.buffer,0,1024);
}

int32_t QueueGetSize()
{
	int32_t ret = 0;
	
	ret = (Q.header - Q.tail + 1024) % 1024;
	return ret;
}

bool QueuePut(char data)
{
 if(QueueGetsize() == (1024-1)){
	 Q.overflow++;
	 return false;
 }
 Q.buffer[Q.header++] = data;
 Q.header %= 1024;
 
 return true;
}

bool QueueGet(char *pb){
	if(QueueGetSize() == 0){
		Q.underflow++;
		return false;
	}
	
	*pb = Q.buffer[tail++];
	Q.tail %= 1024;
	return true;
}

spi통신에서
ssi비트는 slave의 nss핀을 제어할수있음
이게 1이면 nss는 high 0이면 low임
ssm=1일때 이걸 사용하는듯?
hardware는 그냥 gpio이용해서 ss핀 관리하는거

https://www.slideshare.net/FastBitEmbeddedBrain/mastering-microcontroller-timers-pwm-can-rtclow-power-166457262
sleep모드 나중에 보자...

1. thread mode
2. handler mode

xpsr 하위 9비트는 irq 나타냄 0이면 thread
CONTROL 레지스터는 access level 바꿔줌

LR은 브랜치 하기전 다음에 실행할 것을 담아둠...

__asm volatile
(
"MOV RO,R1\n\t"
"ADD R1,R0\n\t"
);

리셋핸들러
data,bss 초기화
c std library 초기화(libc)
main으로 분기


thumb때문에 주소가 0x80001000이라 치면
0x80001001로 분기한다고 어셈블리에서 나오고
실제로는 0x80001000으로 가는거임

CONTROL레지스터는 privileged와
msp psp 뭐 쓸건지 정해주는것

비트밴딩은 32비트짜리 레지스터의 1비트를 주소를 통해 얻을수있다..

R0-3 파라미터 R0 리턴값 R1 리턴값이 64비트면 사용

인터럽트 발생했다면
xPSR PC LR R12 R3 R2 R1 R0
높은주소					낮은주소
이렇게 스택에 쌓인다.

인터럽트 발생시...
1. thread 모드고, interrupt request 0>1
2. interrupt pending status 0>1
3. 이때 레지스터 세트 스택에 쌓음
4. interrupt active status bit 0>1
5. interrupt pedning bit 1>0
6. interrupt request 1>0
7. exception return
8. interrupt active status bit 1>0
9. unstack
10. thread

interrupt active status 는 말그대로
인터럽트 실행중이라는걸 알려줌
pending은 thread에서 handler로 넘어갈때까지 active
interrupt request는 핸들러 실행중에 inactive됨

인터럽트 도중 또 pending이 됐다면 인터럽트 종료후
다시 인터럽트 상태로 넘어감

exception 발생했을때 LR에 EXC_RETURN 값이있음
이건 0xFFFFFFFD 스레드 psp
0xFFFFFFF1 핸들러 msp
0xFFFFFFF9 스레드 msp

/* 메모리 레이아웃 */
글로벌 초기화x :			.common		Data(SRAM)
글로벌 초기화o :			.data		Data(SRAM + Flash)
글로벌 static 초기화x : 	.bss		Data(SRAM)
글로벌 static 초기화o :	.data		Data(SRAM + Flash)
로컬 변수			 :				스택이나 힙(SRAM)
로컬 static 초기화 x :	.bss		Data(SRAM)
로컬 static 초기화 o :	.data		Data(SRAM + Flash)
const data		 :	.rodata		Code(Flash)
const string	 :	.rodata.1	Code(Flash)
Routines		 :	.text		Code(Flash)

The KEEP(*(.isr_vector)) says to LD to keep the vector table inside the final
absolute file, otherwise the section could be “stripped” by other tools that perform optimizations
on the final file.

리스트 파일에서
contents는 lma 그러니깐 플래시에 있는 실제 데이터 용량을 나타냄
alloc 공간 할당(sram flash 둘다 가능) 실제 메모리에 할당된다는 뜻..
load 이진 파일에 포함된 섹션의 데이터를 최종 LMA 메모리로 로드합니다.

*(.text) /* Program code */
*(.text*) /* Merge all .text.* sections inside the .text section */

.text.함수명 이렇게 되어있음 그래서 위에처럼 *(.text*)쓰는듯
그냥 .text는 libgcc하고 crt0 이게 포함되는듯
.section.변수나 함수명 이런식으로 되어있는데
느낌상 컴파일러 관련된것들은 단순 .text 이렇게 표현하는듯?

 .rodata.AHBPrescTable

_sidata는 플래시 메모리
_sdata는 램 데이터 섹션 시작부분임


glue_7 이나 glue_7t 섹션은 instruction mode switching
그니깐 thumb모드 스위칭인듯?? 근데 큐브ide에선 안써서 그냥 버림.
섹션에는 있는데 content에 없다.

.eh_frame은 c++에서 예외 throw된 직후에 스택 푸는 operation도중에 생기는듯
쨋든 c++에서만 쓰고 여기서는 c만 쓰니 discard

.init .fini는 c run time 라이브러리에서 씀.


CCM : core coupled memory 
I,D버스와 연결되어있고 메모리 접근할때 0 wait이라 빠름..
critical data를 주로 저장함..(맞긴한데.. 단순히 이거라면 SRAM도 가능)
주된 이유는 하버드 구조라 동시에 I,D 받아오는데 I를 CCM D를 SRAM 사용하는식으로 사용..

cache메모리
- write through no write allocate
값은 메모리에다 저장하는데 캐시에는 저장안함
- write back no write allocate
??? 주메모리에 저장안하고 캐시로 가져옴?
- write back write&read allocate

strongly ordered : ldr,sta 끝날때까지 기다리고 다음 명령 수행
shared device : 정확한 순서대로 메모리에 로드,저장
shareable : multi bus master 끼리 동기화
TEX C B Memory Type 		Description 						Shareable
000 0 0 Strongly Ordered 	Strongly Ordered					Yes 
000 0 1 Device 				Shared Device 						Yes
000 1 0 Normal 				Write through, no write allocate 	S bit dependent
000 1 1 Normal 				Write-back, no write allocate 		S bit dependent
001 0 0 Normal 				Non-cacheable 						S bit dependent
001 0 1 Reserved 			Reserved 							Reserved
001 1 0 Undefined 			Undefined 							Undefined
001 1 1 Normal 				Write-back, write and read allocate S bit dependent
010 0 0 Device 				Non-shareable device 				No
010 0 1 RESERVED 			RESERVED 							RESERVED

ROM,Flash는 normal memory write-through c=1 b=0 s-0
내부 SRAM	   동일							c=1 b=0 s=1
외부 ram(fmc) noraml       write back		c=1 b=1 s=1
페리퍼럴		device		shareable devices c=0 b=1 s=1

mem manage fault
- 접근권한 오류?(mpu,processor에 의해)
- thread모드에서 privileged access할때
- read-only 메모리 구간에 write할때
- XN 영역에 execute할때
- 요약하자면 MPU와 관련있는듯..

bus fault
- 명령어 fetch, 데이터 읽고 쓰고할때 오류 생길경우
- vector fetch할때는 hard fault로 승격
- 잘못된 주소로 접근
- device가 메모리 전송할 준비가 안됐을경우
- 외부 메모리 문제
- thread mode에서 NVIC접근할때(SCB 영역 접근같은거 말하는듯. 이런 영역의 peripheral?)

usage fault
- 정의되지 않은 명령어
- floating point 명령어 없는데 수행할때(fpu없을경우에만 해당)
- 인터럽트/exception이 still active 인데 thread 로 넘어갈경우
- 0으로 나눌때
- 정렬되지않은 메모리 접근(multiple store&load)
- 정렬되지않은 메모리

naked 컴파일러 지시자는 어셈블리어와 c랑 섞어서 쓰려는듯? 아닌가?
함수명에다 naked atrritube해주고 안에 어셈블리어 넣음

SVC
- thumb ISA instruction이용해서 호출
SVC #svc number하면 됨