#include "bsp_iwdog.h"
/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
void BSP_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  LL_IWDG_EnableWriteAccess(IWDG1);
  LL_IWDG_SetPrescaler(IWDG1, LL_IWDG_PRESCALER_64);		//32k/64 = 500hz  t=2ms
  LL_IWDG_SetReloadCounter(IWDG1, 600);                  	//t = 2*600 = 1.2s
  LL_IWDG_ReloadCounter(IWDG1);
  LL_IWDG_Enable(IWDG1);
  while (LL_IWDG_IsReady(IWDG1) != 1)
  {
        printf("BSP_IWDG_Init ERRO\r\n");    
  }

  LL_IWDG_ReloadCounter(IWDG1);
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}
