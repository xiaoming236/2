/* Core/Inc/control.h */
#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"

/* GPIO定义 */
#define CONTROL_GPIO_Port     GPIOB
#define ALARM_PIN            baojingqi_Pin
#define FAN_PIN             fengshan_Pin
#define WATER_PIN           penshuishebei_Pin
#define LED_PIN             led_Pin

/* 函数声明 */
void Control_Init(void);
void Control_SetAlarm(uint8_t status);
void Control_SetFan(uint8_t speed);
void Control_SetWater(uint8_t status);

#endif /* __CONTROL_H */