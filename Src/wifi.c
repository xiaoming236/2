/* Core/Src/wifi.c */
/* Created by: xiaoming236
 * Created on: 2025-01-13 09:34:12 UTC
 * Description: WiFi模块(ESP8266)通信实现
 */

#include "wifi.h"
#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart1;

static uint8_t wifi_rx_buffer[128];
static uint8_t wifi_tx_buffer[256];

static HAL_StatusTypeDef WIFI_SendCommand(const char* cmd)
{
    uint16_t len = strlen(cmd);
    return HAL_UART_Transmit(&huart1, (uint8_t*)cmd, len, 1000);
}

static HAL_StatusTypeDef WIFI_WaitResponse(const char* expected, uint32_t timeout)
{
    memset(wifi_rx_buffer, 0, sizeof(wifi_rx_buffer));
    if (HAL_UART_Receive(&huart1, wifi_rx_buffer, sizeof(wifi_rx_buffer), timeout) != HAL_OK) {
        return HAL_ERROR;
    }
    
    if (strstr((char*)wifi_rx_buffer, expected) != NULL) {
        return HAL_OK;
    }
    
    return HAL_ERROR;
}

HAL_StatusTypeDef WIFI_Init(void)
{
    // 复位ESP8266
    WIFI_SendCommand("AT+RST\r\n");
    HAL_Delay(2000);
    
    // 设置工作模式为Station
    WIFI_SendCommand("AT+CWMODE=1\r\n");
    HAL_Delay(500);
    
    // 连接到WiFi
    sprintf((char*)wifi_tx_buffer, "AT+CWJAP=\"%s\",\"%s\"\r\n", 
            WIFI_SSID, WIFI_PASSWORD);
    WIFI_SendCommand((char*)wifi_tx_buffer);
    HAL_Delay(5000);
    
    // 配置为TCP客户端
    sprintf((char*)wifi_tx_buffer, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",
            MQTT_SERVER, MQTT_PORT);
    WIFI_SendCommand((char*)wifi_tx_buffer);
    HAL_Delay(2000);
    
    return HAL_OK;
}

HAL_StatusTypeDef WIFI_SendData(SensorData_t* data)
{
    // 构建JSON数据
    sprintf((char*)wifi_tx_buffer, 
            "{\"co\":%.1f,\"temp\":%.1f,\"humi\":%.1f,"
            "\"smoke\":%.1f,\"flame\":%.1f,\"alarm\":%d}",
            data->co_value, data->temperature, data->humidity,
            data->smoke_value, data->flame_value, data->alarm_status);
    
    // 发送数据长度
    uint16_t len = strlen((char*)wifi_tx_buffer);
    sprintf((char*)wifi_tx_buffer, "AT+CIPSEND=%d\r\n", len);
    WIFI_SendCommand((char*)wifi_tx_buffer);
    HAL_Delay(100);
    
    // 发送数据
    return WIFI_SendCommand((char*)wifi_tx_buffer);
}

HAL_StatusTypeDef WIFI_ProcessResponse(void)
{
    if (HAL_UART_Receive(&huart1, wifi_rx_buffer, sizeof(wifi_rx_buffer), 100) == HAL_OK) {
        // 处理响应数据
        if (strstr((char*)wifi_rx_buffer, "CLOSED") != NULL) {
            // 重新连接
            WIFI_Init();
        }
        return HAL_OK;
    }
    return HAL_ERROR;
}