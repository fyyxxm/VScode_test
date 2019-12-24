#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H	 

#include "stm32h7xx_ll_system.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_utils.h"
#include "stm32h7xx_ll_cortex.h"

void BSP_Delay_Init(void);

void BSP_Delay_us(uint32_t Delay);	
void BSP_Delay_ms(uint32_t Delay);
void BSP_Delay_nop(uint16_t times);	

#endif
