/* Core/Inc/main.h */
/* Created by: xiaoming236
 * Created on: 2025-01-13
 * Description: 主要头文件,包含基础定义和外设句柄
 */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* 导出外设句柄 */
extern SPI_HandleTypeDef hspi2;
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;

/* GPIO引脚定义 */
#define CO_Pin              GPIO_PIN_0
#define YANWU_Pin          GPIO_PIN_2
#define FAME_Pin           GPIO_PIN_3
#define dht11_Pin          GPIO_PIN_1
#define dht11_GPIO_Port    GPIOA

#define fengshan_Pin       GPIO_PIN_0
#define baojingqi_Pin      GPIO_PIN_1
#define penshuishebei_Pin  GPIO_PIN_10
#define led_Pin            GPIO_PIN_12

#define ST7735_CS_Pin      GPIO_PIN_12
#define ST7735_CS_GPIO_Port GPIOB
#define ST7735_DC_Pin      GPIO_PIN_14
#define ST7735_DC_GPIO_Port GPIOB
#define ST7735_RST_Pin     GPIO_PIN_4
#define ST7735_RST_GPIO_Port GPIOB

/* 函数声明 */
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */