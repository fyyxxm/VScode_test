#include "app_filter.h"

/*
*
*fyyxxm：简化下面复杂滤波，只使用加权平均滤波
*/

/*测试结果                                                                    //最大最小              //仿最大最小
uint32_t value[FILTER_NUM] = {8,13,15,17,20,23};								//15 - 35				15 - 35

uint32_t value1[FILTER_NUM] = {88,80,75,68,57,55};								// 70  - 20     		20 - 80 

uint32_t value2[FILTER_NUM] = {128,118,107,96,85,75};							//100 - 30				25 - 110 

uint32_t value3[FILTER_NUM] = {12808,11800,10750,9680,8570,7505};				//10000 - 3000			11000 - 3000 

uint32_t value4[FILTER_NUM] = {3808,4800,5750,6668,7507,8550};  				//6000 - 13000			6000 - 13000 

uint32_t value5[FILTER_NUM] = {808,2800,4750,6680,8507,10550};					//7000 - 17000			6000 - 17000 

uint32_t value6[FILTER_NUM] = {3808,4800,6750,9668,14507,19550}; 			    //10000 - 32000			10000 - 32000 

uint32_t value7[FILTER_NUM] = {380800,480000,675000,966800,1450700,1955000};  	//1200000 - 3200000		1000000 - 3200000 
	
uint32_t value8[FILTER_NUM] = {1280800,1080000,875000,668000,357000,150500};	//450000 - 0			600000 -  0	

uint32_t value9[FILTER_NUM] = {1604,1613,1510,1538,1538,1557};					//800 - 2200 			1200 -2300	

uint32_t value10[FILTER_NUM] = {1555,1610,1660,1610,1555,1500};					//700 - 2000 			700 -1900	

uint32_t value11[FILTER_NUM] = {1555,1500,1455,1530,1580,1650};					//900 - 2500 			1200 -2500

uint32_t value12[FILTER_NUM] = {1555,1550,1560,1555,1556,1550};					//800 - 2200 			800 -1900
*/

#define FILTER_NUM (6)
#define SUM_COE (1 + 2 + 4 + 7 + 11 + 16)
//等差权重，具体是什么，需要自己更改
uint8_t coe[FILTER_NUM] = {1, 2, 4, 7, 11, 16};

//uint8_t sum_coe = 1 + 2 + 4 + 7 + 11 + 16; //等差权重，具体是什么，需要自己更改

/*********************************权重滤波核心**********************************/
/*
*越接近现时刻的数据，权取得越大，给予新采样值的权系数越大，则灵敏度越高，但信号平滑度越低
*
*buff: 之前 所获得的连续FILTER_NUM个的正常值数组
*/
uint32_t weight_filter(uint32_t *buff)
{
    uint8_t count;
    uint32_t value_buf[FILTER_NUM];
    uint32_t sum = 0; //因为最大测量距离也就1500m， 1500*1000*16 < uint32_t

    for (count = 0; count < FILTER_NUM; count++)
    {
        value_buf[count] = buff[count];
    }

    for (count = 0; count < FILTER_NUM; count++)
    {
        sum += value_buf[count] * coe[count];
    }

    return (uint32_t)(sum / SUM_COE);
}

