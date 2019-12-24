#ifndef __BSP_SPI_H
#define __BSP_SPI_H	 

#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_bus.h"

#include "stm32h7xx_ll_spi.h"
#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_ll_rcc.h"

#include "bsp_dma.h"


#define SPI_CS_H    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4)
#define SPI_CS_L    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4)

void BSP_SPI1_Init(void);

void BSP_SPI1_SetSpeed(uint32_t LL_SPI_BAUDRATEPRESCALER_DIV);

uint8_t SPI1_ReadWriteByte(uint8_t TxData);

void SPI1_SentForDMA(const void *buffer, uint8_t byte_size, uint16_t len);

void SPI1_RecvForDMA(uint8_t *buffer, uint16_t len);

		 				    
#endif
