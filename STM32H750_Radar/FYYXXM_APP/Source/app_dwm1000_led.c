#include "app_dwm1000_led.h"


void APP_DWLEDInit(void)
{
    /* see NOTE 1: 1st enable GPIO clocks */
    dwt_enablegpioclocks();
    dwt_setgpiodirection(DWT_GxM2 | DWT_GxM3, 0);       
}



void APP_DWSetLED(void)
{
    dwt_setgpiovalue(DWT_GxM2 | DWT_GxM3, DWT_GxP2 | DWT_GxP3); /* set GPIO2 high (LED4 will light up)*/
}



void APP_DWResetLED(void)
{
    dwt_setgpiovalue(DWT_GxM2 | DWT_GxM3, 0); /* set GPIO2 low (LED4 will be off)*/
}

