#include "bsp_spi.h"

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */

void BSP_SPI1_Init(void)
{

    LL_SPI_InitTypeDef SPI_InitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    LL_RCC_SetSPIClockSource(LL_RCC_SPI123_CLKSOURCE_PLL1Q);
    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

    LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
    /**SPI1 GPIO Configuration  
  PA4   ------> SPI1_NSS
  PA5   ------> SPI1_SCK
  PA6   ------> SPI1_MISO
  PA7   ------> SPI1_MOSI 
  */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN SPI1_Init 1 */

    /* USER CODE END SPI1_Init 1 */
    /* SPI1 parameter configuration*/
    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
    SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
    SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
    SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV256;
    SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_ENABLE;
    SPI_InitStruct.CRCPoly = 7;
    LL_SPI_Init(SPI1, &SPI_InitStruct);
    LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
    LL_SPI_SetCRCWidth(SPI1, 0x00000000UL);
    LL_SPI_EnableNSSPulseMgt(SPI1);
    /* USER CODE BEGIN SPI1_Init 2 */
    LL_SPI_Enable(SPI1);
    __nop();
    __nop();
    __nop();
    __nop();
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4);


	LL_SPI_EnableIT_TXP(SPI1);
    LL_SPI_EnableIT_RXP(SPI1);
	LL_SPI_EnableIT_TXTF(SPI1);
    LL_SPI_EnableIT_EOT(SPI1);
    /* USER CODE END SPI1_Init 2 */
}

void BSP_SPI1_SetSpeed(uint32_t LL_SPI_BAUDRATEPRESCALER_DIV)
{
    LL_SPI_Disable(SPI1);
    LL_SPI_SetBaudRatePrescaler(SPI1, LL_SPI_BAUDRATEPRESCALER_DIV);
    LL_SPI_Enable(SPI1);
}

//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
    uint8_t resv_buff = 0;
    LL_SPI_Enable(SPI1);
    SPI_CS_L;
    LL_SPI_StartMasterTransfer(SPI1);
    while (LL_SPI_IsActiveFlag_TXP(SPI1) == 0) //检查指定的SPI标志位设置与否:发送缓存空标志位
    {
    }
    LL_SPI_TransmitData8(SPI1, TxData);        //通过外设SPIx发送一个数据
    while (LL_SPI_IsActiveFlag_RXP(SPI1) == 0) //检查指定的SPI标志位设置与否:发送缓存空标志位
    {
    }
    LL_SPI_ReceiveData8(SPI1); //必须要有，因为SPI 发送和接收是同时进行的，发送了，就必须接收，哪怕是空

    LL_SPI_ClearFlag_TXTF(SPI1);
    LL_SPI_ClearFlag_EOT(SPI1);
    LL_SPI_SuspendMasterTransfer(SPI1);
    LL_SPI_Disable(SPI1);
    SPI_CS_H;
    return resv_buff; //返回通过SPIx最近接收的数据
}

