/* Core/Inc/st7735.h */
/* Created by: xiaoming236
 * Created on: 2025-01-13
 * Description: ST7735显示屏驱动头文件
 */

#ifndef __ST7735_H
#define __ST7735_H

#include "main.h"

/* 显示屏参数 */
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160

/* ST7735命令 */
#define ST7735_SWRESET   0x01
#define ST7735_SLPOUT    0x11
#define ST7735_COLMOD    0x3A
#define ST7735_MADCTL    0x36
#define ST7735_CASET     0x2A
#define ST7735_RASET     0x2B
#define ST7735_RAMWR     0x2C
#define ST7735_DISPON    0x29

/* 颜色定义 */
#define ST7735_BLACK   0x0000
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_BLUE    0x001F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF
/* Core/Inc/st7735.h */
// 在已有内容后添加
/* 5x7字体数据 */
static const uint8_t font5x7[95][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00},  // 空格 20
    {0x00, 0x00, 0x5F, 0x00, 0x00},  // !
    {0x00, 0x07, 0x00, 0x07, 0x00},  // "
    // ... 其他字符数据
    {0x14, 0x08, 0x3E, 0x08, 0x14},  // ×
    {0x08, 0x08, 0x3E, 0x08, 0x08},  // +
    // 添加完整的字体数据
};

/* 函数声明 */
void ST7735_Init(void);
void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ST7735_WriteString(uint16_t x, uint16_t y, const char* str, uint16_t color);
void ST7735_FillScreen(uint16_t color);
void ST7735_DrawChar(uint16_t x, uint16_t y, char ch, uint16_t color);

#endif /* __ST7735_H */