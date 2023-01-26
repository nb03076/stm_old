/* Task Creation */
TaskHandle_t

BaseType_t xTaskCreate(    TaskFunction_t pvTaskCode,
                            const char * const pcName,
                            configSTACK_DEPTH_TYPE usStackDepth,
                            void *pvParameters,
                            UBaseType_t uxPriority,
                            TaskHandle_t *pxCreatedTask
                       );
						  
void vTaskDelete( TaskHandle_t xTask );


ex)
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

/* Task Control */
void vTaskDelay( const TickType_t xTicksToDelay );
void vTaskDelayUntil( TickType_t *pxPreviousWakeTime,
                      const TickType_t xTimeIncrement );

ex)
TickType_t xLastWakeTime;
const TickType_t xFrequency = 10;

     // Initialise the xLastWakeTime variable with the current time.
     xLastWakeTime = xTaskGetTickCount();

     for( ;; )
     {
         // Wait for the next cycle.
         vTaskDelayUntil( &xLastWakeTime, xFrequency );

         // Perform action here.
     }
	 
x버전도 있음 x면 pdPASS면 이미 delay 끝 pdFalse면 delay중

UBaseType_t uxTaskPriorityGet( TaskHandle_t xTask );
우선순위 반환함

void vTaskPrioritySet( TaskHandle_t xTask,
                       UBaseType_t uxNewPriority );
					   z`
void vTaskSuspend( TaskHandle_t xTaskToSuspend );
void vTaskResume( TaskHandle_t xTaskToResume );
BaseType_t xTaskResumeFromISR( TaskHandle_t xTaskToResume );

ex)
 // Resume the suspended task.
     xYieldRequired = xTaskResumeFromISR( xHandle );

     // We should switch context so the ISR returns to a different task.
     // NOTE:  How this is done depends on the port you are using.  Check
     // the documentation and examples for your port.
     portYIELD_FROM_ISR( xYieldRequired );

이거 suspend에 있는걸 푸는거임.. wait은 아니야


/* RTOS Kernel Control */
void taskENTER_CRITICAL( void );
void taskEXIT_CRITICAL( void );
이것들은 인터럽트보다 우선순위가 높아서 인터럽트가 안걸리는걸 볼 수 있다.

UBaseType_t taskENTER_CRITICAL_FROM_ISR( void );
void taskEXIT_CRITICAL_FROM_ISR( UBaseType_t uxSavedInterruptStatus );
ex)
UBaseType_t uxSavedInterruptStatus;
uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );



taskYield();
void vTaskStartScheduler( void );

void vTaskSuspendAll( void );
BaseType_t xTaskResumeAll( void );



vTaskStepTick 이건 저전력 idle tickless 관련
BaseType_t xTaskCatchUpTicks( TickType_t xTicksToCatchUp );


/* Direct To Task Notifications */
BaseType_t xTaskNotifyGive( TaskHandle_t xTaskToNotify );
BaseType_t xTaskNotifyGiveIndexed( TaskHandle_t xTaskToNotify, 
                                    UBaseType_t uxIndexToNotify );
다른 태스크에게 보낼 태스크 핸들을 지정해야함
ex) 태스크1에서 태스크2에게 보내고싶다 그러면 태스크 1에서 핸들을 태스크2핸들로 잡아서
xTaskNotifyGive해줘야함									

indexed는 배열 내의 모든 작업 알림에서 작동할 수 있다

 void vTaskNotifyGiveFromISR( TaskHandle_t xTaskToNotify,
                              BaseType_t *pxHigherPriorityTaskWoken );

 void vTaskNotifyGiveIndexedFromISR( TaskHandle_t xTaskHandle, 
                                     UBaseType_t uxIndexToNotify, 
                                     BaseType_t *pxHigherPriorityTaskWoken)
ISR버전에서는 uxIndexToNotify 항상 0

BaseType_t xHigherPriorityTaskWoken = pdFALSE;
vTaskNotifyGiveIndexedFromISR( xTaskToNotify, 0, &xHigherPriorityTaskWoken );
portYIELD_FROM_ISR( xHigherPriorityTaskWoken ); // pdTRUE로 바꿈...



