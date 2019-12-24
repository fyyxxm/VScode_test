#include "cb_dw_rx.h"

static uint8_t temp_count[60] = {0};
static uint8_t temp_count_error[60] = {0};
static uint32_t resv_buff[60][7] = {0};

_SENTDATATEMP SENTDATA;
uint32_t CB_DW_RX(int32_t resv, uint8_t radar_num, uint8_t *speed_info)
{
//    uint8_t sent_data[70] = {0};
    if (resv >= 0)
    {       
        if (temp_count[radar_num] < 6)
        {
            resv_buff[radar_num][temp_count[radar_num]] = resv;
            temp_count[radar_num]++;
        }
        else
        {
            if (dist_file(resv_buff[radar_num], resv) == 1)
            {
                temp_count_error[radar_num] = 0;
                for (uint8_t count = 0; count < 5; count++)
                {
                    resv_buff[radar_num][count] = resv_buff[radar_num][count + 1];
                }
                resv_buff[radar_num][5] = resv;                
                printf("radar_num = %d, distance = %d\r\n", radar_num, resv);           
//                sent_data[0] = 0xA5;       
//                sent_data[1] = 0xA5; 
//                sent_data[3] = 0x3C;    
//                sent_data[4] = 0x32;       
//                sent_data[5] = 0x03; 
//                sent_data[8] = 0x01;
//                
//                /* subway line id */
//                sent_data[20] = 0x00;       
//                sent_data[21] = SENTDATA.Line_Flag; 
//                /* object radar id */
//                sent_data[22] = 0x00;                
//                sent_data[23] = radar_num;  

//                sent_data[24] = SENTDATA.Up_Down;        //上下行 0x00 未知 0x01上行 0x02下行
//                /* radar channel */
//                sent_data[25] = SENTDATA.Radar_Channel;                   
//                
//                /* distance info */
//                sent_data[28] = (resv >> 24); 
//                sent_data[29] = ((resv >> 16) & 0xFF); 
//                sent_data[30] = ((resv >> 8) & 0xFF); 
//                sent_data[31] = (resv & 0xFF);        
//                
//                /* speed info */
//                sent_data[60] = speed_info[3];     
//                sent_data[61] = speed_info[2]; 
//                sent_data[62] = speed_info[1];
//                sent_data[63] = speed_info[0];
//                /* crc */
//                sent_data[64] = CRC16_XModem(sent_data, 64) >> 8;
//                sent_data[65] = CRC16_XModem(sent_data, 64) & 0xFF;   
//                Usart_SentForDMA(sent_data, 1, 66);                
            }
            else
            {
//                printf("distance error\r\n");
                temp_count_error[radar_num]++; //连续出现6次 错误值，就认为其是正常值
                if (temp_count_error[radar_num] == 6)
                {
                    temp_count[radar_num] = 0; //重新装载正常值
                }
            }
        }
    }
    return resv_buff[radar_num][5];
}
