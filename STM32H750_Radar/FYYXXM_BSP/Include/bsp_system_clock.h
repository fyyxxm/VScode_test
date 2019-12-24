#ifndef __BSP_SYSTEM_CLOCK_H
#define __BSP_SYSTEM_CLOCK_H	 

#include "stm32h7xx_ll_system.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_utils.h"
#include "stm32h7xx_ll_cortex.h"
#include "stm32h7xx_ll_pwr.h"

void BSP_SystemClock_Config(void);

void Error_Handler(void);	
		 				    
#endif
