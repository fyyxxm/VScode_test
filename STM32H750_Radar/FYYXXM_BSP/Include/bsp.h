#ifndef __BSP_H
#define __BSP_H	 

#include "stdint.h"
#include "bsp_usart.h"
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup CORTEX_Exported_Constants CORTEX Exported Constants
  * @{
  */

/** @defgroup CORTEX_Preemption_Priority_Group CORTEX Preemption Priority Group
  * @{
  */
#define NVIC_PRIORITYGROUP_0         0x00000007U /*!< 0 bits for pre-emption priority
                                                      4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         0x00000006U /*!< 1 bits for pre-emption priority
                                                      3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         0x00000005U /*!< 2 bits for pre-emption priority
                                                      2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         0x00000004U /*!< 3 bits for pre-emption priority
                                                      1 bits for subpriority */
#define NVIC_PRIORITYGROUP_4         0x00000003U /*!< 4 bits for pre-emption priority
                                                      0 bits for subpriority */
                                                                                                            
enum
{
    START_STATIC = 0,
    TEMP_STATIC,
    USART_STATIC
};

enum
{
    TTR_MODE = 0,
    RTT_MODE
};



typedef struct 
{
    uint8_t Present_Static_Flag;
    uint8_t IsReset_Flag; 
    uint8_t UpOrDown_Flag;
}_DW_Static;

typedef struct 
{
    uint8_t Radar_Channel;
    uint8_t Line_Flag;
    uint8_t Up_Down;
}_SENTDATATEMP;

extern _SENTDATATEMP SENTDATA;

#endif
