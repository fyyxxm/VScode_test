#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_bus.h"
#include "bsp_delay.h"



/*
    i2c总线GPIO:
         PB6/I2C1_SCL
         PB7/I2C1_SDA
*/
/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */

#define I2C_SCL_GRP1_PERIPH_GPIOX    LL_AHB4_GRP1_PERIPH_GPIOB 
#define I2C_SDA_GRP1_PERIPH_GPIOX    LL_AHB4_GRP1_PERIPH_GPIOB

#define I2C_SCL_GPIO_PORT           GPIOB               /* 连接到SCL时钟线的GPIO */
#define I2C_SCL_PIN                 LL_GPIO_PIN_6       /* 连接到SCL时钟线的GPIO */

#define I2C_SDA_GPIO_PORT           GPIOB               /* 连接到SDA数据线的GPIO */
#define I2C_SDA_PIN                 LL_GPIO_PIN_7       /* 连接到SDA数据线的GPIO */


/* 定义读写SCL和SDA的宏 */
#define I2C_SCL_H                   LL_GPIO_SetOutputPin(I2C_SCL_GPIO_PORT, I2C_SCL_PIN);       /* SCL = 1 */
#define I2C_SCL_L                   LL_GPIO_ResetOutputPin(I2C_SCL_GPIO_PORT, I2C_SCL_PIN);     /* SCL = 0 */

#define I2C_SDA_H                   LL_GPIO_SetOutputPin(I2C_SDA_GPIO_PORT, I2C_SDA_PIN)        /* SDA = 1 */
#define I2C_SDA_L                   LL_GPIO_ResetOutputPin(I2C_SDA_GPIO_PORT, I2C_SDA_PIN)      /* SDA = 0 */

#define I2C_SDA_READ                LL_GPIO_IsInputPinSet(I2C_SDA_GPIO_PORT, I2C_SDA_PIN)       /* 读SDA口线状态 */
#define I2C_SCL_READ                LL_GPIO_IsInputPinSet(I2C_SCL_GPIO_PORT, I2C_SCL_PIN)       /* 读SCL口线状态 */

#define I2C_WR                      0 /* 写控制bit */
#define I2C_RD                      1 /* 读控制bit */

void    BSP_I2C_Init(void);
void    BSP_I2C_Start(void);
void    BSP_I2C_Stop(void);
void    BSP_I2C_SendByte(uint8_t OneByte);
void    BSP_I2C_Ack(void);
void    BSP_I2C_NAck(void);
uint8_t BSP_I2C_ReadByte(void);
uint8_t BSP_I2C_WaitAck(void);
uint8_t BSP_I2C_CheckDevice(uint8_t _Address);

#endif
