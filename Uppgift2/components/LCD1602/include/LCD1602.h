#ifndef LCD1602_H
#define LCD1602_H

#include <stdio.h>
#include <iostream>
#include <string>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include <esp_timer.h>

// #define I2C_MASTER_SCL_IO           19    /*!< gpio number for I2C master clock */
// #define I2C_MASTER_SDA_IO           18    /*!< gpio number for I2C master data  */
// #define I2C_MASTER_NUM              I2C_NUM_0 /*!< I2C port number for master dev */
// #define I2C_MASTER_FREQ_HZ          100000 /*!< I2C master clock frequency */
// #define I2C_MASTER_TX_BUF_DISABLE   0 /*!< I2C master doesn't need buffer */
// #define I2C_MASTER_RX_BUF_DISABLE   0 /*!< I2C master doesn't need buffer */

#define I2C_LCD_ADDR                0x27 /*!< I2C address of the LCD */
 
// Commands
#define LCD_CMD_CLEAR_DISPLAY       0x01
#define LCD_CMD_RETURN_HOME         0x02
#define LCD_CMD_ENTRY_MODE_SET      0x04
#define LCD_CMD_DISPLAY_CONTROL     0x08
#define LCD_CMD_CURSOR_SHIFT        0x10
#define LCD_CMD_FUNCTION_SET        0x20
#define LCD_CMD_SET_CGRAM_ADDR      0x40
#define LCD_CMD_SET_DDRAM_ADDR      0x80
 
#define LCD_FLAG_DISPLAY_ON         0x04
#define LCD_FLAG_DISPLAY_OFF        0x00
#define LCD_FLAG_CURSOR_ON          0x02
#define LCD_FLAG_CURSOR_OFF         0x00
#define LCD_FLAG_BLINK_ON           0x01
#define LCD_FLAG_BLINK_OFF          0x00
 
#define LCD_FLAG_8BIT_MODE          0x10
#define LCD_FLAG_4BIT_MODE          0x00
#define LCD_FLAG_2LINE              0x08
#define LCD_FLAG_1LINE              0x00
#define LCD_FLAG_5x10DOTS           0x04
#define LCD_FLAG_5x8DOTS            0x00

class LCD1602
{
    private:
    gpio_num_t sclPin;
    gpio_num_t sdaPin;
    i2c_port_t i2cport;

    protected:
    esp_err_t lcd_send_cmd(uint8_t cmd);
    esp_err_t lcd_send_data(uint8_t data);

    public:
    LCD1602(gpio_num_t sclPin, gpio_num_t sdaPin, i2c_port_t port);
    ~LCD1602();

    void lcd_init(void);
    void lcd_print(const char *str);
    void ClearScreen();
};

#endif
