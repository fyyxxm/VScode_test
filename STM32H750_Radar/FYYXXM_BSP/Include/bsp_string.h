#ifndef __BSP_STRING_H
#define __BSP_STRING_H	 

void my_strcpy(char *dest, const char *src);

void my_strncpy(char *dest, const char *src, int maxlen);

void my_memset(void *src, int value, int len);

#endif
