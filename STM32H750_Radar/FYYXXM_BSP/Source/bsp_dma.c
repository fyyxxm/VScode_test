#include "bsp_dma.h"

uint8_t usart_tx_buff[200] = {0};
//uint8_t spi1_tx_buff[100] = {0};
//uint8_t spi1_rx_buff[100] = {0};
//uint8_t usart_rx_buffer[100];

void BSP_Usart_DMA_Init()
{
    LL_DMA_InitTypeDef LL_DMA_InitStructure;
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
    //    LL_DMA_DeInit(DMA1, LL_DMA_CHANNEL_3);

    /* DMA interrupt init */
    /* DMA1_Stream3_IRQn interrupt configuration */
    //    NVIC_SetPriority(DMA1_Stream1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),10, 0));
    //    NVIC_EnableIRQ(DMA1_Stream1_IRQn);
    /* ------------------------------ TX DMA CONFIG ----------------------------- */
    LL_DMA_DisableDoubleBufferMode(DMA1, LL_DMA_STREAM_1);
    
    LL_DMA_InitStructure.PeriphOrM2MSrcAddress = (uint32_t)&(USART3 -> TDR);      //LL_USART_DMA_GetRegAddr(USART3, LL_USART_DMA_REG_DATA_TRANSMIT);
    LL_DMA_InitStructure.MemoryOrM2MDstAddress = (uint32_t)usart_tx_buff;
    LL_DMA_InitStructure.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    LL_DMA_InitStructure.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
    LL_DMA_InitStructure.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
    LL_DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
    LL_DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
    LL_DMA_InitStructure.Mode = LL_DMA_MODE_NORMAL;
    LL_DMA_InitStructure.NbData = 0;
    LL_DMA_InitStructure.Priority = LL_DMA_PRIORITY_MEDIUM;
    LL_DMA_InitStructure.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
    LL_DMA_InitStructure.FIFOThreshold = LL_DMA_FIFOTHRESHOLD_1_4;
    LL_DMA_InitStructure.PeriphRequest = LL_DMAMUX1_REQ_USART3_TX;
    LL_DMA_InitStructure.MemBurst = LL_DMA_MBURST_SINGLE;
    LL_DMA_InitStructure.PeriphBurst = LL_DMA_PBURST_SINGLE;
    LL_DMA_Init(DMA1, LL_DMA_STREAM_1, &LL_DMA_InitStructure);

    /** ------------------------------ RX DMA CONFIG ----------------------------- */
    /* disable rx dma
   LL_DMA_InitStructure.PeriphOrM2MSrcAddress = (uint32_t)(&USART3 -> RDR);
   LL_DMA_InitStructure.MemoryOrM2MDstAddress = (uint32_t)usart_rx_buffer;
   LL_DMA_InitStructure.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
   LL_DMA_InitStructure.PeriphOrM2MSrcIncMode = LL_DMA_MEMORY_NOINCREMENT;
   LL_DMA_InitStructure.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
   LL_DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
   LL_DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
   LL_DMA_InitStructure.Mode = LL_DMA_MODE_NORMAL;
   LL_DMA_InitStructure.NbData = 200;
   LL_DMA_InitStructure.Priority = LL_DMA_PRIORITY_HIGH;
   LL_DMA_InitStructure.PeriphRequest = LL_DMAMUX1_REQ_USART3_RX;
   LL_DMA_InitStructure.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
   LL_DMA_Init(DMA1, LL_DMA_STREAM_2, &LL_DMA_InitStructure);

   LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);           //先 使能，因为不知道什么开始接收
   */
    //    LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_1);

    LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_1); //使用时，在使能
}

/*不用中断是因为，发送会很频繁，会频繁进中断，实际也没必要用中断···*/

//void DMA1_Stream1_IRQHandler()
//{
//    if(LL_DMA_IsActiveFlag_TC1(DMA1) == 1)
//    {
//        LL_DMA_ClearFlag_TC1(DMA1);
//    }
//}



#if 0
void BSP_SPI1_DMA_Init()
{
    LL_DMA_InitTypeDef LL_DMA_InitStructure;
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
    //    LL_DMA_DeInit(DMA1, LL_DMA_CHANNEL_3);

    /* DMA interrupt init */
    /* DMA1_Stream3_IRQn interrupt configuration */
    //    NVIC_SetPriority(DMA1_Stream1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));
    //    NVIC_EnableIRQ(DMA1_Stream1_IRQn);
    /* ------------------------------ TX DMA CONFIG ----------------------------- */
    LL_DMA_DisableDoubleBufferMode(DMA1, LL_DMA_STREAM_2);
    LL_DMA_DisableDoubleBufferMode(DMA1, LL_DMA_STREAM_3);

    LL_DMA_InitStructure.PeriphOrM2MSrcAddress = (uint32_t)&(SPI1 -> TXDR);
    LL_DMA_InitStructure.MemoryOrM2MDstAddress = (uint32_t)spi1_tx_buff;
    LL_DMA_InitStructure.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    LL_DMA_InitStructure.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
    LL_DMA_InitStructure.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
    LL_DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
    LL_DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
    LL_DMA_InitStructure.Mode = LL_DMA_MODE_NORMAL;
    LL_DMA_InitStructure.NbData = 0;
    LL_DMA_InitStructure.Priority = LL_DMA_PRIORITY_MEDIUM;
    LL_DMA_InitStructure.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
    LL_DMA_InitStructure.FIFOThreshold = LL_DMA_FIFOTHRESHOLD_1_4;
    LL_DMA_InitStructure.PeriphRequest = LL_DMAMUX1_REQ_SPI1_TX;
    LL_DMA_InitStructure.MemBurst = LL_DMA_MBURST_SINGLE;
    LL_DMA_InitStructure.PeriphBurst = LL_DMA_PBURST_SINGLE;
    LL_DMA_Init(DMA1, LL_DMA_STREAM_2, &LL_DMA_InitStructure);

    /** ------------------------------ RX DMA CONFIG ----------------------------- */
    LL_DMA_InitStructure.PeriphOrM2MSrcAddress = (uint32_t)&(SPI1 -> RXDR);
    LL_DMA_InitStructure.MemoryOrM2MDstAddress = (uint32_t)spi1_rx_buff;
    LL_DMA_InitStructure.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
    LL_DMA_InitStructure.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
    LL_DMA_InitStructure.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
    LL_DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
    LL_DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
    LL_DMA_InitStructure.Mode = LL_DMA_MODE_NORMAL;
    LL_DMA_InitStructure.NbData = 100;
    LL_DMA_InitStructure.Priority = LL_DMA_PRIORITY_MEDIUM;
    LL_DMA_InitStructure.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
    LL_DMA_InitStructure.FIFOThreshold = LL_DMA_FIFOTHRESHOLD_1_4;
    LL_DMA_InitStructure.PeriphRequest = LL_DMAMUX1_REQ_SPI1_RX;
    LL_DMA_InitStructure.MemBurst = LL_DMA_MBURST_SINGLE;
    LL_DMA_InitStructure.PeriphBurst = LL_DMA_PBURST_SINGLE;
    LL_DMA_Init(DMA1, LL_DMA_STREAM_3, &LL_DMA_InitStructure); 

    //    LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_1);

    LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_2); //使用时，在使能
    LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_3); //使用时，在使能
    
}

#endif
