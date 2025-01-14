/* Core/Inc/config.h */
/* Created by: xiaoming236
 * Created on: 2025-01-13
 * Description: 系统配置文件,包含阈值和网络设置
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/* 传感器阈值定义 */
#define CO_THRESHOLD      100.0f  // CO报警阈值(ppm)
#define TEMP_MAX         45.0f    // 温度上限(℃)
#define SMOKE_THRESHOLD   500.0f  // 烟雾报警阈值
#define FLAME_THRESHOLD   100.0f  // 火焰报警阈值

/* WiFi配置 */
#define WIFI_SSID       "YourSSID"
#define WIFI_PASSWORD   "YourPassword"
#define MQTT_SERVER     "mqtt.yourserver.com"
#define MQTT_PORT       1883
#define MQTT_TOPIC      "fire_alarm/sensor_data"

#endif /* __CONFIG_H */