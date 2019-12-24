#ifndef __BSP_I2C_EEPROM_24XX_H
#define __BSP_I2C_EEPROM_24XX_H
#include "bsp_i2c.h"   
#include "bsp_delay.h"


/* H7-TOOL安装的是24C16 (2K字节） */

//#define AT24C02
//#define AT24C04
#define AT24C16
//#define AT24C128

#ifdef AT24C02
#define EE_MODEL_NAME "AT24C02"
#define EE_DEV_ADDR 0xA0            /* 设备地址 */
#define EE_PAGE_SIZE 8              /* 页面大小(字节) */
#define EE_SIZE 256                 /* 总容量(字节) */
#define EE_ADDR_BYTES 1             /* 地址字节个数 */
#endif

#ifdef AT24C04
#define EE_MODEL_NAME "AT24C04"
#define EE_DEV_ADDR 0xA0            /* 设备地址 */
#define EE_PAGE_SIZE 16             /* 页面大小(字节) */
#define EE_SIZE 512                 /* 总容量(字节) */
#define EE_ADDR_BYTES 1             /* 地址字节个数 */
#endif

#ifdef AT24C16
#define EE_MODEL_NAME "AT24C16"
#define EE_DEV_ADDR 0xA0            /* 设备地址 */
#define EE_PAGE_SIZE 16             /* 页面大小(字节) */
#define EE_SIZE 2048                /* 总容量(字节) */
#define EE_ADDR_BYTES 1             /* 地址字节个数 */
#endif

#ifdef AT24C128
#define EE_MODEL_NAME "AT24C128"
#define EE_DEV_ADDR 0xA0            /* 设备地址 */
#define EE_PAGE_SIZE 64             /* 页面大小(字节) */
#define EE_SIZE (16 * 1024)         /* 总容量(字节) */
#define EE_ADDR_BYTES 2             /* 地址字节个数 */
#endif

uint8_t BSP_EEPROM_Check(void);
uint8_t BSP_EEPROM_ReadBytes(uint8_t *ReadBuff, uint16_t Address, uint16_t nBytes);
uint8_t BSP_EEPROM_WriteBytes(uint8_t *WriteBuff, uint16_t Address, uint16_t nByte);

uint8_t BSP_EEPROM_WriteHalfWords(uint16_t *WriteBuff, uint16_t Address, uint16_t nHalfWords);
uint8_t BSP_EEPROM_WriteWords(uint32_t *WriteBuff, uint16_t Address, uint16_t Words);
uint8_t BSP_EEPROM_ReadHalfWords(uint16_t *ReadBuff, uint16_t Address, uint16_t nHalfWords);
uint8_t BSP_EEPROM_ReadWords(uint32_t *ReadBuff, uint16_t Address, uint16_t nWords);

#endif



