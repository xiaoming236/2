/* Core/Inc/sensors.h */
/* Created by: xiaoming236
 * Created on: 2025-01-13
 * Description: 传感器相关定义和函数声明
 */

#ifndef __SENSORS_H
#define __SENSORS_H

#include "main.h"
#include "config.h"
#include "dht11.h"

/* 传感器数据结构 */
typedef struct {
    uint32_t timestamp;    // 时间戳
    float co_value;        // CO浓度(ppm)
    float temperature;     // 温度(℃)
    float humidity;        // 湿度(%)
    float smoke_value;     // 烟雾浓度
    float flame_value;     // 火焰强度
    uint8_t alarm_status;  // 报警状态
} SensorData_t;

/* 函数声明 */
HAL_StatusTypeDef Sensors_Init(void);
HAL_StatusTypeDef Sensors_ReadAll(SensorData_t* data);
uint8_t Check_Alarm_Condition(SensorData_t* data);

#endif /* __SENSORS_H */