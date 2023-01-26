https://www.keil.com/pack/doc/CMSIS/RTOS/html/functionOverview.html#CMSIS_RTOS_TimeOutValue

#x면 문자열로... ##y은 인자에다 붙여서 사용..


#define PRINT_A(x) printf(#x " = %d\n",x)
#define CAT(x,y) (x##y)

int main()
{
	int a1, b1;

	CAT(a, 1) = 10; // 이건 a1 = 10이랑 같음
	CAT(b, 1) = 20;

	PRINT_A(a1+b1);
}


6. Threads and tasks
cmsis  rtos
#include "cmsis_os.h"

osThreadDef(thread_name,osPriorityNormal,1,0)

osThreadId thread_id
thread_id = osThreadCreate(osThread(thread_name),NULL)
if(thread_id == NULL){}
osThreadTerminate(thread_id)

위의거는 v1임 v1은 일반적인 m시리즈 통용인데
v2는 ARMv8M까지 호환임.. 미래를 볼때는 v2쓰는게 맞을지도..?
근데 freeRTOS에는 없는 기능이 너무많아... 일단 v1으로 공부하자

thread task
- solve different problems
- 큰걸 작은 걸로 분할
- 분할정복 알고리즘


struct osThreadDef_t
(?,os_pthread(스레드 함수 시작 주소),
스택 사이즈, 우선순위(osPriority))

typedef void (*os_pthread)(void const *argument)

struct osSemaphoreDef_t 는 더미..
struct osMutexDef_t 더미
struct osMessageQDef_t
사이즈,메모리,큐에 얼마나 들어가있는가

struct osPoolDef_t
아이템 사이즈, 메모리풀 포인터, 메모리풀에 얼마나 들어가있는가
struct osMailQDef_t
아이템 사이즈, 메모리, 큐에 들어간 요소의 수
struct osEvent
이벤트 정의,	값, 				상태
(메일,메시지)  포인터,시그널플래그,값	osStatus


#define osThreadDef	(	name,
						priority,
						instances,
						stacksz 
)		




/* v1 api */

osThreadId 	osThreadCreate (const osThreadDef_t *thread_def, void *argument)
xTaskCreate랑 같음

osThreadId osThreadGetId (void)
xTaskGetCurrentTaskHandle();

ex)  
osThreadId id;                                           // id for the currently running thread
id = osThreadGetId ();
if (id == NULL) {
   // Failed to get the id;
}

osStatus osThreadTerminate (osThreadId thread_id)
vTaskDelete(thread_id);

ex)
status = osThreadTerminate (id);                         // stop the thread
if (status == osOK) {
  // Thread was terminated successfully
}
else {
    // Failed to terminate a thread
}


osStatus osThreadYield (void)
taskYIELD();

ex)
status = osThreadYield();                              // 
if (status != osOK)  {
  // thread switch not occurred, not in a thread function
}

osStatus osThreadSetPriority (osThreadId thread_id, osPriority priority)
vTaskPrioritySet(thread_id, makeFreeRtosPriority(priority));

ex)
osThreadId id;                                           // id for the currently running thread
osPriority pr;                                           // thread priority
osStatus   status;                                       // status of the executed function
     
id = osThreadGetId ();                                   // Obtain ID of current running thread
   
if (id != NULL) {
  status = osThreadSetPriority (id, osPriorityBelowNormal);
  if (status == osOK)  {
     // Thread priority changed to BelowNormal
  }
  else {
     // Failed to set the priority
  }
}
else  {
    // Failed to get the id
}


osPriority osThreadGetPriority (osThreadId thread_id)
IPSR(irq 수행하고 있으면 해당 irq number 있음) 이게 0이면 스레드모드 그외에는 핸들러모드
uxTaskPriorityGetFromISR  /  uxTaskPriorityGet

ex)
osThreadId id;                                           // id for the currently running thread
osPriority priority;                                     // thread priority
   
id = osThreadGetId ();                                   // Obtain ID of current running thread
   
if (id != NULL)  {
   priority = osThreadGetPriority (id);
}
else {
    // Failed to get the id
}


