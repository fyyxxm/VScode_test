/**
  ******************************************************************************
  * @file    delay.c
  * @author  fyyxxm
  * @version V1.0
  * @date    2018.06.28
  * @brief   set delay
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
		
#if 1
#include "bsp_delay.h"
void BSP_Delay_Init(void)
{
    /* Use frequency provided in argument */
  LL_InitTick(400000000, 1000000U);

}


void BSP_Delay_us(uint32_t Delay)
{
  uint32_t count = Delay;
  __IO uint32_t  tmp = SysTick->CTRL;  /* Clear the COUNTFLAG first */
  /* Add this code to indicate that local variable is not used */
  ((void)tmp);

  /* Add a period to guaranty minimum wait */
  if(count < LL_MAX_DELAY)
  {
    count++;
  }

  while (count != 0U)
  {
    if((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
      count--;
    }
  }  
}	

void BSP_Delay_ms(uint32_t Delay)                //实测 5000以内都可以，5000以上没测。。。。。
{

  BSP_Delay_us(Delay*1000);

}

void BSP_Delay_nop(uint16_t times)
{
    for(uint16_t temp = 0; temp < times; temp++)
    {
        __nop();
        __nop();
        __nop();
        __nop();   
        __nop(); 
    }        
}

#endif
