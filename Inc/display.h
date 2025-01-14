/* Core/Inc/display.h */
/* Created by: xiaoming236
 * Created on: 2025-01-13
 * Description: 显示控制头文件
 */

#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "main.h"
#include "st7735.h"
#include "sensors.h"
#include "config.h"

void Display_Init(void);
void Display_ShowBootScreen(void);
void Display_ShowMainScreen(void);
void Display_UpdateData(SensorData_t* data);
void Display_ShowError(const char* error);

#endif /* __DISPLAY_H */