#define osThreadDef(name, thread, priority, instances, stacksz)  \
const osThreadDef_t os_thread_def_##name = \
{ #name, (thread), (priority), (instances), (stacksz), NULL, NULL }
여기서 instance는 몇개 만들거냐는 뜻 3이면 3개 만든다는거임

#define osThread	(name)	 &os_thread_def_##name

typedef struct os_thread_def  {
  char                   *name;        ///< Thread name 
  os_pthread             pthread;      ///< start address of thread function
  osPriority             tpriority;    ///< initial thread priority
  uint32_t               instances;    ///< maximum number of instances of that thread function
  uint32_t               stacksize;    ///< stack size requirements in bytes; 0 is default stack size
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
  uint32_t               *buffer;      ///< stack buffer for static allocation; NULL for dynamic allocation
  osStaticThreadDef_t    *controlblock;     ///< control block to hold thread's data for static allocation; NULL for dynamic allocation
#endif
} osThreadDef_t;
os_pthread 는 typedef void(* os_pthread)(void const *argument)



스레드 생성 예제
void Thread (void const *arg);                           
osThreadDef (TaskName, Thread, osPriorityNormal, 3, 0);     
osThreadId id1, id2, id3;
id1 = osThreadCreate (osThread (Thread), NULL);   
id2 = osThreadCreate (osThread (Thread), NULL);      
id3 = osThreadCreate (osThread (Thread), NULL);
if(id1 == NULL){
	에러 처리
}

osThreadId id1;
osThreadDef(led1,led_task1,osPriorityNormal,0,128);
id1 = osThreadCreate(osThread(led1),NULL);

void led_task1(const void *argument)
{
	for(;;)
	{
	}
}


/* delay */
osStatus osDelay(uitn32_t millisec)
vTaskDelay

osStatus osDelayUntil (uint32_t *PreviousWakeTime, uint32_t millisec)
@previous : 현재시간 osKernelSysTick() 이용  millisec : 지연하고 싶은 시간

osWait(uint32_t millisec)
시그널,메시지,메일 기다림. 근데 freeRTOS에는 없다














v2
struct osThreadAttr_t
name,osThreadDetached(0) or osThreadJoinable(1)
메모리 블럭(동적할당이면 NULL static경우 넣어줘야함)
메모리 사이즈(마찬가지)
스택 메모리,스택 사이즈,우선순위,?? trust zone

void(* osThreadFunc_t)(void *argument)
osThreadId_t
osThreadState_t
osThreadInactive,ready,running
blocked,terminated(리소스 free안됨),error(존재안함)

enum osPriority_t
osPriorityIdle
Low Normal High등등 엄청많다...

osThreadId_t osThreadNew	(	osThreadFunc_t 	func,
void * 	argument,
const osThreadAttr_t * 	attr 
)osKernelStart하기전에 태스크 생성하자..


const char* name = osThreadGetName(thread_id);
  
 osThreadId_t id;// id for the currently running thread
 id = osThreadGetId();
 id == NULL이면 에러 처리해주자
  
osThreadState_t osThreadGetState(osThreadId_t thread_id	)

osStatus_t osThreadSetPriority(osThreadId_t thread_id,
osPriority_t priority)

osPriority_t osThreadGetPriority(osThreadId_t thread_id	)	

osStatus_t osThreadYield(void)	
ex)status = osThreadYield();
    if (status != osOK) {
      // an error occurred
    }


osStatus_t osThreadSuspend	(	osThreadId_t 	thread_id	)	
osStatus_t osThreadResume	(	osThreadId_t 	thread_id	)	


osStatus_t osThreadDetach	(	osThreadId_t 	thread_id	)	
attribute를 detach로 바꾸는거임
osStatus_t osThreadJoin	(	osThreadId_t 	thread_id	)	


__NO_RETURN void osThreadExit	(void )	
스레드에서 이거쓰면 join한거 동기화 가능


osStatus_t osThreadTerminate	(	osThreadId_t 	thread_id	)	
스레드 삭제


uint32_t osThreadGetStackSize	(	osThreadId_t 	thread_id	)	
사용하고 있는 스택 사이즈
uint32_t osThreadGetStackSpace	(	osThreadId_t 	thread_id	)	
남아있는 스택공간 사이즈


uint32_t osThreadGetCount	(	void 		)	
동작하고 있는 스레드 수


uint32_t osThreadEnumerate	(	osThreadId_t * 	thread_array,
uint32_t 	array_items 
)	
쓰긴하려나.. thread_array가 실제, 아이템은 갯수


/* 스레드 플래그 */
// Flags options (\ref osThreadFlagsWait and \ref osEventFlagsWait).
#define osFlagsWaitAny        0x00000000U ///< Wait for any flag (default).
#define osFlagsWaitAll        0x00000001U ///< Wait for all flags.
#define osFlagsNoClear        0x00000002U ///< Do not clear flags which have been specified to wait for.

// Flags errors (returned by osThreadFlagsXxxx and osEventFlagsXxxx).
#define osFlagsError          0x80000000U ///< Error indicator.
#define osFlagsErrorUnknown   0xFFFFFFFFU ///< osError (-1).
#define osFlagsErrorTimeout   0xFFFFFFFEU ///< osErrorTimeout (-2).
#define osFlagsErrorResource  0xFFFFFFFDU ///< osErrorResource (-3).
#define osFlagsErrorParameter 0xFFFFFFFCU ///< osErrorParameter (-4).
#define osFlagsErrorISR       0xFFFFFFFAU ///< osErrorISR (-6).

uint32_t osThreadFlagsSet	(	osThreadId_t 	thread_id,
uint32_t 	flags 
)	