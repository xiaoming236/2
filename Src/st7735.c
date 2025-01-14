/* Core/Src/st7735.c */
/* Created by: xiaoming236
 * Created on: 2025-01-13 09:34:12 UTC
 * Description: ST7735 LCD????
 */

#include "st7735.h"

extern SPI_HandleTypeDef hspi2;

static void ST7735_WriteCommand(uint8_t cmd)
{
    HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

static void ST7735_WriteData(uint8_t data)
{
    HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, &data, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

static void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    /* Column Address Set */
    ST7735_WriteCommand(ST7735_CASET);
    ST7735_WriteData(0x00);
    ST7735_WriteData(x0);
    ST7735_WriteData(0x00);
    ST7735_WriteData(x1);

    /* Row Address Set */
    ST7735_WriteCommand(ST7735_RASET);
    ST7735_WriteData(0x00);
    ST7735_WriteData(y0);
    ST7735_WriteData(0x00);
    ST7735_WriteData(y1);

    /* Memory Write */
    ST7735_WriteCommand(ST7735_RAMWR);
}

void ST7735_Init(void)
{
    /* Hardware Reset */
    HAL_GPIO_WritePin(ST7735_RST_GPIO_Port, ST7735_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(ST7735_RST_GPIO_Port, ST7735_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(20);

    /* Software Reset */
    ST7735_WriteCommand(ST7735_SWRESET);
    HAL_Delay(150);

    /* Out of Sleep Mode */
    ST7735_WriteCommand(ST7735_SLPOUT);
    HAL_Delay(500);

    /* Color Mode: 16-bit per pixel */
    ST7735_WriteCommand(ST7735_COLMOD);
    ST7735_WriteData(0x05);
    HAL_Delay(10);

    /* Memory Access Control */
    ST7735_WriteCommand(ST7735_MADCTL);
    ST7735_WriteData(0xC8);

    /* Display On */
    ST7735_WriteCommand(ST7735_DISPON);
}

void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
        return;

    ST7735_SetAddressWindow(x, y, x+1, y+1);

    uint8_t data[2] = {color >> 8, color & 0xFF};
    
    HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, data, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, uint16_t color)
{
    // ???????????
    if(ch < 32 || ch > 126) return;
    
    // ??????
    for(int i = 0; i < 5; i++) {
        uint8_t line = font5x7[ch - 32][i];
        for(int j = 0; j < 7; j++) {
            if(line & (1 << j)) {
                ST7735_DrawPixel(x + i, y + j, color);
            }
        }
    }
}


void ST7735_WriteString(uint16_t x, uint16_t y, const char* str, uint16_t color)
{
    while(*str) {
        if(x + 6 >= ST7735_WIDTH) {
            x = 0;
            y += 8;
            if(y + 8 >= ST7735_HEIGHT) {
                break;
            }
        }
        ST7735_WriteChar(x, y, *str++, color);
        x += 6;
    }
}

void ST7735_FillScreen(uint16_t color)
{
    uint8_t data[2] = {color >> 8, color & 0xFF};
    ST7735_SetAddressWindow(0, 0, ST7735_WIDTH-1, ST7735_HEIGHT-1);
    
    HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    
    for(uint32_t i = 0; i < ST7735_WIDTH * ST7735_HEIGHT; i++) {
        HAL_SPI_Transmit(&hspi2, data, 2, HAL_MAX_DELAY);
    }
    
    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}