#ifndef __BSP_EXIT_INTERRUPT_H
#define __BSP_EXIT_INTERRUPT_H 

#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_exti.h"
#include "stm32h7xx_ll_bus.h"

#include "bsp_usart.h"
#include "bsp_delay.h"



void BSP_Exit_Interrupt_Init(void);
void BSP_DWSetRxHook(void(*rx_hook)(void));

		 				    
#endif
