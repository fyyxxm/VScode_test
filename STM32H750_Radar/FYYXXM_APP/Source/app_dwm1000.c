#include "app_dwm1000.h"

/*************增加功率(开启放大器)****************************/
void APP_DWEnableTXAmplifier(void)
{
    uint32_t reg = 0;
    // Set up MFIO
    reg = dwt_read32bitreg(GPIO_CTRL_ID);
    reg |= 0x00014000; //7 and 8 to mode - to be used with PA
    reg |= 0x00050000; //8 and 9 to mode - RX/TX testing
    dwt_write32bitreg(GPIO_CTRL_ID, reg);

    //disable fine grain sequencing - this is needed when using PA on the TX
    dwt_write16bitoffsetreg(PMSC_ID, PMSC_TXFINESEQ_OFFSET, 0);
}

/*****************DW1000 TX SET START************************/
 dwt_config_t config_temp;

void APP_DWTXRXInit(void)
{
    uint16_t channel_buff[1] = {0};
    SENTDATA.Radar_Channel = 4;
    /* Reset and initialise DW1000.
     * For initialisation, DW1000 clocks must be temporarily set to crystal speed. After initialisation SPI rate can be increased for optimum
     * performance. */
    reset_DW1000(); /* Target specific drive of RSTn line into DW1000 low for a period. */
    BSP_SPI1_SetSpeed(LL_SPI_BAUDRATEPRESCALER_DIV256);
    if (dwt_initialise(DWT_LOADUCODE) == DWT_ERROR)
    {
        while (1)
        {
        };
    }
    BSP_SPI1_SetSpeed(LL_SPI_BAUDRATEPRESCALER_DIV8);    //160M/8 = 20MHZ

    /* Configure DW1000. See NOTE 7 below. */
    BSP_EEPROM_ReadHalfWords(channel_buff, EEPROM_SAVE_ADDR + 4, 1);
    SENTDATA.Radar_Channel = channel_buff[0];
    
    switch (SENTDATA.Radar_Channel)
    {
        case 0x01:
            config_temp = config1;
            break;
        case 0x02:
            config_temp = config2;
            break;
        case 0x03:
            config_temp = config3;
            break;
        case 0x04:
            config_temp = config4;
            break;
        default:
            config_temp = config4;
            break;
    }
    dwt_configure(&config_temp);

    /*******************SET LED RX(BLUE)  TX(RED)**************************/
    /* Configure GPIOs to show TX/RX activity. See NOTE 6 below. */
    dwt_setlnapamode(DWT_LNA_ENABLE | DWT_PA_ENABLE);

    /* Configure LEDs management. See NOTE 6 below. */
    dwt_setleds(DWT_LEDS_ENABLE);

    /* Apply default antenna delay value. See NOTE 1 below. */
    dwt_setrxantennadelay(RX_ANT_DLY);
    dwt_settxantennadelay(TX_ANT_DLY);

    APP_DWEnableTXAmplifier();
}



