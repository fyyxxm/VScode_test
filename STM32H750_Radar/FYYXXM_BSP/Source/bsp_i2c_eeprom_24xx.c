#include "bsp_i2c_eeprom_24xx.h" 


/*
*********************************************************************************************************
*    函 数 名: ee_CheckOk
*    功能说明: 判断串行EERPOM是否正常
*    形    参:  无
*    返 回 值: 1 表示正常， 0 表示不正常
*********************************************************************************************************
*/
uint8_t BSP_EEPROM_Check(void)
{
    if (BSP_I2C_CheckDevice(EE_DEV_ADDR) == 0)
    {
        return 1;
    }
    else
    {
        /* 失败后，切记发送I2C总线停止信号 */
        BSP_I2C_Stop();
        return 0;
    }
}

/*
*********************************************************************************************************
*    函 数 名: ee_ReadBytes
*    功能说明: 从串行EEPROM指定地址处开始读取若干数据
*    形    参:  _usAddress : 起始地址
*             _usSize : 数据长度，单位为字节
*             _pReadBuf : 存放读到的数据的缓冲区指针
*    返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t BSP_EEPROM_ReadBytes(uint8_t *ReadBuff, uint16_t Address, uint16_t nBytes)
{
    uint16_t i;

    /* 采用串行EEPROM随即读取指令序列，连续读取若干字节 */

    /* 第1步：发起I2C总线启动信号 */
    BSP_I2C_Start();

/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
#if EE_ADDR_BYTES == 1
    BSP_I2C_SendByte(EE_DEV_ADDR | I2C_WR | (((Address >> 8) & 0x07) << 1)); /* 此处是写指令 */
#else
    BSP_I2C_SendByte(EE_DEV_ADDR | I2C_WR);
#endif

    /* 第3步：发送ACK */
    if (BSP_I2C_WaitAck() != 0)
    {
        goto cmd_fail; /* EEPROM器件无应答 */
    }

    /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
    if (EE_ADDR_BYTES == 1)
    {
        BSP_I2C_SendByte((uint8_t)Address);
        if (BSP_I2C_WaitAck() != 0)
        {
            goto cmd_fail; /* EEPROM器件无应答 */
        }
    }
    else
    {
        BSP_I2C_SendByte(Address >> 8);
        if (BSP_I2C_WaitAck() != 0)
        {
            goto cmd_fail; /* EEPROM器件无应答 */
        }

        BSP_I2C_SendByte(Address);
        if (BSP_I2C_WaitAck() != 0)
        {
            goto cmd_fail; /* EEPROM器件无应答 */
        }
    }

    /* 第6步：重新启动I2C总线。下面开始读取数据 */
    BSP_I2C_Start();

    /* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
    BSP_I2C_SendByte(EE_DEV_ADDR | I2C_RD); /* 此处是读指令 */

    /* 第8步：发送ACK */
    if (BSP_I2C_WaitAck() != 0)
    {
        goto cmd_fail; /* EEPROM器件无应答 */
    }

    /* 第9步：循环读取数据 */
    for (i = 0; i < nBytes; i++)
    {
        ReadBuff[i] = BSP_I2C_ReadByte(); /* 读1个字节 */

        /* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
        if (i != nBytes - 1)
        {
            BSP_I2C_Ack(); /* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
        }
        else
        {
            BSP_I2C_NAck(); /* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
        }
    }
    /* 发送I2C总线停止信号 */
    BSP_I2C_Stop();
    return 1; /* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    BSP_I2C_Stop();
    return 0;
}

/*
*********************************************************************************************************
*    函 数 名: ee_WriteBytes
*    功能说明: 向串行EEPROM指定地址写入若干数据，采用页写操作提高写入效率
*    形    参:  _usAddress : 起始地址
*             _usSize : 数据长度，单位为字节
*             _pWriteBuf : 存放读到的数据的缓冲区指针
*    返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t BSP_EEPROM_WriteBytes(uint8_t *WriteBuff, uint16_t Address, uint16_t nBytes)
{
    uint16_t byte_num, counter;
    uint16_t usAddr;

    /*
        写串行EEPROM不像读操作可以连续读取很多字节，每次写操作只能在同一个page。
        对于24xx02，page size = 8
        简单的处理方法为：按字节写操作模式，每写1个字节，都发送地址
        为了提高连续写的效率: 本函数采用page wirte操作。
    */

    usAddr = Address;
    for (byte_num = 0; byte_num < nBytes; byte_num++)
    {
        /* 当发送第1个字节或是页面首地址时，需要重新发起启动信号和地址 */
        if ((byte_num == 0) || (usAddr & (EE_PAGE_SIZE - 1)) == 0)
        {
            /*　发停止信号，启动内部写操作　*/
            BSP_I2C_Stop();

            /* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms
                CLK频率为200KHz时，查询次数为30次左右
            */
            for (counter = 0; counter < 1000; counter++)
            {
                /* 第1步：发起I2C总线启动信号 */
                BSP_I2C_Start();

                /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */

#if EE_ADDR_BYTES == 1
                BSP_I2C_SendByte(EE_DEV_ADDR | I2C_WR | (((usAddr >> 8) & 0x07) << 1)); /* 此处是写指令 */
#else
                BSP_I2C_SendByte(EE_DEV_ADDR | I2C_WR);
#endif

                /* 第3步：发送一个时钟，判断器件是否正确应答 */
                if (BSP_I2C_WaitAck() == 0)
                {
                    break;
                }
            }
            if (counter == 1000)
            {
                goto cmd_fail; /* EEPROM器件写超时 */
            }

            /* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
            if (EE_ADDR_BYTES == 1)
            {
                BSP_I2C_SendByte((uint8_t)usAddr);
                if (BSP_I2C_WaitAck() != 0)
                {
                    goto cmd_fail; /* EEPROM器件无应答 */
                }
            }
            else
            {
                BSP_I2C_SendByte(usAddr >> 8);
                if (BSP_I2C_WaitAck() != 0)
                {
                    goto cmd_fail; /* EEPROM器件无应答 */
                }

                BSP_I2C_SendByte(usAddr);
                if (BSP_I2C_WaitAck() != 0)
                {
                    goto cmd_fail; /* EEPROM器件无应答 */
                }
            }
        }

        /* 第6步：开始写入数据 */
        BSP_I2C_SendByte(WriteBuff[byte_num]);

        /* 第7步：发送ACK */
        if (BSP_I2C_WaitAck() != 0)
        {
            goto cmd_fail; /* EEPROM器件无应答 */
        }

        usAddr++; /* 地址增1 */
    }

    /* 命令执行成功，发送I2C总线停止信号 */
    BSP_I2C_Stop();

    /* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms
        CLK频率为200KHz时，查询次数为30次左右
    */
    for (counter = 0; counter < 1000; counter++)
    {
        /* 第1步：发起I2C总线启动信号 */
        BSP_I2C_Start();

/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
#if EE_ADDR_BYTES == 1
        BSP_I2C_SendByte(EE_DEV_ADDR | I2C_WR | (((Address >> 8) & 0x07) << 1)); /* 此处是写指令 */
#else
        BSP_I2C_SendByte(EE_DEV_ADDR | I2C_WR); /* 此处是写指令 */
#endif

        /* 第3步：发送一个时钟，判断器件是否正确应答 */
        if (BSP_I2C_WaitAck() == 0)
        {
            break;
        }
    }
    if (counter == 1000)
    {
        goto cmd_fail; /* EEPROM器件写超时 */
    }

    /* 命令执行成功，发送I2C总线停止信号 */
    BSP_I2C_Stop();

    return 1;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
    /* 发送I2C总线停止信号 */
    BSP_I2C_Stop();
    return 0;
}



