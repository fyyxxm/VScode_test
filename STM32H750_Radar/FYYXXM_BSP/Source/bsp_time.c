#include "bsp_time.h"

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
void BSP_Time7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */
  BSP_IWDG_Init();
  /* USER CODE END TIM7_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7);

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  TIM_InitStruct.Prescaler = 9999;									// 200MHZ/10000 = 20000hz  t = 0.00005s
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;			
  TIM_InitStruct.Autoreload = 19999;                               	// t = 0.00005 * 20000 = 1s
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;			// TIM7 分频无效
  LL_TIM_Init(TIM7, &TIM_InitStruct);
  LL_TIM_EnableARRPreload(TIM7);
//  LL_TIM_SetTriggerOutput(TIM7, LL_TIM_TRGO_UPDATE);
//  LL_TIM_DisableMasterSlaveMode(TIM7);

  /* TIM7 interrupt Init */
  NVIC_SetPriority(TIM7_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),6, 0));
  NVIC_EnableIRQ(TIM7_IRQn);  
  /* USER CODE BEGIN TIM7_Init 2 */
  LL_TIM_EnableCounter(TIM7);         //需要使能计数器
  LL_TIM_EnableIT_UPDATE(TIM7); 
  /* USER CODE END TIM7_Init 2 */
}

void TIM7_IRQHandler(void)     
{
    if(LL_TIM_IsActiveFlag_UPDATE(TIM7) != 0)
    {
        LL_IWDG_ReloadCounter(IWDG1);      //喂狗
    }
    LL_TIM_ClearFlag_UPDATE(TIM7);
}
