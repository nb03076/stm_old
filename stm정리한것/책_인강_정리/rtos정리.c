BaseType_t xTaskCreate(    TaskFunction_t pvTaskCode,
                            const char * const pcName,
                            configSTACK_DEPTH_TYPE usStackDepth,
                            void *pvParameters,
                            UBaseType_t uxPriority,
                            TaskHandle_t *pxCreatedTask
                          );
						 
						 
void vBlueLedControllerTask(void *pvParameters)
{
		while(1)
		{
			//To Do...
		};
}


xTaskCreate(vBlueLedControllerTask,
			"Blue Led Controller",
			100,
			NULL,
			1,
			&blue_handle);
			
vStartScheduler(); 해주면 시작함

pvParameters 쓸거면
#define blue GPIO_PIN_13
const uint16_t *blue_led = (uint16_t *)blue
태스크 안에 HAL_GPIO_TogglePin(GPIOD,(uint16_t)pvParameters);

TaskHandle_t blue_handle  //control하기 위해 사용

vTaskPrioritySet(green_handle,3) 이러면 우선순위 3으로 변경
vTaskPrioritySet(NULL,1)이면 현재 태스크의 우선순위 1로 변경

uint32_t green_priority = uxTaskPriorityGet(green_handle)
 if( uxTaskPriorityGet( xHandle ) != tskIDLE_PRIORITY )
 if( uxTaskPriorityGet( xHandle ) < uxTaskPriorityGet( NULL ) )

vTaskSuspend(blue_handle);
vTaskResume(blue_handle);
vTaskDelete(NULL);

block state 발생하는 경우
- time related event
- synchronization event

vTaskDelay(TickType_t xTicksToDelay)
pdMS_TO_TICKS

void vBlueLedControllerTask(void *pvParameters)
{
		TickType_t xLastWakeTime;
		const TickType_t xPeriod = pdMS_TO_TICKS(100);
		xLastWakeTime = xTaskGetTickCount();
		while(1)
		{
			vTaskDelayUntil(&xLastWakeTime,xPeriod);
		};
}

task create 에러처리는
/* Task to be created. */
void vTaskCode( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below. 
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    for( ;; )
    {
        /* Task code goes here. */
    }
}

/* Function that creates a task. */
void vOtherFunction( void )
{
BaseType_t xReturned;
TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
                    vTaskCode,       /* Function that implements the task. */
                    "NAME",          /* Text name for the task. */
                    STACK_SIZE,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,/* Priority at which the task is created. */
                    &xHandle );      /* Used to pass out the created task's handle. */

    if( xReturned == pdPASS )
    {
        /* The task was created.  Use the task's handle to delete the task. */
        vTaskDelete( xHandle );
    }
}

 xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
 이거는 1이면 delay된거고 0이면 delay가 안된거임
 만약 waketime할 시간이 현재보다 과거에 있다면 fail됨...
 
 
 이건 idle일때 실행하는거... 기본적으로는 아무것도 안함
 void vApplicationIdleHook(void)
 {
	 //clean up 관련해서 작성하는듯?
 }
 
 tick hook(tick callback)은 틱 인터럽트 발생할때 마다 커널이 호출
 여기는 인터럽트니깐 FromISR같은 함수를 사용해야함. 스케줄링함수 못쓰는듯
 
 resumefromisr
  void vAnExampleISR( void )
 {
     BaseType_t xYieldRequired;

     // Resume the suspended task.
     xYieldRequired = xTaskResumeFromISR( xHandle );

     // We should switch context so the ISR returns to a different task.
     // NOTE:  How this is done depends on the port you are using.  Check
     // the documentation and examples for your port.
     portYIELD_FROM_ISR( xYieldRequired );
 }
 
 
 blcoking queue read
 - 큐가 비어있으면 block. 큐에 내용이 들어왔다면 자동으로 ready
 
 blocking queue write
 - 큐가 가득 차있으면 block. 큐에 공간이 생겼으면 자동으로 ready
 
QueueHandle_t xQueueCreate( UBaseType_t uxQueueLength,
                             UBaseType_t uxItemSize );
							 
BaseType_t xQueueSend(
                        QueueHandle_t xQueue,
                        const void * pvItemToQueue,
                        TickType_t xTicksToWait
                     );
					 
BaseType_t xQueueReceive(
                            QueueHandle_t xQueue,
                            void *pvBuffer,
                            TickType_t xTicksToWait
                        );
						
						
Queue sets : 하나보다 많은 큐로부터 데이터를 받음 (각각의 큐에서 task polling 없이)


BaseType_t xQueueAddToSet
                    (
                        QueueSetMemberHandle_t xQueueOrSemaphore,
                        QueueSetHandle_t xQueueSet
                    );
					
					
ex) 메시지 큐
void sender(void *pvParameters)
void receiver(void *pvParameters)

QueueHandle_t yearQueue

yearQueue = xQueueCreate(5,sizeof(int32_t))

xTaskCreate:sender,receiver 생성. 둘다 우선순위1로 둠 일단..

void sender(void *pvParameters)
{
	int32_t value = 2050;
	BaseType_t qStatus;
	
	while(1)
	{
		qStatus = xQueueSend(yearQueue,&value_to_send,0); // 0은 do not wait
	
		if(qStatus != pdPASS)
		{
			//error handling
		}
	}
}

void receiver(void *pvParameters)
{
	int32_t value_received;
	BaseType_t qStatus;
	
	while(1)
	{
		qStatus = xQueueReceive(yearQueue,&value,pdMS_TO_TICKS(100));
	
		if(qStatus != pdPASS)
		{
			//error handling
		}
	}
}






receiver task2개에 우선순위1 sender는 우선순위2로 해서 작동...



sender sensor2개 우선순위2 receive는 1
typedef enum
{
	humidity_sensor,
	pressure_sensor
}DataSource_t

typedef struct
{
	uint8_t value;
	DataSource_t Datasource;
}Data_t;

static const Data_t xStructToSend[2] =
{
	{77,humidity_sensor},
	{66,pressure_sensor}
};

xQueue = xQueueCreate(3,sizeof(Data_t));

xqueuereceive하고
if(xReceivedStructure.sDataSource == humidity_sensor)
	이런식으로 해서 사용가능함...


QueueHandle_t xQueue1,xQueue2
이걸 QueueSetHandle_t qset
xQueue1,2 = xQueueCreate(1,sizeof(char*))

xQueueSet = xQueueCreateSet(1 * 2)

xQueueAddToSet(xQueue1,xQueueSet)
xQueueAddToSet(xQueue2,xQueueSet)

태스크에서 xQueue1,2는 send 하고 receive할때는
xQueueSelectFromSet써서 받음
태스크 내부에 QueueHandle_t contain;
while 내부에
contain = (QueueHandle_t) xQueueSelectFromSet(xQueueSet,portMAX_DELAY)
xQueueReceive(contain,&anme,0)

/* 세마포어 */
바이너리 세마포어 : 0,1  signal or not
카운팅 세마포어 : 특정 리소스 접근가능 수
뮤텍스 : multiple tasks

SemaPhoreHandle_t sem
take,give

gatekeeper task
1. gatekeeper task는 우선순위 낮게 해야댐
2. 우선순위 높은 태스크가 queuesend
3. gatekeeper 는 우선순위 낮으면 queuereceive

software timers
- timer callback
- auto reload : periodic
- oneshot : only once

