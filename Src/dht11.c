/* Core/Src/dht11.c */
/* Created by: xiaoming236
 * Created on: 2025-01-13 09:35:29 UTC
 * Description: DHT11温湿度传感器驱动实现
 */

#include "dht11.h"

static void DHT11_DelayUs(uint32_t us)
{
    uint32_t delay = us * (SystemCoreClock / 1000000U);
    while (delay--) { }
}

static void DHT11_SetPinOutput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pin = dht11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(dht11_GPIO_Port, &GPIO_InitStruct);
}

static void DHT11_SetPinInput(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pin = dht11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(dht11_GPIO_Port, &GPIO_InitStruct);
}

HAL_StatusTypeDef DHT11_Init(void)
{
    DHT11_SetPinOutput();
    HAL_GPIO_WritePin(dht11_GPIO_Port, dht11_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);  // 等待DHT11稳定
    return HAL_OK;
}

static uint8_t DHT11_ReadByte(void)
{
    uint8_t i, byte = 0;
    
    for(i = 0; i < 8; i++)
    {
        byte <<= 1;
        // 等待50us低电平结束
        while(HAL_GPIO_ReadPin(dht11_GPIO_Port, dht11_Pin) == GPIO_PIN_RESET);
        DHT11_DelayUs(30);  // 延时30us
        
        // 如果高电平持续时间超过30us，则为1，否则为0
        if(HAL_GPIO_ReadPin(dht11_GPIO_Port, dht11_Pin) == GPIO_PIN_SET)
        {
            byte |= 0x01;
            while(HAL_GPIO_ReadPin(dht11_GPIO_Port, dht11_Pin) == GPIO_PIN_SET);
        }
    }
    
    return byte;
}

HAL_StatusTypeDef DHT11_Read(float* temperature, float* humidity)
{
    uint8_t data[5] = {0};
    uint32_t timeout;
    
    // 发送起始信号
    DHT11_SetPinOutput();
    HAL_GPIO_WritePin(dht11_GPIO_Port, dht11_Pin, GPIO_PIN_RESET);
    HAL_Delay(18);  // 至少18ms
    HAL_GPIO_WritePin(dht11_GPIO_Port, dht11_Pin, GPIO_PIN_SET);
    DHT11_DelayUs(30);  // 20-40us
    
    // 切换为输入模式
    DHT11_SetPinInput();
    
    // 等待DHT11响应
    timeout = HAL_GetTick();
    while(HAL_GPIO_ReadPin(dht11_GPIO_Port, dht11_Pin) == GPIO_PIN_SET)
    {
        if(HAL_GetTick() - timeout > HAL_MAX_DELAY) return HAL_TIMEOUT;
    }
    
    timeout = HAL_GetTick();
    while(HAL_GPIO_ReadPin(dht11_GPIO_Port, dht11_Pin) == GPIO_PIN_RESET)
    {
        if(HAL_GetTick() - timeout > HAL_MAX_DELAY) return HAL_TIMEOUT;
    }
    
    timeout = HAL_GetTick();
    while(HAL_GPIO_ReadPin(dht11_GPIO_Port, dht11_Pin) == GPIO_PIN_SET)
    {
        if(HAL_GetTick() - timeout > HAL_MAX_DELAY) return HAL_TIMEOUT;
    }
    
    // 读取40位数据
    for(int i = 0; i < 5; i++)
    {
        data[i] = DHT11_ReadByte();
    }
    
    // 校验
    if(data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
    {
        *humidity = (float)data[0] + data[1] * 0.1f;
        *temperature = (float)data[2] + data[3] * 0.1f;
        return HAL_OK;
    }
    
    return HAL_ERROR;
}

// 添加温度补偿
void DHT11_CompensateReadings(float* temperature, float* humidity)
{
    // 温度补偿值，可根据实际情况调整
    const float TEMP_OFFSET = -2.0f;
    const float HUMI_OFFSET = 5.0f;
    
    *temperature += TEMP_OFFSET;
    *humidity += HUMI_OFFSET;
    
    // 确保数值在合理范围内
    if(*temperature < -40.0f) *temperature = -40.0f;
    if(*temperature > 80.0f) *temperature = 80.0f;
    if(*humidity < 0.0f) *humidity = 0.0f;
    if(*humidity > 100.0f) *humidity = 100.0f;
}