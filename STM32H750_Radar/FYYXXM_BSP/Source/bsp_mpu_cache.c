#include "bsp_mpu_cache.h"

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/*
*********************************************************************************************************
*    函 数 名: MPU_Config
*    功能说明: 配置MPU
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
void BSP_MPU_Config(void)
{
    /* 禁止 MPU */
    LL_MPU_Disable();
#if 0
    /* Configure the MPU attributes as Device not cacheable 
     for ETH DMA descriptors */
    LL_MPU_ConfigRegion(LL_MPU_REGION_NUMBER0, 0x00, 0x30040000, 
                        LL_MPU_REGION_SIZE_256B|LL_MPU_REGION_FULL_ACCESS|LL_MPU_TEX_LEVEL0|LL_MPU_INSTRUCTION_ACCESS_ENABLE|
                        LL_MPU_ACCESS_NOT_SHAREABLE|LL_MPU_ACCESS_NOT_CACHEABLE|LL_MPU_ACCESS_BUFFERABLE);
    LL_MPU_EnableRegion(LL_MPU_REGION_NUMBER0);
    /* Configure the MPU attributes as Cacheable write through 
     for LwIP RAM heap which contains the Tx buffers */
    LL_MPU_ConfigRegion(LL_MPU_REGION_NUMBER1, 0x00, 0x30044000, 
                        LL_MPU_REGION_SIZE_16KB|LL_MPU_REGION_FULL_ACCESS|LL_MPU_TEX_LEVEL0|LL_MPU_INSTRUCTION_ACCESS_ENABLE|
                        LL_MPU_ACCESS_NOT_SHAREABLE|LL_MPU_ACCESS_CACHEABLE|LL_MPU_ACCESS_NOT_BUFFERABLE);
    LL_MPU_EnableRegion(LL_MPU_REGION_NUMBER1);
#endif
    
    /* 配置 AXI SRAM 的 MPU 属性为 Write back, Read allocate Write allocate */
    LL_MPU_ConfigRegion(LL_MPU_REGION_NUMBER3, 0x00, 0x24000000, 
                        LL_MPU_REGION_SIZE_512KB|LL_MPU_REGION_FULL_ACCESS|LL_MPU_TEX_LEVEL0|LL_MPU_INSTRUCTION_ACCESS_ENABLE|
                        LL_MPU_ACCESS_NOT_SHAREABLE|LL_MPU_ACCESS_CACHEABLE|LL_MPU_ACCESS_NOT_BUFFERABLE);
    LL_MPU_EnableRegion(LL_MPU_REGION_NUMBER3);
    
    /* 配置 FMC 扩展 IO 的 MPU 属性为 Device 或者 Strongly Ordered */
    LL_MPU_ConfigRegion(LL_MPU_REGION_NUMBER4, 0x00, 0x60000000, 
                        LL_MPU_REGION_SIZE_64KB|LL_MPU_REGION_FULL_ACCESS|LL_MPU_TEX_LEVEL0|LL_MPU_INSTRUCTION_ACCESS_ENABLE|
                        LL_MPU_ACCESS_NOT_SHAREABLE|LL_MPU_ACCESS_NOT_CACHEABLE|LL_MPU_ACCESS_NOT_BUFFERABLE);
    LL_MPU_EnableRegion(LL_MPU_REGION_NUMBER4);
    
    /* 配置 SDRAM 的 MPU 属性为 Write back, Read allocate Write allocate */
    LL_MPU_ConfigRegion(LL_MPU_REGION_NUMBER5, 0x00, 0xD0000000, 
                        LL_MPU_REGION_SIZE_256MB|LL_MPU_REGION_FULL_ACCESS|LL_MPU_TEX_LEVEL0|LL_MPU_INSTRUCTION_ACCESS_ENABLE|
                        LL_MPU_ACCESS_NOT_SHAREABLE|LL_MPU_ACCESS_CACHEABLE|LL_MPU_ACCESS_NOT_BUFFERABLE);
    LL_MPU_EnableRegion(LL_MPU_REGION_NUMBER5);
    /*使能 MPU */
    LL_MPU_Enable(LL_MPU_CTRL_PRIVILEGED_DEFAULT);
}
