#ifndef __BSP_SPI_DWM1000_H
#define __BSP_SPI_DWM1000_H	 

#include "bsp_spi.h"
#include "deca_device_api.h"

enum 
{
   WRITE_OK = 0,
   WRITE_FALSE    
};

uint8_t SPI1_WriteToDW(uint16_t headerLength, const uint8_t *headerBuffer, uint32_t bodyLength, const uint8_t *bodyBuffer);
uint8_t SPI1_ReadFromDM(uint16_t headerLength, const uint8_t *headerBuffer, uint32_t readlength, uint8_t *readBuffer);

void reset_DW1000(void);

#endif
