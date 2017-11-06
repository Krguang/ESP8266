/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related       hardware initialization 
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

#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "usart.h"

#define MODULE_TYPE 1
#define GIZWITS_LOG printf


#ifndef GPIO_KEY_NUM
#define GPIO_KEY_NUM 2  
#endif
extern keyTypedef_t singleKey[GPIO_KEY_NUM];                   ///< 定义单个按键成员数组指针
extern keysTypedef_t keys;                                     ///< 定义总的按键模块结构体指针
/** 用户区设备状态变量 */
extern dataPoint_t currentDataPoint;
/**
* @brief 事件处理接口

* 说明：

* 1.用户可以对WiFi模组状态的变化进行自定义的处理

* 2.用户可以在该函数内添加数据点事件处理逻辑，如调用相关硬件外设的操作接口

* @param[in] info : 事件队列
* @param[in] data : 协议数据
* @param[in] len : 协议数据长度
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
	uint8_t i = 0;
	dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
	moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
	protocolTime_t *ptime = (protocolTime_t *)gizdata;

#if MODULE_TYPE
	gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
	moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

	if ((NULL == info) || (NULL == gizdata))
	{
		return -1;
	}

	for (i = 0; i<info->num; i++)
	{
		switch (info->event[i])
		{
		case EVENT_led3:
			currentDataPoint.valueled3 = dataPointPtr->valueled3;
			GIZWITS_LOG("Evt: EVENT_led3 %d \n", currentDataPoint.valueled3);
			if (0x01 == currentDataPoint.valueled3)
			{
				//user handle
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3,GPIO_PIN_RESET);
			}
			else
			{
				//user handle    
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
			}
			break;
		case EVENT_led4:
			currentDataPoint.valueled4 = dataPointPtr->valueled4;
			GIZWITS_LOG("Evt: EVENT_led4 %d \n", currentDataPoint.valueled4);
			if (0x01 == currentDataPoint.valueled4)
			{
				//user handle
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
			}
			else
			{
				//user handle    
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
			}
			break;


		case EVENT_ledValue:
			currentDataPoint.valueledValue = dataPointPtr->valueledValue;
			GIZWITS_LOG("Evt:EVENT_ledValue %d\n", currentDataPoint.valueledValue);
			//user handle
			break;


		case WIFI_SOFTAP:
			break;
		case WIFI_AIRLINK:
			break;
		case WIFI_STATION:
			break;
		case WIFI_CON_ROUTER:

			break;
		case WIFI_DISCON_ROUTER:

			break;
		case WIFI_CON_M2M:

			break;
		case WIFI_DISCON_M2M:
			break;
		case WIFI_RSSI:
			GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
			break;
		case TRANSPARENT_DATA:
			GIZWITS_LOG("TRANSPARENT_DATA \n");
			//user handle , Fetch data from [data] , size is [len]
			break;
		case WIFI_NTP:
			GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n", ptime->year, ptime->month, ptime->day, ptime->hour, ptime->minute, ptime->second, ptime->ntp);
			break;
		case MODULE_INFO:
			GIZWITS_LOG("MODULE INFO ...\n");
#if MODULE_TYPE
			GIZWITS_LOG("GPRS MODULE ...\n");
			//Format By gprsInfo_t
#else
			GIZWITS_LOG("WIF MODULE ...\n");
			//Format By moduleInfo_t
			GIZWITS_LOG("moduleType : [%d] \n", ptModuleInfo->moduleType);
#endif
			break;
		default:
			break;
		}
	}

	return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
 /*
    currentDataPoint.valuepower_sw = ;//Add Sensor Data Collection

    */
	//currentDataPoint.valuepower_sw = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3);
	currentDataPoint.valueO2 = 189;
   
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
      currentDataPoint.valuepower_sw = ;
    */

}

/**
* @brief 系统毫秒计时维护函数,毫秒自增,溢出归零

* @param none
* @return none
*/
static uint32_t timerMsCount;
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief 读取系统时间毫秒计时数

* @param none
* @return 系统时间毫秒数
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}


/**
* @brief MCU复位函数

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);
    HAL_NVIC_SystemReset();
}

/**@} */

/**
* @brief 定时器TIM中断处理函数

* @param none
* @return none
*/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance ==TIM6)//tim10 1ms中断，作为MCU和WIFI模组的心跳用
  {
    gizTimerMs();
  }
  if(htim->Instance ==TIM7)//tim11 1ms中断，按键检测逻辑
  {
    keyHandle((keysTypedef_t *)&keys);
  }
}

/**
* @brief 定时器TIM定时器初始化

* @param none
* @return none
*/
void timerInit(void)
{
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
}
/**
* @brief USART串口中断函数

* 接收功能，用于接收与WiFi模组间的串口协议数据
* @param none
* @return none
*/

uint8_t RxData=0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  if(UartHandle->Instance ==USART2 )
  {	
    gizPutData(&RxData, 1);
    HAL_UART_Receive_IT(&huart2, &RxData,1);
  }
  
}

/**
* @brief 串口写操作，发送数据到WiFi模组
*
* @param buf      : 数据地址
* @param len       : 数据长度
*
* @return : 正确返回有效数据长度;-1，错误返回
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    static uint8_t aa=0x55;
    
    if(NULL == buf)
    {
        return -1;
    }
#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
#endif
    
    for(i=0; i<len; i++)
    {
HAL_UART_Transmit(&huart2, (uint8_t *)&buf[i], 1,HAL_MAX_DELAY); 
#ifdef PROTOCOL_DEBUG
        GIZWITS_LOG("%02x ", buf[i]);
#endif
        if(i >=2 && buf[i] == 0xFF)
        {
HAL_UART_Transmit(&huart2, (uint8_t *)&aa, 1,HAL_MAX_DELAY); 
#ifdef PROTOCOL_DEBUG
        GIZWITS_LOG("%02x ", 0x55);
#endif
        }
    }
    
#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("\n");
#endif
    
    return len;
}

/**
* @brief USART串口初始化

* 用于与WiFi模组间的串口通信
* @param none
* @return none
*/
void uartInit(void)
{
  HAL_UART_Receive_IT(&huart2, &RxData,1);
}
