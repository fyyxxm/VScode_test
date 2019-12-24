#ifndef __APP_CRC_H
#define __APP_CRC_H	 

unsigned short CRC16_CCITT(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_XModem(void *ptr, int len);
unsigned short CRC16_X25(unsigned char* pData, unsigned int nLength);

#endif
