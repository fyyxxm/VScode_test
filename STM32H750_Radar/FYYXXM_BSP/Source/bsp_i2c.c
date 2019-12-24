#include "bsp_i2c.h"


/*
*********************************************************************************************************
*    函 数 名: bsp_InitI2C
*    功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*    形    参:  无
*    返 回 值: 无
*********************************************************************************************************
*/
void BSP_I2C_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB4_GRP1_EnableClock(I2C_SCL_GRP1_PERIPH_GPIOX);
  /* GPIO Ports Clock Enable */
  LL_AHB4_GRP1_EnableClock(I2C_SDA_GRP1_PERIPH_GPIOX);
  
  //SCL
  GPIO_InitStruct.Pin =  I2C_SCL_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  LL_GPIO_Init(I2C_SCL_GPIO_PORT, &GPIO_InitStruct);	
  //SDA
  GPIO_InitStruct.Pin =  I2C_SDA_PIN;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  LL_GPIO_Init(I2C_SDA_GPIO_PORT, &GPIO_InitStruct);

  /* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
    BSP_I2C_Stop();
}

/*
*********************************************************************************************************
*    函 数 名: I2C_Delay
*    功能说明: I2C总线位延迟，最快400KHz
*    形    参:  无
*    返 回 值: 无
*********************************************************************************************************
*/
static void I2C_Delay(void)
{
    /*　
        CPU主频168MHz时，在内部Flash运行, MDK工程不优化。用台式示波器观测波形。
        循环次数为5时，SCL频率 = 1.78MHz (读耗时: 92ms, 读写正常，但是用示波器探头碰上就读写失败。时序接近临界)
        循环次数为10时，SCL频率 = 1.1MHz (读耗时: 138ms, 读速度: 118724B/s)
        循环次数为30时，SCL频率 = 440KHz， SCL高电平时间1.0us，SCL低电平时间1.2us

        上拉电阻选择2.2K欧时，SCL上升沿时间约0.5us，如果选4.7K欧，则上升沿约1us

        实际应用选择400KHz左右的速率即可
    */
    /* STM8S GPIO芯片，2us 4us 5us不成功, 6us成功，选7us留点余量 */
    BSP_Delay_nop(10);
}

/*
*********************************************************************************************************
*    函 数 名: i2c_Start
*    功能说明: CPU发起I2C总线启动信号
*    形    参:  无
*    返 回 值: 无
*********************************************************************************************************
*/
void BSP_I2C_Start(void)
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
    I2C_SDA_H;
    I2C_SCL_H;
    I2C_Delay();
    I2C_SDA_L;
    I2C_Delay();
    I2C_SCL_L;
    I2C_Delay();
}

/*
*********************************************************************************************************
*    函 数 名: i2c_Start
*    功能说明: CPU发起I2C总线停止信号
*    形    参:  无
*    返 回 值: 无
*********************************************************************************************************
*/
void BSP_I2C_Stop(void)
{
    /* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
    I2C_SDA_L;
    I2C_SCL_H;
    I2C_Delay();
    I2C_SDA_H;
    I2C_Delay();
}

/*
*********************************************************************************************************
*    函 数 名: i2c_SendByte
*    功能说明: CPU向I2C总线设备发送8bit数据
*    形    参:  _ucByte ： 等待发送的字节
*    返 回 值: 无
*********************************************************************************************************
*/
void BSP_I2C_SendByte(uint8_t OneByte)
{
    uint8_t i;
    /* 先发送字节的高位bit7 */
    for (i = 0; i < 8; i++)
    {
        if (OneByte & 0x80)
        {
            I2C_SDA_H;
        }
        else
        {
            I2C_SDA_L;
        }
        I2C_Delay();
        I2C_SCL_H;
        I2C_Delay();
        I2C_SCL_L;
        if (i == 7)
        {
            I2C_SDA_H; // 释放总线
        }
        OneByte <<= 1; /* 左移一个bit */
                                     //        I2C_Delay();
    }
}

/*
*********************************************************************************************************
*    函 数 名: i2c_ReadByte
*    功能说明: CPU从I2C总线设备读取8bit数据
*    形    参:  无
*    返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t BSP_I2C_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        I2C_SCL_H;
        I2C_Delay();
        if (I2C_SDA_READ)
        {
            value++;
        }
        I2C_SCL_L;
        I2C_Delay();
    }
    return value;
}

/*
*********************************************************************************************************
*    函 数 名: i2c_WaitAck
*    功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*    形    参:  无
*    返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t BSP_I2C_WaitAck(void)
{
    uint8_t re;

    I2C_SDA_H; /* CPU释放SDA总线 */
    I2C_Delay();
    I2C_SCL_H; /* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    I2C_Delay();
    if (I2C_SDA_READ) /* CPU读取SDA口线状态 */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    I2C_SCL_L;
    I2C_Delay();
    return re;
}

/*
*********************************************************************************************************
*    函 数 名: i2c_Ack
*    功能说明: CPU产生一个ACK信号
*    形    参:  无
*    返 回 值: 无
*********************************************************************************************************
*/
void BSP_I2C_Ack(void)
{
    I2C_SDA_L; /* CPU驱动SDA = 0 */
    I2C_Delay();
    I2C_SCL_H; /* CPU产生1个时钟 */
    I2C_Delay();
    I2C_SCL_L;
    I2C_Delay();
    I2C_SDA_H; /* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*    函 数 名: i2c_NAck
*    功能说明: CPU产生1个NACK信号
*    形    参:  无
*    返 回 值: 无
*********************************************************************************************************
*/
void BSP_I2C_NAck(void)
{
    I2C_SDA_H; /* CPU驱动SDA = 1 */
    I2C_Delay();
    I2C_SCL_H; /* CPU产生1个时钟 */
    I2C_Delay();
    I2C_SCL_L;
    I2C_Delay();
}

/*
*********************************************************************************************************
*    函 数 名: i2c_CheckDevice
*    功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*    形    参:  _Address：设备的I2C总线地址
*    返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t BSP_I2C_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;

    if (I2C_SDA_READ && I2C_SCL_READ)
    {
        BSP_I2C_Start(); /* 发送启动信号 */
        
        /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
        BSP_I2C_SendByte(_Address | I2C_WR);
        ucAck = BSP_I2C_WaitAck(); /* 检测设备的ACK应答 */
        BSP_I2C_Stop(); /* 发送停止信号 */
        return ucAck;
    }
    return 1; /* I2C总线异常 */
}




