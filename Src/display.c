/* Core/Src/display.c */
/* Created by: xiaoming236
 * Created on: 2025-01-13
 * Description: 显示控制实现
 */

#include "display.h"
#include <stdio.h>

void Display_Init(void)
{
    ST7735_Init();
    ST7735_FillScreen(ST7735_BLACK);
}

void Display_ShowBootScreen(void)
{
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_WriteString(10, 60, "Fire Alarm System", ST7735_GREEN);
    ST7735_WriteString(20, 80, "Initializing...", ST7735_WHITE);
}

void Display_ShowMainScreen(void)
{
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_WriteString(5, 5, "Fire Alarm Monitor", ST7735_GREEN);
}

void Display_UpdateData(SensorData_t* data)
{
    char buffer[32];
    
    /* Display CO concentration */
    sprintf(buffer, "CO: %.1f ppm", data->co_value);
    ST7735_WriteString(5, 25, buffer, 
        data->co_value > CO_THRESHOLD ? ST7735_RED : ST7735_GREEN);
    
    /* Display temperature */
    sprintf(buffer, "Temp: %.1f C", data->temperature);
    ST7735_WriteString(5, 45, buffer,
        data->temperature > TEMP_MAX ? ST7735_RED : ST7735_GREEN);
    
    /* Display humidity */
    sprintf(buffer, "Humi: %.1f %%", data->humidity);
    ST7735_WriteString(5, 65, buffer, ST7735_WHITE);
    
    /* Display smoke value */
    sprintf(buffer, "Smoke: %.1f", data->smoke_value);
    ST7735_WriteString(5, 85, buffer,
        data->smoke_value > SMOKE_THRESHOLD ? ST7735_RED : ST7735_GREEN);
    
    /* Display flame value */
    sprintf(buffer, "Flame: %.1f", data->flame_value);
    ST7735_WriteString(5, 105, buffer,
        data->flame_value > FLAME_THRESHOLD ? ST7735_RED : ST7735_GREEN);
    
    /* Display alarm status */
    if(data->alarm_status) {
        ST7735_WriteString(5, 140, "! ALARM !", ST7735_RED);
    } else {
        ST7735_WriteString(5, 140, "Normal  ", ST7735_GREEN);
    }
}

void Display_ShowError(const char* error)
{
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_WriteString(5, 60, "ERROR:", ST7735_RED);
    ST7735_WriteString(5, 80, error, ST7735_RED);
}