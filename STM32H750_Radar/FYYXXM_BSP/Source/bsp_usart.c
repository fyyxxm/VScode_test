#include "bsp_usart.h"

////////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while ((USART3->ISR & 0X40) == 0)
    {
    }; //循环发送,直到发送完毕
    USART3->TDR = (uint8_t)ch;
    return ch;
}
#endif

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
uint8_t USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.
uint8_t UART4_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.

void (*BSP_UsartRXCallBack)(uint8_t *rx_buff, uint8_t buff_len);

void (*BSP_Uart4RXCallBack)(uint8_t *rx_buff, uint8_t buff_len);

void BSP_Usart3_GPIO_Init(void)
{

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
    /**USART3 GPIO Configuration  
        PC10   ------> USART1_TX
        PC11   ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_10 | LL_GPIO_PIN_11;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);           
}

void BSP_Uart4_GPIO_Init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
    /**USART3 GPIO Configuration  
        PB8   ------> UART8_TX
        PB9   ------> UART8_RX 
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_8 | LL_GPIO_PIN_9;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);           
}

void BSP_Usart3_Init()
{
    
    /* USER CODE BEGIN USART3_Init 0 */
    
    /* USER CODE END USART3_Init 0 */
    LL_USART_InitTypeDef USART_InitStruct = {0};
    BSP_Usart3_GPIO_Init();
    BSP_Usart_DMA_Init();
    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
    
    /* USART3 interrupt Init */
    NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 7, 0));
    NVIC_EnableIRQ(USART3_IRQn);

    /* USER CODE BEGIN USART3_Init 1 */

    /* USER CODE END USART3_Init 1 */
    USART_InitStruct.BaudRate = 115200;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV2;         //50Mhz
    LL_USART_Init(USART3, &USART_InitStruct);
    LL_USART_ConfigAsyncMode(USART3);
    LL_USART_EnableIT_RXNE(USART3);                                 //不用DMA模式时，使用
    LL_USART_EnableIT_IDLE(USART3);                                 //  用中断空闲模式，不定长接收，DMA
    LL_USART_Enable(USART3);
    /* USER CODE BEGIN USART3_Init 2 */

    /* USER CODE END USART3_Init 2 */
    /* -------------------------------- DMA INIT -------------------------------- */
    /* enable usart dma tx&rx */
    LL_USART_EnableDMAReq_TX(USART3);
}




void BSP_Uart4_Init()
{
    
    /* USER CODE BEGIN USART3_Init 0 */
    
    /* USER CODE END USART3_Init 0 */
    LL_USART_InitTypeDef USART_InitStruct = {0};
    BSP_Uart4_GPIO_Init();
    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART4);
    
    /* USART3 interrupt Init */
    NVIC_SetPriority(UART4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 9, 0));
    NVIC_EnableIRQ(UART4_IRQn);

    /* USER CODE BEGIN USART3_Init 1 */

    /* USER CODE END USART3_Init 1 */
    USART_InitStruct.BaudRate = 115200;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV2;         //50Mhz
    LL_USART_Init(UART4, &USART_InitStruct);
    LL_USART_ConfigAsyncMode(UART4);
    LL_USART_EnableIT_RXNE(UART4);                                 //不用DMA模式时，使用
    LL_USART_EnableIT_IDLE(UART4);                                 //  用中断空闲模式，不定长接收，DMA
    LL_USART_Enable(UART4);
    /* USER CODE BEGIN USART3_Init 2 */

    /* USER CODE END USART3_Init 2 */
}

///* --------------------------------- 串口3不定长接收 -------------------------------- */

void USART3_IRQHandler()
{
    static uint8_t count = 0;
    if (LL_USART_IsActiveFlag_RXNE(USART3) != RESET)
    {
        USART_RX_BUF[count] = LL_USART_ReceiveData8(USART3);   //读取就清楚RXNE标志位了
        count++;
        if (count == 1)
        {
            LL_USART_EnableIT_IDLE(USART3);
        }
    }
    else if (LL_USART_IsActiveFlag_IDLE(USART3) != RESET)
    {
        LL_USART_ClearFlag_IDLE(USART3);
        LL_USART_DisableIT_IDLE(USART3);
        if(count != 0)
        {
//            Usart_SentForDMA(USART_RX_BUF, 1, count);
            BSP_UsartRXCallBack(USART_RX_BUF, count);
            count = 0;
        }
    }
}


