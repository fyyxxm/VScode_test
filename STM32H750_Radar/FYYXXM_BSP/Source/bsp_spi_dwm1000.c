#include "bsp_spi_dwm1000.h"

uint8_t SPI1_WriteToDW(uint16_t headerLength, const uint8_t *headerBuffer, uint32_t bodyLength, const uint8_t *bodyBuffer)
{
    uint8_t i = 0;
    
    decaIrqStatus_t stat;

    stat = decamutexon();  
    LL_SPI_Enable(SPI1);    
    SPI_CS_L;    
    LL_SPI_StartMasterTransfer(SPI1);
    for (i = 0; i < headerLength; i++)
    {
        while (LL_SPI_IsActiveFlag_TXP(SPI1) == 0) //检查指定的SPI标志位设置与否:发送缓存空标志位
        {            
        }
        LL_SPI_TransmitData8(SPI1, headerBuffer[i]); //通过外设SPIx发送一个数据
        while (LL_SPI_IsActiveFlag_RXP(SPI1) == 0) //检查指定的SPI标志位设置与否:发送缓存空标志位
        { 
        }
        LL_SPI_ReceiveData8(SPI1);
    }	
    for (i = 0; i < bodyLength; i++)
    {
        while (LL_SPI_IsActiveFlag_TXP(SPI1) == 0) //检查指定的SPI标志位设置与否:发送缓存空标志位
        {      
        }
        LL_SPI_TransmitData8(SPI1, bodyBuffer[i]); //通过外设SPIx发送一个数据 
        while (LL_SPI_IsActiveFlag_RXP(SPI1) == 0) //检查指定的SPI标志位设置与否:发送缓存空标志位
        { 
        }
        LL_SPI_ReceiveData8(SPI1);     
    }
	LL_SPI_ClearFlag_TXTF(SPI1);
	LL_SPI_ClearFlag_EOT(SPI1);
    LL_SPI_SuspendMasterTransfer(SPI1);
    LL_SPI_Disable(SPI1);
    SPI_CS_H;
    decamutexoff(stat);
    return WRITE_OK;
} 


uint8_t SPI1_ReadFromDM(uint16_t headerLength, const uint8_t *headerBuffer, uint32_t readlength, uint8_t *readBuffer)
{
    uint8_t i = 0;
    
    decaIrqStatus_t stat;

    stat = decamutexon();  
    LL_SPI_Enable(SPI1);
    SPI_CS_L;
    LL_SPI_StartMasterTransfer(SPI1);
    for (i = 0; i < headerLength; i++)               //send order 0x00 to dw1000   如果编译器开启优化，这个for循环会被优化掉
    {
        while(LL_SPI_IsActiveFlag_TXP(SPI1) == 0) //检查指定的SPI标志位设置与否:发送缓存空标志位
        { 
        }
        LL_SPI_TransmitData8(SPI1, headerBuffer[i]); //通过外设SPIx发送一个数据  
        while (LL_SPI_IsActiveFlag_RXP(SPI1) == 0) //检查指定的SPI标志位设置与否:发送缓存空标志位
        { 

        }
        LL_SPI_ReceiveData8(SPI1);                   //必须要有，因为SPI 发送和接收是同时进行的，发送了，就必须接收，哪怕是空
    }  
    for (i = 0; i < readlength; i++)
    { 
        while(LL_SPI_IsActiveFlag_TXP(SPI1) == 0) //检查指定的SPI标志位设置与否:发送缓存空标志位
        { 
        }  
        LL_SPI_TransmitData8(SPI1,0);               //必须要有，因为SPI 发送和接收是同时进行的，想要接收就必须发送，哪怕是空包。
        while (LL_SPI_IsActiveFlag_RXP(SPI1) == 0) //检查指定的SPI标志位设置与否:发送缓存空标志位
        { 
        }    
        readBuffer[i] = LL_SPI_ReceiveData8(SPI1);  //port_SPIx_receive_data(); //this clears RXNE bit                   
    }    
	LL_SPI_ClearFlag_TXTF(SPI1);
	LL_SPI_ClearFlag_EOT(SPI1);
    LL_SPI_SuspendMasterTransfer(SPI1);
    LL_SPI_Disable(SPI1);
    SPI_CS_H;
    decamutexoff(stat);    
    return WRITE_OK;
} 

/*
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;				 	
	while (LL_SPI_IsActiveFlag_TXE(SPI2) == 0) //检查指定的SPI标志位设置与否:发送缓存空标志位
    {
		retry++;
		if(retry>200)return 0;
    }	       
    LL_SPI_TransmitData8(SPI2, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (LL_SPI_IsActiveFlag_RXNE(SPI2) == 0) //检查指定的SPI标志位设置与否:接受缓存非空标志位
    {
		retry++;
		if(retry>200)return 0;
    }	 
    
	return LL_SPI_ReceiveData8(SPI2); //返回通过SPIx最近接收的数据					    
}

*/

void reset_DW1000(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* GPIO Ports Clock Enable */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOI);
  
	// Enable GPIO used for DW1000 reset
  GPIO_InitStruct.Pin =  LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOI, &GPIO_InitStruct);	

  BSP_Delay_ms(2);
	//drive the RSTn pin low
  LL_GPIO_ResetOutputPin(GPIOI, LL_GPIO_PIN_5);

	//put the pin back to tri-state ... as input
  GPIO_InitStruct.Pin =  LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  LL_GPIO_Init(GPIOI, &GPIO_InitStruct);
  BSP_Delay_ms(2);
}