uint8_t BSP_EEPROM_WriteHalfWords(uint16_t *WriteBuff, uint16_t Address, uint16_t nHalfWords)
{
    uint16_t bytes_num = nHalfWords + nHalfWords;
    uint8_t byte_buff[bytes_num];
    for(uint8_t temp = 0; temp < nHalfWords; temp++)
    {
        byte_buff[temp + temp] = WriteBuff[temp] >> 8;
        byte_buff[temp + temp + 1] = WriteBuff[temp] & 0xFF;
    }
    return BSP_EEPROM_WriteBytes(byte_buff, Address, bytes_num);
}


uint8_t BSP_EEPROM_WriteWords(uint32_t *WriteBuff, uint16_t Address, uint16_t Words)
{
    uint16_t bytes_num = Words + Words +Words + Words;
    uint8_t byte_buff[bytes_num];
    for(uint8_t temp = 0; temp < Words; temp++)
    {
        byte_buff[temp + temp + temp + temp] = WriteBuff[temp] >> 24;
        byte_buff[temp + temp + temp + temp + 1] = (WriteBuff[temp] >> 16) & 0xFF;
        byte_buff[temp + temp + temp + temp + 2] = (WriteBuff[temp] >> 8) & 0xFF;
        byte_buff[temp + temp + temp + temp + 3] = WriteBuff[temp] & 0xFF;        
    }
    return BSP_EEPROM_WriteBytes(byte_buff, Address, bytes_num);
}


uint8_t BSP_EEPROM_ReadHalfWords(uint16_t *ReadBuff, uint16_t Address, uint16_t nHalfWords)
{
    uint16_t bytes_num = nHalfWords + nHalfWords;
    uint8_t byte_buff[bytes_num];
    uint8_t back;
    
    back = BSP_EEPROM_ReadBytes(byte_buff, Address, bytes_num);
    if(back == 0)
    {
        return 0;
    }
    else
    {
        for(uint8_t temp = 0; temp < nHalfWords; temp++)
        {
            ReadBuff[temp] = (byte_buff[temp + temp] << 8) + byte_buff[temp + temp + 1];
        } 
        return 1;
    }    
}


uint8_t BSP_EEPROM_ReadWords(uint32_t *ReadBuff, uint16_t Address, uint16_t nWords)
{
    uint16_t bytes_num = nWords + nWords + nWords + nWords;
    uint8_t byte_buff[bytes_num];
    uint8_t back;
    
    back = BSP_EEPROM_ReadBytes(byte_buff, Address, bytes_num);
    if(back == 0)
    {
        return 0;
    }
    else
    {
        for(uint8_t temp = 0; temp < nWords; temp++)
        {
            ReadBuff[temp] = (byte_buff[temp + temp + temp + temp] << 24) + (byte_buff[temp + temp + temp + temp + 1] << 16) + (byte_buff[temp + temp + temp + temp + 2] << 8) + byte_buff[temp + temp + temp + temp + 3];
        } 
        return 1;
    }    
}




