#ifndef __APP_DWM1000_H
#define __APP_DWM1000_H	 

#include "bsp.h"
#include "bsp_i2c_eeprom_24xx.h"
#include "deca_device_api.h"
#include "deca_regs.h"
#include "FreeRTOS.h"
#include "task.h"


#define EEPROM_SAVE_ADDR 2020                      //设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000) 1FC00 = 127K


/********************************************************************************************/
/* Inter-ranging delay period, in milliseconds. */
#define RNG_DELAY_MS 20

/***************************6.8M专用**********************************/
///* Default communication configuration. We use here EVK1000's mode 4. See NOTE 1 below. */
//static dwt_config_t config = {
//    4,               /* Channel number. */
//    DWT_PRF_64M,     /* Pulse repetition frequency. */
//    DWT_PLEN_128,    /* Preamble length. Used in TX only. */
//    DWT_PAC8,        /* Preamble acquisition chunk size. Used in RX only. */
//    18,               /* TX preamble code. Used in TX only. */
//    18,               /* RX preamble code. Used in RX only. */
//    0,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
//    DWT_BR_6M8,      /* Data rate. */
//    DWT_PHRMODE_STD, /* PHY header mode. */
//    (129 + 8 - 8)    /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
//};
///* Delay between frames, in UWB microseconds. See NOTE 1 below. */
//#define POLL_TX_TO_RESP_RX_DLY_UUS 140
///* Receive response timeout. See NOTE 5 below. */
//#define RESP_RX_TIMEOUT_UUS 300
///* Delay between frames, in UWB microseconds. See NOTE 1 below. */
//#define POLL_RX_TO_RESP_TX_DLY_UUS 400        //6.8M专用
///******************************************************/

///***************************110K专用**********************************/
///* Default communication configuration. We use here EVK1000's default mode (mode 3). */
//static dwt_config_t config = {
//    4,               /* Channel number. */
//    DWT_PRF_64M,     /* Pulse repetition frequency. */
//    DWT_PLEN_1024,   /* Preamble length. Used in TX only. */
//    DWT_PAC32,       /* Preamble acquisition chunk size. Used in RX only. */
//    18,               /* TX preamble code. Used in TX only. */
//    18,               /* RX preamble code. Used in RX only. */
//    0,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
//    DWT_BR_110K,     /* Data rate. */
//    DWT_PHRMODE_STD, /* PHY header mode. */
//    (1025 + 64 - 32) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
//};

///* Delay between frames, in UWB microseconds. See NOTE 1 below. */
//#define POLL_TX_TO_RESP_RX_DLY_UUS 150
///* Receive response timeout. See NOTE 5 below. */
//#define RESP_RX_TIMEOUT_UUS 3500

//#define POLL_RX_TO_RESP_TX_DLY_UUS 2800       //110K专用,数据需要+2.5米
/****************************************************************************/


/***************************850K专用**********************************/
/* Default communication configuration. We use here EVK1000's default mode (mode 3). */
static dwt_config_t config4 = {
    4,               /* Channel number. */
    DWT_PRF_64M,     /* Pulse repetition frequency. */
    DWT_PLEN_256,   /* Preamble length. Used in TX only. */
    DWT_PAC16,       /* Preamble acquisition chunk size. Used in RX only. */
    18,               /* TX preamble code. Used in TX only. */
    18,               /* RX preamble code. Used in RX only. */
    0,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_850K,     /* Data rate. */
    DWT_PHRMODE_STD, /* PHY header mode. */
    (257 + 16 - 16) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};

static dwt_config_t config1 = {
    1,               /* Channel number. */
    DWT_PRF_64M,     /* Pulse repetition frequency. */
    DWT_PLEN_256,   /* Preamble length. Used in TX only. */
    DWT_PAC16,       /* Preamble acquisition chunk size. Used in RX only. */
    9,               /* TX preamble code. Used in TX only. */
    9,               /* RX preamble code. Used in RX only. */
    0,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_850K,     /* Data rate. */
    DWT_PHRMODE_STD, /* PHY header mode. */
    (257 + 16 - 16) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};

static dwt_config_t config2 = {    //2实际是3
    3,               /* Channel number. */
    DWT_PRF_64M,     /* Pulse repetition frequency. */
    DWT_PLEN_256,   /* Preamble length. Used in TX only. */
    DWT_PAC16,       /* Preamble acquisition chunk size. Used in RX only. */
    9,               /* TX preamble code. Used in TX only. */
    9,               /* RX preamble code. Used in RX only. */
    0,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_850K,     /* Data rate. */
    DWT_PHRMODE_STD, /* PHY header mode. */
    (257 + 16 - 16) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};

