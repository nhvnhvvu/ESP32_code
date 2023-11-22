#include<Arduino.h>

TaskHandle_t Task1;

void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  for(;;){
    Serial.print("First");
    delay(1000);
  } 
}

// ISR: Interupt Service Routine: Các hàm sẽ được thực thi khi xảy ra ngắt(thường được lưu trong IRAM_ATTR)



void setup(){
  // TÁC VỤ

  // create task
  xTaskCreatePinnedToCore(
                      Task1code,   /* Task function.(must be never return like infinity loop or be terminated) */
                      "Task1",     /* name of task.(max length = 16) */
                      10000,       /* Stack size of task in bytes */
                      NULL,        /* parameter of the task */
                      1,           /* priority of the task(min la 0 max la configMAX_PRIORITIES - 1) */
                      &Task1,      /* Task handle to keep track of created task */
                      0);          /* pin task to core 0 */

  xTaskCreate(
                      Task1code,
                      "Task1",
                      10000,
                      NULL,
                      1,
                      &Task1); // mac dinh chay tren core 1

  // CONFIG TÁC VỤ

  vTaskPrioritySet(&Task1,2); // set priority for task
  uxTaskPriorityGet(&Task1); //get priority for task

  // QUẢN LÝ TÁC VỤ

  vTaskSuspend(&Task1); //suspend task1 , null to suspend itself 
  vTaskSuspendAll();
  vTaskResume(&Task1); //resume task1
  xTaskResumeAll();
  vTaskDelete(&Task1); //delete task1 , null to delete itself
  vTaskDelay(500/portTICK_PERIOD_MS); //delay task for 500ms

  //TaskDelay se delay task 500ms ke tu thoi diem ham dc goi
  //nen neu muon delay theo chu ky se ko dc vi thoi diem ham dc
  //goi se thay doi tuy theo thoi gian code o phia duoi chay
  //muon delay theo chu ky se su dung TaskDelayUntil 
  
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount (); // lay so tick hien tai
  vTaskDelayUntil( &xLastWakeTime, 500/portTICK_PERIOD_MS );

  // HÀNG ĐỢI

  // hàng đợi có 1 độ dài nhất định, nếu một tác vụ muốn ghi vào 1 hàng chờ đầy,
  // nó sẽ bị block và sẽ được unblock nếu có không gian khả dụng
  // nếu một tác vụ muốn lấy dữ liệu từ 1 hàng chờ trống, nó cũng sẽ bị block
  // một hàng đợi sẽ gồm 2 khối là khối QueueStorage và khối QueueBuffer
  // nếu khai báo bằng QueueCreate thì 2 khối này sẽ được cấp phát động
  // nếu khai báo bằng QueueCreateStatic thì 2 khối này sẽ được cấp phát tĩnh
  
  // KHỞI TẠO HÀNG ĐỢI

  QueueHandle_t xQueue1, xQueue2;

  // Create a queue capable of containing 10 uint32_t values.
  xQueue1 = xQueueCreate( 10, sizeof( uint32_t ) ); // xQueueCreate(uxQueueLength, uxItemSize); trả về 1 QueueHandle nếu tạo thành công
  
  #define QUEUE_LENGTH 10
  #define ITEM_SIZE sizeof( uint32_t )
  StaticQueue_t xQueueBuffer;// xQueueBuffer will hold the queue structure.
  uint8_t ucQueueStorage[ QUEUE_LENGTH * ITEM_SIZE ];// ucQueueStorage will hold the items posted to the queue
  
  // Create a queue capable of containing 10 uint32_t values, lưu trữ static
  xQueue1 = xQueueCreate( QUEUE_LENGTH, // The number of items the queue can hold.
                          ITEM_SIZE     // The size of each item in the queue
                          &( ucQueueStorage[ 0 ] ), // The buffer that will hold the items in the queue.
                          &xQueueBuffer ); // The buffer that will hold the queue structure.

  // GHI DỮ LIỆU VÀO HÀNG ĐỢI

  xQueueSendToBack(xQueue, pvItemToQueue, xTicksToWait)  // gửi dữ liệu vào đằng sau của queue
  xQueueSendToFront(xQueue, pvItemToQueue, xTicksToWait) //gửi dữ liệu vào đằng trước của queue
  // xQueue – The handle to the queue on which the item is to be posted.
  // pvItemToQueue – A pointer to the item that is to be placed on the queue. The size of the items the queue will hold was defined when the queue was created, so this many bytes will be copied from pvItemToQueue into the queue storage area.
  // xTicksToWait – The maximum amount of time the task should block waiting for space to become available on the queue, should it already be full. The call will return immediately if this is set to 0 and the queue is full. The time is defined in tick periods so the constant portTICK_PERIOD_MS should be used to convert to real time if this is required.

  // Lưu ý nếu hàm gửi dữ liệu vào Queue được thực thi bởi ISR thì phải thêm vào thành
  xQueueSendToFrontFromISR(xQueue, pvItemToQueue, pxHigherPriorityTaskWoken)
  xQueueSendToBackFromISR(xQueue, pvItemToQueue, pxHigherPriorityTaskWoken)
  // pxHigherPriorityTaskWoken - bằng pdFALSE thì sẽ ghi kể cả khi Queue đầy và sẽ làm cho Task khác bị dừng

  xQueueReset(xQueue)// xóa hết dữ liệu trong Queue
  vQueueDelete(QueueHandle_t xQueue) //xóa queue

  uint32_t ulVar = 10UL;
  xQueueSendToFront( xQueue1, ( void * ) &ulVar, ( TickType_t ) 10 ); // 

  // ĐỌC DỮ LIỆU TỪ HÀNG ĐỢI

  xQueuePeek(QueueHandle_t xQueue, void *const pvBuffer, TickType_t xTicksToWait)// copy giá trị từ queue vào buffer mà không xóa giá trị đó trong Queue
  BaseType_t xQueueReceive(QueueHandle_t xQueue, void *const pvBuffer, TickType_t xTicksToWait)// copy giá trị từ queue vào buffer nếu thành công thì xóa giá trị đó trong Queue
  // pvBuffer – Pointer to the buffer into which the received item will be copied. Độ lớn của buffer phải bằng độ lớn của 1 phần tử trong queue

  // nếu được thực thi bởi ISR
  xQueuePeekFromISR(QueueHandle_t xQueue, void *const pvBuffer)
  xQueueReceiveFromISR(QueueHandle_t xQueue, void *const pvBuffer, BaseType_t *const pxHigherPriorityTaskWoken)

  uxQueueMessagesWaiting(const QueueHandle_t xQueue) // số phần tử còn lại trong queue
  uxQueueSpacesAvailable(const QueueHandle_t xQueue) // số chỗ trống còn lại trong queue

  // SEMAPHORE : ĐỒNG BỘ TÁC VỤ, QUẢN LÝ TÀI NGUYÊN

  // một semaphore chứa 1 số lượng token, các tác vụ cần nhận được token để được thực thi
  // nếu như các token đều đã được sử dụng, các tác vụ yêu cầu token sẽ bị đình chỉ cho đến khi
  // một số token khác được giải phóng. Token có thể là event trigger hoặc tài nguyên

  // có 2 loại semaphore: binary và counting
  // BINARY SEMAPHORE: chỉ có duy nhất 1 phần tử dữ liệu. Minh họa 1 quy trình hoạt động như sau:
  // ban đầu semaphore trống, task bị đình chỉ
  // sau đó có ngắt xảy ra, ISR đặt 1 semaphore vào, lúc này task sẽ lấy semaphore đó ra và bắt đầu được thực thi
  // semaphore lúc này lại trống và đợi cho lần ngắt tiếp theo
  
  SemaphoreHandle_t xSemaphoreCreateBinary()
  xSemaphoreTake(xSemaphore, xBlockTime) //return pdTRUE if the semaphore was obtained. pdFALSE if xBlockTime expired without the semaphore becoming available
  xSemaphoreGive(xSemaphore) //return pdTRUE if the semaphore was released. pdFALSE if an error occurred.
  xSemaphoreGiveFromISR(xSemaphore, pxHigherPriorityTaskWoken)
  // xSemaphore – A handle to the semaphore being taken - obtained when the semaphore was created.
  // xBlockTime – The time in ticks to wait for the semaphore to become available. The macro portTICK_PERIOD_MS can be used to convert this to a real time.

  // COUNTING SEMAPHORE: event handler give 1 semaphore when event occur and handle task take 1 semaphore when it handle an event
  
  SemaphoreHandle_t xSemaphoreCreateCounting(uxMaxCount, uxInitialCount)
  // uxMaxCount – The maximum count value that can be reached. When the semaphore reaches this value it can no longer be ‘given’.
  // uxInitialCount – The count value assigned to the semaphore when it is created.

  // MUTEX: cơ chế hạn chế truy cập vào cùng 1 tài nguyên
  
  // có cơ chế giống Binary semaphore nhưng được tích hợp thêm cơ chế kế thừa mức ưu tiên để hạn
  // chế truy cập vào cùng 1 tài nguyên giữa nhiều tác vụ
  // ví dụ: token trong mutex là duy nhất, nghĩa là task nào lấy mutex thì mutex đó sẽ trống cho đến khi
  // token được trả lại, trong khi với binary semaphore thì task nào cũng có thể đặt token vào đó
  // quy trình như sau: task prior thấp đang chiếm token, task prior cao yêu cầu token thì sẽ chiếm token đó
  // nhưng chưa được thực thi ngay, task prior thấp trả token thì task prior cao sẽ được thực thi ngay lập tức
  SemaphoreHandle_t xSemaphoreCreateMutex()


  // KẾT NỐI TÁC VỤ
  
  // sử dụng khái niệm Events flags và Task flags

  // tập hợp các Events flags và Events bits gọi là Events Group
  // Events group có 1 số lượng các bit ứng với các cờ để báo hiệu sự kiện có xảy ra hay không
  EventGroupHandle_t xEventGroupCreate()
  EventBits_t xEventGroupWaitBits(EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToWaitFor, const BaseType_t xClearOnExit, const BaseType_t xWaitForAllBits, TickType_t xTicksToWait) //block to wait for one or more bits to be set within a previously created event group
  EventBits_t xEventGroupClearBits(EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToClear) //Clear bits within an event group
  EventBits_t xEventGroupSetBits(EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet) // Set bits
  EventBits_t xEventGroupGetBitsFromISR(EventGroupHandle_t xEventGroup)
  EventBits_t xEventGroupClearBitsFromISR(EventGroupHandle_t xEventGroup,const EventBits_t uxBitsToClear)
  EventBits_t xEventGroupSetBitsFromISR(EventGroupHandle_t xEventGroup,const EventBits_t uxBitsToSet, pxHigherPriorityTaskWoken)

  EventBits_t uxBits;
  EventGroupHandle_t xEventGroup;
  #define BIT_0 ( 1 << 0 )
  #define BIT_4 ( 1 << 4 )
  const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS;
  EventBits_t uxBits = xEventGroupWaitBits(
              xEventGroup,    // The event group being tested.
              BIT_0 | BIT_4,  // The bits within the event group to wait for.
              pdTRUE,         // BIT_0 and BIT_4 should be cleared before returning.
              pdFALSE,        // Don't wait for both bits, either bit will do.
              xTicksToWait ); // Wait a maximum of 100ms for either bit to be set.


  // Task flags là 1 dạng đặc biệt của Events flags


  // SOFTWARE TIMER: BỘ ĐẾM THỜI GIAN PHẦN MỀM
  
  // có 2 loại timer: one-shot timer và auto-reload timer 
  // one-shot timer chỉ gọi hàm callback 1 lần khi đến thời gian và không tự khởi động lại (có thể khởi động thủ công)
  // auto-reload timer sẽ gọi hàm callback và tự khởi động lại

  // các timer được chạy trên 1 daemon task , để quản lý các timer thì có 1 queue gọi là timer command queue
  
  // KHỞI TẠO TIMER
  
  TimerHandle_t xTimerCreate(const char *const pcTimerName, const TickType_t xTimerPeriodInTicks, const UBaseType_t uxAutoReload, void *const pvTimerID, TimerCallbackFunction_t pxCallbackFunction)
  // pcTimerName – Timer name in text
  // xTimerPeriodInTicks – The timer period. The time is defined in tick periods so the constant portTICK_PERIOD_MS can be used to convert a time that has been specified in milliseconds. 
  // uxAutoReload – If uxAutoReload is set to pdTRUE then the timer will expire repeatedly with a frequency set by the xTimerPeriodInTicks parameter. If uxAutoReload is set to pdFALSE then the timer will be a one-shot timer and enter the dormant state after it expires.
  // pvTimerID – Timer ID (để biết xem timer nào đã gọi hàm callback khi hàm callback được gán với nhiều timer)
  // pxCallbackFunction – The function to call when the timer expires. Callback functions must have the prototype defined by TimerCallbackFunction_t, which is “void vCallbackFunction( TimerHandle_t xTimer );

  void *pvTimerGetTimerID(const TimerHandle_t xTimer);
  BaseType_t xTimerStart( TimerHandle_t xTimer, TickType_t xTicksToWait ); //bắt đầu timer
  BaseType_t xTimerStop( TimerHandle_t xTimer, TickType_t xTicksToWait );
  BaseType_t xTimerChangePeriod( TimerHandle_t xTimer, TickType_t xNewPeriod, TickType_t xTicksToWait );
  BaseType_t xTimerDelete( TimerHandle_t xTimer, TickType_t xTicksToWait );
  BaseType_t xTimerReset( TimerHandle_t xTimer, TickType_t xTicksToWait );



}



void loop(){
    
}


