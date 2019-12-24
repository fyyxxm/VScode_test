#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_bus.h"
#include "bsp_delay.h"



/*
    i2c����GPIO:
         PB6/I2C1_SCL
         PB7/I2C1_SDA
*/
/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */

#define I2C_SCL_GRP1_PERIPH_GPIOX    LL_AHB4_GRP1_PERIPH_GPIOB 
#define I2C_SDA_GRP1_PERIPH_GPIOX    LL_AHB4_GRP1_PERIPH_GPIOB

#define I2C_SCL_GPIO_PORT           GPIOB               /* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SCL_PIN                 LL_GPIO_PIN_6       /* ���ӵ�SCLʱ���ߵ�GPIO */

#define I2C_SDA_GPIO_PORT           GPIOB               /* ���ӵ�SDA�����ߵ�GPIO */
#define I2C_SDA_PIN                 LL_GPIO_PIN_7       /* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ� */
#define I2C_SCL_H                   LL_GPIO_SetOutputPin(I2C_SCL_GPIO_PORT, I2C_SCL_PIN);       /* SCL = 1 */
#define I2C_SCL_L                   LL_GPIO_ResetOutputPin(I2C_SCL_GPIO_PORT, I2C_SCL_PIN);     /* SCL = 0 */

#define I2C_SDA_H                   LL_GPIO_SetOutputPin(I2C_SDA_GPIO_PORT, I2C_SDA_PIN)        /* SDA = 1 */
#define I2C_SDA_L                   LL_GPIO_ResetOutputPin(I2C_SDA_GPIO_PORT, I2C_SDA_PIN)      /* SDA = 0 */

#define I2C_SDA_READ                LL_GPIO_IsInputPinSet(I2C_SDA_GPIO_PORT, I2C_SDA_PIN)       /* ��SDA����״̬ */
#define I2C_SCL_READ                LL_GPIO_IsInputPinSet(I2C_SCL_GPIO_PORT, I2C_SCL_PIN)       /* ��SCL����״̬ */

#define I2C_WR                      0 /* д����bit */
#define I2C_RD                      1 /* ������bit */

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
