#include "main.h"


//* ---------------------------------- 开始任务 ---------------------------------- */

//任务优先级
#define START_TASK_PRIO 1
//任务堆栈大小
#define START_STK_SIZE 128
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

/* --------------------------------- INIT任务 --------------------------------- */

#define RANGING_TASK_PRIO 28
#define RANGING_STK_SIZE 1024
TaskHandle_t RANGINGTask_Handler;
void ranging_task(void *pvParameters);

/* --------------------------------- USART3 POLLING任务 --------------------------------- */

#define USART3_TASK_PRIO 27
#define USART3_STK_SIZE 256
TaskHandle_t USART3Task_Handler;
void usart3_task(void *pvParameters);


/* --------------------------------- UART4 POLLING任务 --------------------------------- */

#define UART4_TASK_PRIO 26
#define UART4_STK_SIZE 256
TaskHandle_t UART4Task_Handler;
void uart4_task(void *pvParameters);

/* -------------------------------------------------------------------------- */
/*                                     主函数                                    */
/* -------------------------------------------------------------------------- */

void APP_RFID_DataProcessing(uint16_t *rfid_buff, uint8_t train_up_down, uint8_t train_forward, uint8_t train_backward);

int main(void)
{
    BSP_MPU_Config();
    CPU_CACHE_Enable();
    BSP_SystemClock_Config();
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    BSP_GPIO_Init();
    BSP_Delay_Init();
    BSP_SPI1_Init();
    BSP_Usart3_Init();
    BSP_Uart4_Init();
    BSP_Time7_Init();
    BSP_RNG_Init();
    BSP_I2C_Init();
    BSP_SetUsartRXHOOK(CB_Usart_RX);
    BSP_SetUart4RXHOOK(CB_Uart4_RX); 
    APP_DWTXRXInit();
    BSP_Delay_ms(100);
    /* --------------------------------- 创建开始任务 --------------------------------- */
    xTaskCreate((TaskFunction_t)start_task,          //任务函数
                (const char *)"start_task",          //任务名称
                (uint16_t)START_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&StartTask_Handler); //任务句柄
    vTaskStartScheduler();                           //开启任务调度
}

/* -------------------------------------------------------------------------- */
/*                                  开始任务任务函数                                  */
/* -------------------------------------------------------------------------- */

void start_task(void *pvParameters)
{
    taskENTER_CRITICAL(); //进入临界区

    /* --------------------------------- 创建ranging任务 --------------------------------- */

    xTaskCreate((TaskFunction_t)ranging_task,
                (const char *)"ranging_task",
                (uint16_t)RANGING_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)RANGING_TASK_PRIO,
                (TaskHandle_t *)&RANGINGTask_Handler);

    /* --------------------------------- 创建usart3任务 --------------------------------- */

    xTaskCreate((TaskFunction_t)usart3_task,
                (const char *)"usart3_task",
                (uint16_t)USART3_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)USART3_TASK_PRIO,
                (TaskHandle_t *)&USART3Task_Handler);
                
    /* --------------------------------- 创建uart4任务 --------------------------------- */

    xTaskCreate((TaskFunction_t)uart4_task,
                (const char *)"uart4_task",
                (uint16_t)UART4_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)UART4_TASK_PRIO,
                (TaskHandle_t *)&UART4Task_Handler);                

    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

/* ----------------------------------- 变量 ----------------------------------- */
extern uint8_t radar_static_flag;
extern uint8_t scan_flag;

_DW_Static DW_Static = {0};
_USART_INTERRUPT USART_INTERRUPT = {0};

/* -------------------------------------------------------------------------- */
/*                                    测距任务                                    */
/* -------------------------------------------------------------------------- */

void ranging_task(void *pvParameters)
{
    uint8_t radar_ldle_counter = 0;
    dwt_enableframefilter(DWT_FF_DATA_EN | DWT_FF_ACK_EN);
    dwt_setpanid(0xF0F0);
    for (;;)
    {
        radar_ldle_counter++;
        vTaskDelay(1);
    }
}

/* -------------------------------------------------------------------------- */
/*                                   参数配置任务                                   */
/* -------------------------------------------------------------------------- */

void usart3_task(void *pvParameters)
{
    uint8_t led_flag = 0;  
    
    for (;;)
    {
        if ((DW_Static.Present_Static_Flag == START_STATIC) || (DW_Static.Present_Static_Flag == USART_STATIC))
        {
            printf("Channel_Num = %d\r\n", SENTDATA.Radar_Channel);
            if (DW_Static.Present_Static_Flag == USART_STATIC)      //串口改变状态的情况下，需要复位重新加载
            {
                LL_GPIO_SetOutputPin(GPIOI, LL_GPIO_PIN_8);         //reset
            }
            DW_Static.Present_Static_Flag = TEMP_STATIC;
        }
        led_flag++;
        if (led_flag == 3)
        {
            LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9);
            printf("CPU Usage = %d%%\r\n", osGetCPUUsage());            
        }
        else if (led_flag == 6)
        {
            LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9);
            led_flag = 0;
        }                                        
        vTaskDelay(200);
    }
}


/* -------------------------------------------------------------------------- */
/*                                   RFID数据处理任务                                   */
/* -------------------------------------------------------------------------- */

void uart4_task(void *pvParameters)
{   
    static uint16_t for_num = 0;
    for (;;)
    {                         
        for_num++;
        if(for_num == 65535)
        {
            for_num = 302;
        }        
        vTaskDelay(20);
    }
}



void CB_Usart_RX(uint8_t *rx_buff, uint8_t resv_len)
{
    USART_INTERRUPT.Interrupt_Flag = 1;
    USART_INTERRUPT.Data_flag = rx_buff;
    USART_INTERRUPT.Len_Flag = resv_len;
}

void CB_Uart4_RX(uint8_t *rx_buff, uint8_t resv_len)
{
    USART_INTERRUPT.Interrupt_Flag2 = 1;
    USART_INTERRUPT.Uart4_Data_flag = rx_buff;
    USART_INTERRUPT.Uart4_Len_Flag = resv_len;
}


void APP_RFID_DataProcessing(uint16_t *rfid_buff, uint8_t train_up_down, uint8_t train_forward, uint8_t train_backward)
{
    if((rfid_buff[1] == rfid_buff[0] + 1) && ((rfid_buff[2] == rfid_buff[1]) || (rfid_buff[2] == 0)))    //forward
    {
        SENTDATA.Up_Down = train_forward;
        DW_Static.UpOrDown_Flag = train_up_down;
        printf("train forward\r\n");    
    }
    else
    if((rfid_buff[1] == rfid_buff[0] + 1) && (rfid_buff[2] == rfid_buff[0]))                                //backward
    {
        SENTDATA.Up_Down = train_backward;
        DW_Static.UpOrDown_Flag = train_up_down;
        printf("train backward\r\n");
    }
    else
    {
        printf("Unknown logic for read rfid data\r\n");
    }
}
