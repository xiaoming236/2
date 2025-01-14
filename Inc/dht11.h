/* Core/Inc/dht11.h */
/* Created by: xiaoming236
 * Created on: 2025-01-13
 * Description: DHT11温湿度传感器驱动头文件
 */

#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"

HAL_StatusTypeDef DHT11_Init(void);
HAL_StatusTypeDef DHT11_Read(float* temperature, float* humidity);

#endif /* __DHT11_H */