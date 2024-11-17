#ifndef ILI9341_H
#define ILI9341_H

#include "stm32f7xx_hal.h"

// Kích thước màn hình ILI9341
#define ILI9341_WIDTH  240
#define ILI9341_HEIGHT 320

// Các lệnh cơ bản của ILI9341
#define ILI9341_RESET 0x01
#define ILI9341_SLEEP_OUT 0x11
#define ILI9341_DISPLAY_ON 0x29
#define ILI9341_COLUMN_ADDR 0x2A
#define ILI9341_PAGE_ADDR 0x2B
#define ILI9341_GRAM 0x2C
#define ILI9341_MAC 0x36
#define ILI9341_PIXEL_FORMAT 0x3A

// Khai báo extern để dùng biến hspi1 từ file khác
extern SPI_HandleTypeDef hspi1;


// Các chức năng cơ bản của driver
void ILI9341_Init(SPI_HandleTypeDef *hspi);
void ILI9341_Reset(void);
void ILI9341_SendCommand(uint8_t cmd);
void ILI9341_SendData(uint8_t *data, uint16_t size);
void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);

#endif // ILI9341_H