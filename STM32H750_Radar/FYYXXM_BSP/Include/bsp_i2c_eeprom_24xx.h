#ifndef __BSP_I2C_EEPROM_24XX_H
#define __BSP_I2C_EEPROM_24XX_H
#include "bsp_i2c.h"   
#include "bsp_delay.h"


/* H7-TOOL��װ����24C16 (2K�ֽڣ� */

//#define AT24C02
//#define AT24C04
#define AT24C16
//#define AT24C128

#ifdef AT24C02
#define EE_MODEL_NAME "AT24C02"
#define EE_DEV_ADDR 0xA0            /* �豸��ַ */
#define EE_PAGE_SIZE 8              /* ҳ���С(�ֽ�) */
#define EE_SIZE 256                 /* ������(�ֽ�) */
#define EE_ADDR_BYTES 1             /* ��ַ�ֽڸ��� */
#endif

#ifdef AT24C04
#define EE_MODEL_NAME "AT24C04"
#define EE_DEV_ADDR 0xA0            /* �豸��ַ */
#define EE_PAGE_SIZE 16             /* ҳ���С(�ֽ�) */
#define EE_SIZE 512                 /* ������(�ֽ�) */
#define EE_ADDR_BYTES 1             /* ��ַ�ֽڸ��� */
#endif

#ifdef AT24C16
#define EE_MODEL_NAME "AT24C16"
#define EE_DEV_ADDR 0xA0            /* �豸��ַ */
#define EE_PAGE_SIZE 16             /* ҳ���С(�ֽ�) */
#define EE_SIZE 2048                /* ������(�ֽ�) */
#define EE_ADDR_BYTES 1             /* ��ַ�ֽڸ��� */
#endif

#ifdef AT24C128
#define EE_MODEL_NAME "AT24C128"
#define EE_DEV_ADDR 0xA0            /* �豸��ַ */
#define EE_PAGE_SIZE 64             /* ҳ���С(�ֽ�) */
#define EE_SIZE (16 * 1024)         /* ������(�ֽ�) */
#define EE_ADDR_BYTES 2             /* ��ַ�ֽڸ��� */
#endif

uint8_t BSP_EEPROM_Check(void);
uint8_t BSP_EEPROM_ReadBytes(uint8_t *ReadBuff, uint16_t Address, uint16_t nBytes);
uint8_t BSP_EEPROM_WriteBytes(uint8_t *WriteBuff, uint16_t Address, uint16_t nByte);

uint8_t BSP_EEPROM_WriteHalfWords(uint16_t *WriteBuff, uint16_t Address, uint16_t nHalfWords);
uint8_t BSP_EEPROM_WriteWords(uint32_t *WriteBuff, uint16_t Address, uint16_t Words);
uint8_t BSP_EEPROM_ReadHalfWords(uint16_t *ReadBuff, uint16_t Address, uint16_t nHalfWords);
uint8_t BSP_EEPROM_ReadWords(uint32_t *ReadBuff, uint16_t Address, uint16_t nWords);

#endif



