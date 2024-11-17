#include "ili9341.h"

// Define các chân GPIO (tùy vào cấu hình của bạn)
#define ILI9341_CS_PORT GPIOA
#define ILI9341_CS_PIN  GPIO_PIN_4
#define ILI9341_DC_PORT GPIOA
#define ILI9341_DC_PIN  GPIO_PIN_5
#define ILI9341_RST_PORT GPIOA
#define ILI9341_RST_PIN GPIO_PIN_6

// Một số macro điều khiển chân
#define ILI9341_CS_LOW() HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET)
#define ILI9341_CS_HIGH() HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_SET)
#define ILI9341_DC_LOW() HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_RESET)
#define ILI9341_DC_HIGH() HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET)
#define ILI9341_RST_LOW() HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_RESET)
#define ILI9341_RST_HIGH() HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_SET)

SPI_HandleTypeDef hspi1; // Khai báo biến SPI_HandleTypeDef
// Khởi tạo màn hình
void ILI9341_Init(SPI_HandleTypeDef *hspi) {
    ILI9341_Reset();

    ILI9341_SendCommand(ILI9341_SLEEP_OUT);
    HAL_Delay(120);

    ILI9341_SendCommand(ILI9341_DISPLAY_ON);
    HAL_Delay(10);

    ILI9341_SendCommand(ILI9341_PIXEL_FORMAT);
    uint8_t data = 0x55; // 16-bit/pixel
    ILI9341_SendData(&data, 1);
}

// Reset màn hình
void ILI9341_Reset(void) {
    ILI9341_RST_LOW();
    HAL_Delay(20);
    ILI9341_RST_HIGH();
    HAL_Delay(150);
}

// Gửi lệnh tới ILI9341
void ILI9341_SendCommand(uint8_t cmd) {
    ILI9341_DC_LOW();
    ILI9341_CS_LOW();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    ILI9341_CS_HIGH();
}

// Gửi dữ liệu tới ILI9341
void ILI9341_SendData(uint8_t *data, uint16_t size) {
    ILI9341_DC_HIGH();
    ILI9341_CS_LOW();
    HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY);
    ILI9341_CS_HIGH();
}

// Thiết lập cửa sổ hiển thị
void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    ILI9341_SendCommand(ILI9341_COLUMN_ADDR);
    uint8_t data[] = { x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF };
    ILI9341_SendData(data, sizeof(data));

    ILI9341_SendCommand(ILI9341_PAGE_ADDR);
    data[0] = y0 >> 8;
    data[1] = y0 & 0xFF;
    data[2] = y1 >> 8;
    data[3] = y1 & 0xFF;
    ILI9341_SendData(data, sizeof(data));

    ILI9341_SendCommand(ILI9341_GRAM);
}

// Vẽ 1 pixel với màu sắc nhất định
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)) return;

    ILI9341_SetAddressWindow(x, y, x, y);
    uint8_t data[] = { color >> 8, color & 0xFF };
    ILI9341_SendData(data, sizeof(data));
}
