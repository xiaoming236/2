#include "app_main.h"
/* Core/Src/app_main.c */
#include "app_main.h"
#include "control.h"
#include "sensors.h"
#include "display.h"
#include "wifi.h"

static SystemState_t systemState = SYSTEM_INIT;

// ... 其他代码保持不变

// 全局变量
static SensorData_t sensorData = {0};
static uint32_t lastUpdateTime = 0;
static uint32_t lastWifiTime = 0;

// 系统初始化
void APP_Init(void)
{
    // 初始化显示屏
    Display_Init();
    Display_ShowBootScreen();
    
    // 初始化传感器
    if (Sensors_Init() != HAL_OK) {
        systemState = SYSTEM_ERROR;
        return;
    }
    
    // 初始化WiFi
    if (WIFI_Init() != HAL_OK) {
        Display_ShowError("WiFi Init Failed");
        HAL_Delay(2000);
    }
    
    systemState = SYSTEM_NORMAL;
    Display_ShowMainScreen();
}

// 主循环
void APP_MainLoop(void)
{
    uint32_t currentTime = HAL_GetTick();
    
    // 读取传感器数据
    if (Sensors_ReadAll(&sensorData) != HAL_OK) {
        systemState = SYSTEM_ERROR;
        Display_ShowError("Sensor Error");
        return;
    }
    
    // 检查报警条件
    if (Check_Alarm_Condition(&sensorData)) {
        systemState = SYSTEM_ALARM;
        Control_SetAlarm(1);    // 开启报警器
        Control_SetFan(255);    // 开启风扇
        Control_SetWater(1);    // 开启喷水装置
    } else if (systemState == SYSTEM_ALARM) {
        systemState = SYSTEM_NORMAL;
        Control_SetAlarm(0);
        Control_SetFan(0);
        Control_SetWater(0);
    }
    
    // 更新显示(每500ms)
    if (currentTime - lastUpdateTime >= 500) {
        Display_UpdateData(&sensorData);
        lastUpdateTime = currentTime;
    }
    
    // WiFi数据上报(每2秒)
    if (currentTime - lastWifiTime >= 2000) {
        WIFI_SendData(&sensorData);
        lastWifiTime = currentTime;
    }
}