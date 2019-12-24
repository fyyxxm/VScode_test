#ifndef __BSP_DMA_H
#define __BSP_DMA_H

#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_ll_usart.h"
#include "stm32h7xx_ll_bus.h"
#include "stm32h7xx_ll_spi.h"

extern uint8_t usart_tx_buff[200];
//extern uint8_t spi1_tx_buff[100];
//extern uint8_t spi1_rx_buff[100];
//extern uint8_t usart_rx_buffer[100];

void BSP_Usart_DMA_Init(void);
void BSP_SPI1_DMA_Init(void);

#endif