///* --------------------------------- 串口4不定长接收 -------------------------------- */

void UART4_IRQHandler()
{
    static uint8_t count = 0;
    if (LL_USART_IsActiveFlag_RXNE(UART4) != RESET)
    {        
        UART4_RX_BUF[count] = LL_USART_ReceiveData8(UART4);   //读取就清楚RXNE标志位了
        count++;
        if (count == 1)
        {
            LL_USART_EnableIT_IDLE(UART4);
        }
    }
    else if (LL_USART_IsActiveFlag_IDLE(UART4) != RESET)
    {
        LL_USART_ClearFlag_IDLE(UART4);
        LL_USART_DisableIT_IDLE(UART4);
        if(count != 0)
        {
//            Usart_SentForDMA(UART4_RX_BUF, 1, count);
            BSP_Uart4RXCallBack(UART4_RX_BUF, count);
            count = 0;
        }
    }
}
///* -------------------------------------------------------------------------- */

void Usart_SentForDMA(const void *buffer, uint8_t byte_size, uint16_t len)
{
    uint16_t counter;
    if (byte_size == 1)
    {
        const uint8_t *p_buff;
        p_buff = buffer;
        for (counter = 0; counter < len; counter++)
        {
            usart_tx_buff[counter] = p_buff[counter];
        }
        LL_DMA_ClearFlag_TC1(DMA1);
        LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_1);
        LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_1, len);
        LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);               
    }
    else if (byte_size == 2)
    {
        const uint16_t *p_buff;
        p_buff = buffer;

        for (counter = 0; counter < len; counter++)
        {
            usart_tx_buff[0 + counter + counter] = (uint32_t)p_buff[counter] >> 8;
            usart_tx_buff[1 + counter + counter] = (uint32_t)p_buff[counter] & 0xFF;
        }
        LL_DMA_ClearFlag_TC1(DMA1);
        LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_1);
        LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_1, len * byte_size);
        LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);  
    }
    else if (byte_size == 4)
    {
        const uint32_t *p_buff;
        p_buff = buffer;

        for (counter = 0; counter < len; counter++)
        {
            usart_tx_buff[0 + counter + counter + counter + counter] = (uint32_t)p_buff[counter] >> 24;
            usart_tx_buff[1 + counter + counter + counter + counter] = (uint32_t)(p_buff[counter] >> 16) & 0xFF;
            usart_tx_buff[2 + counter + counter + counter + counter] = (uint32_t)(p_buff[counter] >> 8) & 0xFF;
            usart_tx_buff[3 + counter + counter + counter + counter] = (uint32_t)p_buff[counter] & 0xFF;
        }
        LL_DMA_ClearFlag_TC1(DMA1);
        LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_1);
        LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_1, len * byte_size);
        LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);  
    }
    else
    {
        printf("byte_size erro\r\n");
    }
}

void BSP_SentForUsart(USART_TypeDef *USARTx, uint8_t *string, uint16_t len)
{
    LL_USART_ClearFlag_IDLE(USARTx);
    LL_USART_DisableIT_IDLE(USARTx);

    for(uint16_t temp = 0; temp < len; temp++)
    {
        LL_USART_TransmitData8(USARTx, *string++);
        while(LL_USART_IsActiveFlag_TC(USARTx) != 1);
    }
}

void BSP_SetUsartRXHOOK(void (*UsartRXCallBack)(uint8_t *rx_buff, uint8_t buff_len))
{
    BSP_UsartRXCallBack = UsartRXCallBack;
}

void BSP_SetUart4RXHOOK(void (*Uart4RXCallBack)(uint8_t *rx_buff, uint8_t buff_len))
{
    BSP_Uart4RXCallBack = Uart4RXCallBack;
}
