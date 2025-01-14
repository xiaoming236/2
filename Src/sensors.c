/* Core/Src/sensors.c */
#include "sensors.h"

static uint16_t ADC_DMA_Buffer[4] = {0};

HAL_StatusTypeDef Sensors_Init(void)
{
    /* Start ADC with DMA */
    if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_DMA_Buffer, 4) != HAL_OK)
    {
        return HAL_ERROR;
    }
    
    return HAL_OK;
}

HAL_StatusTypeDef Sensors_ReadAll(SensorData_t* data)
{
    data->timestamp = HAL_GetTick();
    
    /* Convert ADC values to physical quantities */
    data->co_value = (float)ADC_DMA_Buffer[0] * 3.3f / 4096.0f * 1000;
    data->smoke_value = (float)ADC_DMA_Buffer[1] * 3.3f / 4096.0f * 1000;
    data->flame_value = (float)ADC_DMA_Buffer[2] * 3.3f / 4096.0f * 1000;
    
    /* Read DHT11 */
    float temp, humi;
    if (DHT11_Read(&temp, &humi) == HAL_OK)
    {
        data->temperature = temp;
        data->humidity = humi;
    }
    else
    {
        return HAL_ERROR;
    }
    
    return HAL_OK;
}

uint8_t Check_Alarm_Condition(SensorData_t* data)
{
    if (data->co_value > CO_THRESHOLD ||
        data->temperature > TEMP_MAX ||
        data->smoke_value > SMOKE_THRESHOLD ||
        data->flame_value > FLAME_THRESHOLD)
    {
        return 1;
    }
    return 0;
}