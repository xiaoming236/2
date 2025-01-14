/* Core/Inc/wifi.h */
/* Created by: xiaoming236
 * Created on: 2025-01-13
 * Description: WiFi通信相关头文件
 */

#ifndef __WIFI_H
#define __WIFI_H

#include "main.h"
#include "config.h"
#include "sensors.h"

HAL_StatusTypeDef WIFI_Init(void);
HAL_StatusTypeDef WIFI_SendData(SensorData_t* data);
HAL_StatusTypeDef WIFI_ProcessResponse(void);

#endif /* __WIFI_H */