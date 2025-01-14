/* Core/Src/control.c */
/* Created by: xiaoming236
 * Created on: 2025-01-13 09:35:29 UTC
 * Description: 控制设备(风扇、报警器、喷水装置)驱动实现
 */

#include "control.h"

void Control_Init(void)
{
    // 初始化时关闭所有设备
    Control_SetAlarm(0);
    Control_SetFan(0);
    Control_SetWater(0);
    
    // 初始化LED指示灯
    HAL_GPIO_WritePin(GPIOB, led_Pin, GPIO_PIN_SET);
}

void Control_SetAlarm(uint8_t status)
{
    if(status)
    {
        HAL_GPIO_WritePin(GPIOB, baojingqi_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, led_Pin, GPIO_PIN_SET);  // LED闪烁
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, baojingqi_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, led_Pin, GPIO_PIN_RESET);
    }
}

void Control_SetFan(uint8_t speed)
{
    static uint8_t last_speed = 0;
    
    if(speed != last_speed)
    {
        if(speed > 0)
        {
            HAL_GPIO_WritePin(GPIOB, fengshan_Pin, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOB, fengshan_Pin, GPIO_PIN_RESET);
        }
        last_speed = speed;
    }
}

void Control_SetWater(uint8_t status)
{
    static uint8_t last_status = 0;
    
    if(status != last_status)
    {
        HAL_GPIO_WritePin(GPIOB, penshuishebei_Pin,
            status ? GPIO_PIN_SET : GPIO_PIN_RESET);
        last_status = status;
    }
}

// 定时器中断回调函数，用于LED闪烁
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        HAL_GPIO_TogglePin(GPIOB, led_Pin);
    }
}