uint8_t dist_file(uint32_t *buff, uint32_t newvalue)
{
    uint32_t new_buff[FILTER_NUM] = {0};
    uint32_t pre_buff[FILTER_NUM] = {0};

    uint32_t weight_value = 0;
    uint32_t next_weight_value = 0;
    uint32_t pre_weight_value = 0;

    uint32_t temp_weight_value = 0;

    //计算出之前的加权平均值
    weight_value = weight_filter(buff);

    //计算出上一个的加权平均值
    for (uint8_t count = 1; count < FILTER_NUM; count++)
    {
        pre_buff[count] = buff[count - 1];
    }
    if (buff[0] >= buff[1])
        pre_buff[0] = buff[0] + buff[0] - buff[1];
    else
        pre_buff[0] = buff[0] - (buff[1] - buff[0]);

    pre_weight_value = weight_filter(pre_buff);

    //计算出下一个的加权平均值
    for (uint8_t count = 0; count < FILTER_NUM - 1; count++)
    {
        new_buff[count] = buff[count + 1];
    }
    new_buff[FILTER_NUM - 1] = newvalue;

    next_weight_value = weight_filter(new_buff);

    temp_weight_value = weight_value + weight_value - pre_weight_value;

    if (pre_weight_value >= weight_value) //递减时：把weight_value当成最大值，把temp_weight_value当成最小值（下面注释掉的更精确，但计算量更大，舍弃）
    {
        if ((next_weight_value >= (temp_weight_value - temp_weight_value / 4)) && (next_weight_value <= (weight_value + weight_value / 8))) //递减时，向更小的值偏移
            return 1;
        else
            return 0;
    }
    else //递增时：把weight_value当成最小值，把temp_weight_value当成最大值（下面注释掉的更精确，但计算量更大，舍弃）
    {
        if ((next_weight_value >= (weight_value - weight_value / 8)) && (next_weight_value <= (temp_weight_value + temp_weight_value / 4))) //递增是，向更大的值偏移
            return 1;
        else
            return 0;
    }
}

/*更精确，但是计算量大*/
//uint8_t dist_file(uint32_t *buff, uint32_t newvalue)
//{
//	uint32_t new_buff[FILTER_NUM] = {0};
//	uint32_t pre_buff[FILTER_NUM] = {0};
//	uint32_t max_buff[FILTER_NUM] = {0};
//	uint32_t min_buff[FILTER_NUM] = {0};
//
//	uint32_t weight_value = 0;
//    uint32_t next_weight_value = 0;
//    uint32_t pre_weight_value = 0;

//    uint32_t max_weight_value = 0;
//    uint32_t min_weight_value = 0;
//
////计算出之前的加权平均值
//	weight_value = weight_filter(buff);
//
////计算出上一个的加权平均值
//	for(uint8_t count = 1; count < FILTER_NUM; count++)
//	{
//    	pre_buff[count] = buff[count - 1];
//	}
//	if(buff[0] >= buff[1])
//    	pre_buff[0] = buff[0] + buff[0] - buff[1];
//	else
//    	pre_buff[0] = buff[0] - (buff[1] - buff[0]);
//
//	pre_weight_value = weight_filter(pre_buff);
//
////计算出下一个的加权平均值
//	for(uint8_t count = 0; count < FILTER_NUM - 1; count++)
//	{
//    	new_buff[count] = buff[count + 1];
//	}
//    new_buff[FILTER_NUM - 1] = newvalue;
//
//	next_weight_value = weight_filter(new_buff);
//
////估算出下一个的加权平均值的max
//	for(uint8_t count = 0; count < FILTER_NUM - 1; count++)
//	{
//    	max_buff[count] = buff[count + 1];
//	}
//    max_buff[FILTER_NUM - 1] = buff[FILTER_NUM  - 1] + weight_value - pre_weight_value;
//
//	max_weight_value = weight_filter(max_buff);
//

////估算出下一个的加权平均值的min
//	for(uint8_t count = 0; count < FILTER_NUM - 1; count++)
//	{
//    	min_buff[count] = buff[count + 1];
//	}
//    min_buff[FILTER_NUM - 1] = buff[FILTER_NUM  - 1] - (weight_value - pre_weight_value);
//
//	min_weight_value = weight_filter(min_buff);
//
////	printf("min_weight_value = %d\r\n",min_weight_value);
////	printf("max_weight_value = %d\r\n",max_weight_value);
////	printf("next_weight_value = %d\r\n",next_weight_value);
//
////	数据递增时：max是最大值，反之max是最小值
////  递增：向上预留，递减：向下预留
//	if(((next_weight_value >= (min_weight_value - min_weight_value/10)) && (next_weight_value <= (max_weight_value + max_weight_value/5))) \
//        || ((next_weight_value >= (max_weight_value - max_weight_value/5)) && (next_weight_value <= (min_weight_value + min_weight_value/10))))
//	{
//	    return 1;
//	}
//    else
//        return 0;
//}
