/* Core/Inc/app_main.h */
#ifndef __APP_MAIN_H
#define __APP_MAIN_H

#include "main.h"
#include "sensors.h"
#include "display.h"
#include "wifi.h"
#include "control.h"

/* 系统状态定义 */
typedef enum {
    SYSTEM_INIT = 0,
    SYSTEM_NORMAL,
    SYSTEM_ALARM,
    SYSTEM_ERROR
} SystemState_t;

/* 函数声明 */
void APP_Init(void);
void APP_MainLoop(void);

#endif /* __APP_MAIN_H */