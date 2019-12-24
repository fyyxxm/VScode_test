#include "bsp_i2c_eeprom_24xx.h" 


/*
*********************************************************************************************************
*    �� �� ��: ee_CheckOk
*    ����˵��: �жϴ���EERPOM�Ƿ�����
*    ��    ��:  ��
*    �� �� ֵ: 1 ��ʾ������ 0 ��ʾ������
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
        /* ʧ�ܺ��мǷ���I2C����ֹͣ�ź� */
        BSP_I2C_Stop();
        return 0;
    }
}

/*
*********************************************************************************************************
*    �� �� ��: ee_ReadBytes
*    ����˵��: �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
*    ��    ��:  _usAddress : ��ʼ��ַ
*             _usSize : ���ݳ��ȣ���λΪ�ֽ�
*             _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
*    �� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t BSP_EEPROM_ReadBytes(uint8_t *ReadBuff, uint16_t Address, uint16_t nBytes)
{
    uint16_t i;

    /* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */

    /* ��1��������I2C���������ź� */
    BSP_I2C_Start();

/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
#if EE_ADDR_BYTES == 1
    BSP_I2C_SendByte(EE_DEV_ADDR | I2C_WR | (((Address >> 8) & 0x07) << 1)); /* �˴���дָ�� */
#else
    BSP_I2C_SendByte(EE_DEV_ADDR | I2C_WR);
#endif

    /* ��3��������ACK */
    if (BSP_I2C_WaitAck() != 0)
    {
        goto cmd_fail; /* EEPROM������Ӧ�� */
    }

    /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
    if (EE_ADDR_BYTES == 1)
    {
        BSP_I2C_SendByte((uint8_t)Address);
        if (BSP_I2C_WaitAck() != 0)
        {
            goto cmd_fail; /* EEPROM������Ӧ�� */
        }
    }
    else
    {
        BSP_I2C_SendByte(Address >> 8);
        if (BSP_I2C_WaitAck() != 0)
        {
            goto cmd_fail; /* EEPROM������Ӧ�� */
        }

        BSP_I2C_SendByte(Address);
        if (BSP_I2C_WaitAck() != 0)
        {
            goto cmd_fail; /* EEPROM������Ӧ�� */
        }
    }

    /* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
    BSP_I2C_Start();

    /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    BSP_I2C_SendByte(EE_DEV_ADDR | I2C_RD); /* �˴��Ƕ�ָ�� */

    /* ��8��������ACK */
    if (BSP_I2C_WaitAck() != 0)
    {
        goto cmd_fail; /* EEPROM������Ӧ�� */
    }

    /* ��9����ѭ����ȡ���� */
    for (i = 0; i < nBytes; i++)
    {
        ReadBuff[i] = BSP_I2C_ReadByte(); /* ��1���ֽ� */

        /* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
        if (i != nBytes - 1)
        {
            BSP_I2C_Ack(); /* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
        }
        else
        {
            BSP_I2C_NAck(); /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
        }
    }
    /* ����I2C����ֹͣ�ź� */
    BSP_I2C_Stop();
    return 1; /* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    BSP_I2C_Stop();
    return 0;
}

/*
*********************************************************************************************************
*    �� �� ��: ee_WriteBytes
*    ����˵��: ����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
*    ��    ��:  _usAddress : ��ʼ��ַ
*             _usSize : ���ݳ��ȣ���λΪ�ֽ�
*             _pWriteBuf : ��Ŷ��������ݵĻ�����ָ��
*    �� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t BSP_EEPROM_WriteBytes(uint8_t *WriteBuff, uint16_t Address, uint16_t nBytes)
{
    uint16_t byte_num, counter;
    uint16_t usAddr;

    /*
        д����EEPROM�������������������ȡ�ܶ��ֽڣ�ÿ��д����ֻ����ͬһ��page��
        ����24xx02��page size = 8
        �򵥵Ĵ�����Ϊ�����ֽ�д����ģʽ��ÿд1���ֽڣ������͵�ַ
        Ϊ���������д��Ч��: ����������page wirte������
    */

    usAddr = Address;
    for (byte_num = 0; byte_num < nBytes; byte_num++)
    {
        /* �����͵�1���ֽڻ���ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ */
        if ((byte_num == 0) || (usAddr & (EE_PAGE_SIZE - 1)) == 0)
        {
            /*����ֹͣ�źţ������ڲ�д������*/
            BSP_I2C_Stop();

            /* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms
                CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
            */
            for (counter = 0; counter < 1000; counter++)
            {
                /* ��1��������I2C���������ź� */
                BSP_I2C_Start();

                /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */

#if EE_ADDR_BYTES == 1
                BSP_I2C_SendByte(EE_DEV_ADDR | I2C_WR | (((usAddr >> 8) & 0x07) << 1)); /* �˴���дָ�� */
#else
                BSP_I2C_SendByte(EE_DEV_ADDR | I2C_WR);
#endif

                /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
                if (BSP_I2C_WaitAck() == 0)
                {
                    break;
                }
            }
            if (counter == 1000)
            {
                goto cmd_fail; /* EEPROM����д��ʱ */
            }

            /* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
            if (EE_ADDR_BYTES == 1)
            {
                BSP_I2C_SendByte((uint8_t)usAddr);
                if (BSP_I2C_WaitAck() != 0)
                {
                    goto cmd_fail; /* EEPROM������Ӧ�� */
                }
            }
            else
            {
                BSP_I2C_SendByte(usAddr >> 8);
                if (BSP_I2C_WaitAck() != 0)
                {
                    goto cmd_fail; /* EEPROM������Ӧ�� */
                }

                BSP_I2C_SendByte(usAddr);
                if (BSP_I2C_WaitAck() != 0)
                {
                    goto cmd_fail; /* EEPROM������Ӧ�� */
                }
            }
        }

        /* ��6������ʼд������ */
        BSP_I2C_SendByte(WriteBuff[byte_num]);

        /* ��7��������ACK */
        if (BSP_I2C_WaitAck() != 0)
        {
            goto cmd_fail; /* EEPROM������Ӧ�� */
        }

        usAddr++; /* ��ַ��1 */
    }

    /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
    BSP_I2C_Stop();

    /* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms
        CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
    */
    for (counter = 0; counter < 1000; counter++)
    {
        /* ��1��������I2C���������ź� */
        BSP_I2C_Start();

/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
#if EE_ADDR_BYTES == 1
        BSP_I2C_SendByte(EE_DEV_ADDR | I2C_WR | (((Address >> 8) & 0x07) << 1)); /* �˴���дָ�� */
#else
        BSP_I2C_SendByte(EE_DEV_ADDR | I2C_WR); /* �˴���дָ�� */
#endif

        /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
        if (BSP_I2C_WaitAck() == 0)
        {
            break;
        }
    }
    if (counter == 1000)
    {
        goto cmd_fail; /* EEPROM����д��ʱ */
    }

    /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
    BSP_I2C_Stop();

    return 1;

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
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




