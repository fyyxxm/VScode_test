#include "bsp_rng.h"

void BSP_RNG_Init()
{

    /* USER CODE BEGIN RNG_Init 0 */
//    LL_RCC_HSI48_Enable();
    /* USER CODE END RNG_Init 0 */
    LL_RCC_SetRNGClockSource(LL_RCC_RNG_CLKSOURCE_PLL1Q);
    /* Peripheral clock enable */
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_RNG);
    /* USER CODE BEGIN RNG_Init 1 */

    /* USER CODE END RNG_Init 1 */

    LL_RNG_Enable(RNG);
    /* USER CODE BEGIN RNG_Init 2 */

    /* USER CODE END RNG_Init 2 */
}
