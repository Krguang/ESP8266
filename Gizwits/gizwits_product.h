/**
************************************************************
* @file         gizwits_product.h
* @brief        Corresponding gizwits_product.c header file (including product hardware and software version definition)
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#ifndef _GIZWITS_PRODUCT_H
#define _GIZWITS_PRODUCT_H

#include <stdint.h>
//#include "stm32f1xx_hal.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "hal_key.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "gizwits_protocol.h"


#ifndef ICACHE_FLASH_ATTR
#define ICACHE_FLASH_ATTR
#endif

/**@} */

#define typedef_t __packed typedef
	
/**
* MCU software version
*/
#define SOFTWARE_VERSION "03030000"
/**
* MCU hardware version
*/
#define HARDWARE_VERSION "03010100"

#ifndef SOFTWARE_VERSION
    #error "no define SOFTWARE_VERSION"
#endif

#ifndef HARDWARE_VERSION
    #error "no define HARDWARE_VERSION"
#endif

/**@name 设备状态数据上报间隔时间宏定义
* @{
*/
#define REPORT_TIME_MAX 6000 //6S
/**@} */

/** User area the current device state structure*/

void timerInit(void);
void uartInit(void);
void mcuRestart(void);
int32_t uartWrite(uint8_t *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