static dwt_config_t config3 = {    //2实际是3
    7,               /* Channel number. */
    DWT_PRF_64M,     /* Pulse repetition frequency. */
    DWT_PLEN_256,   /* Preamble length. Used in TX only. */
    DWT_PAC16,       /* Preamble acquisition chunk size. Used in RX only. */
    18,               /* TX preamble code. Used in TX only. */
    18,               /* RX preamble code. Used in RX only. */
    0,               /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_850K,     /* Data rate. */
    DWT_PHRMODE_STD, /* PHY header mode. */
    (257 + 16 - 16) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};




/* Delay between frames, in UWB microseconds. See NOTE 1 below. */
#define POLL_TX_TO_RESP_RX_DLY_UUS 100
/* Receive response timeout. See NOTE 5 below. */
#define RESP_RX_TIMEOUT_UUS 1500

#define POLL_RX_TO_RESP_TX_DLY_UUS 1000       
/*********************************************************************/

/* Default antenna delay values for 64 MHz PRF. See NOTE 2 below. */
#define TX_ANT_DLY 16436
#define RX_ANT_DLY 16436


/* Frames used in the ranging process. See NOTE 3 below. */
static uint8_t tx_poll_msg[] = {0x41, 0x88, 0, 0xF0, 0xF0, 0x03, 0x00, 0x00, 0x00, 0xE0, 0, 0};                   //12 13 14  15是上次的距离信息，发送给RTT端
static uint8_t rx_resp_msg[] = {0x41, 0x88, 0, 0xF0, 0xF0, 0x00, 0x00, 0x03, 0x00, 0xE1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
/* Frames used in the ranging process. See NOTE 3 below. */
static uint8_t rx_poll_msg[] = {0x41, 0x88, 0, 0xF0, 0xF0, 0x03, 0x00, 0x00, 0x00, 0xE0, 0, 0};                   
static uint8_t tx_resp_msg[] = {0x41, 0x88, 0, 0xF0, 0xF0, 0x00, 0x00, 0x03, 0x00, 0xE1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static  uint8_t temp[10] = {0};
    
/* Length of the common part of the message (up to and including the function code, see NOTE 3 below). */
#define ALL_MSG_COMMON_LEN 10
/* Indexes to access some of the fields in the frames defined above. */
#define ALL_MSG_SN_IDX 2
#define RESP_MSG_POLL_RX_TS_IDX 10
#define RESP_MSG_RESP_TX_TS_IDX 14
#define RESP_MSG_TS_LEN 4
///* Frame sequence number, incremented after each transmission. */
//static uint8_t frame_seq_nb = 0;

/* Buffer to store received response message.
 * Its size is adjusted to longest frame that this example code is supposed to handle. */
#define RX_BUF_LEN 26
//static uint8_t rx_buffer[RX_BUF_LEN];

///* Hold copy of status register state here for reference so that it can be examined at a debug breakpoint. */
//static uint32_t status_reg = 0;

/* UWB microsecond (uus) to device time unit (dtu, around 15.65 ps) conversion factor.
 * 1 uus = 512 / 499.2 祍 and 1 祍 = 499.2 * 128 dtu. */
#define UUS_TO_DWT_TIME 65536



/* Speed of light in air, in metres per second. */
#define SPEED_OF_LIGHT 299702547

/* Hold copies of computed time of flight and distance here for reference so that it can be examined at a debug breakpoint. */
static float tof;
static int32_t distance;

/* Declaration of static functions. */
static void resp_msg_get_ts(uint8_t *ts_field, uint32_t *ts);

/* Declaration of static functions. */
static uint64_t get_rx_timestamp_u64(void);
static void resp_msg_set_ts(uint8_t *ts_field, const uint64_t ts);

/******************************************************/
enum RX_FLAG
{
   RX_OK = 0,
   RX_FALSE    
};


enum TX_FLAG
{
   TX_OK = 0,
   TX_FALSE    
};

//typedef union
//{
//    uint16_t rader_mode;
//    struct
//    {
//        uint8_t channel_mode;
//        uint8_t work_mode;  
//    };       
//}_RADER_MODE;

//extern _RADER_MODE RADER_MODE;

/*****************DW1000 TX SET************************/
void APP_DWTXRXInit(void);

void APP_DWEnableTXAmplifier(void);
		 				    
#endif
