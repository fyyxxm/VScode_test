/**
  ******************************************************************************
  * @file    gpio.c
  * @author  fyyxxm
  * @version V1.0
  * @date    2019.06.28
  * @brief   GPIO_Init.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "bsp_gpio.h"

void BSP_GPIO_Init()
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOI);

  /**/
//  LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9|LL_GPIO_PIN_10);

  /**/
  GPIO_InitStruct.Pin =  LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);	
  
  GPIO_InitStruct.Pin =  LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOI, &GPIO_InitStruct);	

  LL_GPIO_ResetOutputPin(GPIOI, LL_GPIO_PIN_8);
}
