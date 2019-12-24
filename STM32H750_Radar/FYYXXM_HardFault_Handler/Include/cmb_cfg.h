/*
 * This file is part of the CmBacktrace Library.
 *
 * Copyright (c) 2016, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: It is the configure head file for this library.
 * Created on: 2016-12-15
 */

#ifndef _CMB_CFG_H_
#define _CMB_CFG_H_

/*
| 配置名称 |功能|备注|
|:--|:--|:--|
|cmb_println(...)|错误及诊断信息输出|必须配置|
|CMB_USING_BARE_METAL_PLATFORM|是否使用在裸机平台|使用则定义该宏|
|CMB_USING_OS_PLATFORM|是否使用在操作系统平台|操作系统与裸机必须二选一|
|CMB_OS_PLATFORM_TYPE|操作系统平台|RTT/UCOSII/UCOSIII/FREERTOS|
|CMB_CPU_PLATFORM_TYPE|CPU平台|M0/M3/M4/M7|
|CMB_USING_DUMP_STACK_INFO|是否使用 Dump 堆栈的功能|使用则定义该宏|
|CMB_PRINT_LANGUAGE|输出信息时的语言|CHINESE/ENGLISH|
*/
/* print line, must config by user */
#define cmb_println(...)  printf(__VA_ARGS__);printf("\r\n") 
/* enable bare metal(no OS) platform */
/* #define CMB_USING_BARE_METAL_PLATFORM */
/* enable OS platform */
#define CMB_USING_OS_PLATFORM 
/* OS platform type, must config when CMB_USING_OS_PLATFORM is enable */
 #define CMB_OS_PLATFORM_TYPE   CMB_OS_PLATFORM_FREERTOS 
/* cpu platform type, must config by user */
#define CMB_CPU_PLATFORM_TYPE   CMB_CPU_ARM_CORTEX_M3
/* enable dump stack information */
//#define CMB_USING_DUMP_STACK_INFO 
/* language of print information */
#define CMB_PRINT_LANGUAGE  CMB_PRINT_LANGUAGE_CHINESE 
#endif /* _CMB_CFG_H_ */
