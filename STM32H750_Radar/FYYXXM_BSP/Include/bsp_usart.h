#ifndef __BSP_USART_H
#define __BSP_USART_H	 

#include "stm32h7xx_ll_usart.h"
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_bus.h"
#include "bsp_dma.h"
#include "bsp_string.h"
#include "bsp_delay.h"
#include "stdio.h"	

#define USART_REC_LEN  			200  	//定义最大接收字节数 200

void BSP_Usart3_GPIO_Init(void);

void BSP_Uart4_GPIO_Init(void);

void BSP_Usart3_Init(void);

void BSP_Uart4_Init(void);

void BSP_SetUsartRXHOOK(void (*UsartRXCallBack)(uint8_t *rx_buff, uint8_t buff_len));

void BSP_SetUart4RXHOOK(void (*Uart4RXCallBack)(uint8_t *rx_buff, uint8_t buff_len));

void Usart_SentForDMA(const void *buffer, uint8_t byte_size, uint16_t len);

void BSP_SentForUsart(USART_TypeDef *USARTx, uint8_t *string, uint16_t len);
		 				    
#endif
