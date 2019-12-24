/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "bsp_system_clock.h"
#include "bsp_delay.h"
#include "bsp_spi.h"
#include "bsp_spi_dwm1000.h"
#include "bsp_gpio.h"
#include "bsp_usart.h"
#include "bsp_spi.h"
#include "app_dwm1000.h"

#include "bsp_i2c.h"
#include "bsp_i2c_eeprom_24xx.h"
#include "bsp_mpu_cache.h"

#include "bsp_rng.h"

#include "bsp_time.h"
#include "cb_dw_rx.h"
#include "cpu_utils.h"


#define HARDWARE_VERSION               "V0.9.0"
#define SOFTWARE_VERSION               "V1.0.0"


typedef union
{
    uint16_t rader_id;
    struct
    {
        uint8_t self_id;
        uint8_t line_id;  
    };       
}_RADER_ID;

typedef struct 
{
    uint8_t Interrupt_Flag;
    uint8_t *Data_flag;
    uint8_t Len_Flag;
    uint8_t Interrupt_Flag2;
    uint8_t *Uart4_Data_flag;
    uint8_t Uart4_Len_Flag;
}_USART_INTERRUPT;


#define TRAIN_HOUSE     0x00

enum
{
    TRAIN_UP = 0x00,
    TRAIN_DOWN,
};

enum
{
    TRAIN_UP_FORWARD = 0x10,
    TRAIN_UP_BACKWARD,
};

enum
{
    TRAIN_DOWN_FORWARD = 0x20,
    TRAIN_DOWN_BACKWARD,
};


static uint8_t usart_return_buffer[14] = {0xA5, 0xA5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void CB_Usart_RX(uint8_t *rx_buff, uint8_t resv_len);

void CB_Uart4_RX(uint8_t *rx_buff, uint8_t resv_len);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
