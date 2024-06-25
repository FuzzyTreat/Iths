
#include "LCD1602.h"

static const char *TAG = "LCD1602";

LCD1602::LCD1602(gpio_num_t sclPin, gpio_num_t sdaPin, i2c_port_t port) : sclPin(sclPin), sdaPin(sdaPin), i2cport(port)
{
    lcd_init();
}

LCD1602::~LCD1602(){}
 
esp_err_t LCD1602::lcd_send_cmd(uint8_t cmd) {
    uint8_t data[4];
    data[0] = (cmd & 0xF0) | 0x0C;
    data[1] = (cmd & 0xF0) | 0x08;
    data[2] = ((cmd << 4) & 0xF0) | 0x0C;
    data[3] = ((cmd << 4) & 0xF0) | 0x08;
    
    i2c_cmd_handle_t handle = i2c_cmd_link_create();
    i2c_master_start(handle);
    i2c_master_write_byte(handle, (I2C_LCD_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(handle, data, sizeof(data), true);
    i2c_master_stop(handle);
    
    esp_err_t ret = i2c_master_cmd_begin(i2cport, handle, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(handle);
    
    return ret;
}
 
esp_err_t LCD1602::lcd_send_data(uint8_t data) {
    uint8_t data_arr[4];
    data_arr[0] = (data & 0xF0) | 0x0D;
    data_arr[1] = (data & 0xF0) | 0x09;
    data_arr[2] = ((data << 4) & 0xF0) | 0x0D;
    data_arr[3] = ((data << 4) & 0xF0) | 0x09;

    i2c_cmd_handle_t handle = i2c_cmd_link_create();
    i2c_master_start(handle);
    i2c_master_write_byte(handle, (I2C_LCD_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(handle, data_arr, sizeof(data_arr), true);
    i2c_master_stop(handle);
    
    esp_err_t ret = i2c_master_cmd_begin(i2cport, handle, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(handle);
    
    return ret;
}
 
void LCD1602::lcd_init(void) {
    // The `lcd_send_cmd(0x30);` command is used to initialize the LCD module. 
    // In 4-bit mode (which is commonly used for interfacing with LCDs), this command sets the 
    // module to expect 8-bit communication. It is typically sent three times to ensure proper 
    // initialization and to switch the LCD from its power-on state to a known operational mode. 
    // This sequence is a common initialization procedure specified by most HD44780-compatible 
    // LCD controllers.

    vTaskDelay(pdMS_TO_TICKS(50)); // wait for >40ms
    lcd_send_cmd(0x30);
    vTaskDelay(pdMS_TO_TICKS(10));  // wait for >4.1ms
    lcd_send_cmd(0x30);
    vTaskDelay(pdMS_TO_TICKS(10));  // wait for >100us
    lcd_send_cmd(0x30);
    vTaskDelay(pdMS_TO_TICKS(10));
    lcd_send_cmd(0x20);  // 4-bit mode
    vTaskDelay(pdMS_TO_TICKS(10));
 
    lcd_send_cmd(LCD_CMD_FUNCTION_SET | LCD_FLAG_2LINE | LCD_FLAG_5x8DOTS);
    vTaskDelay(pdMS_TO_TICKS(10));
    lcd_send_cmd(LCD_CMD_DISPLAY_CONTROL | LCD_FLAG_DISPLAY_ON | LCD_FLAG_CURSOR_OFF | LCD_FLAG_BLINK_OFF);
    vTaskDelay(pdMS_TO_TICKS(10));
    lcd_send_cmd(LCD_CMD_CLEAR_DISPLAY);
    vTaskDelay(pdMS_TO_TICKS(10));
    lcd_send_cmd(LCD_CMD_ENTRY_MODE_SET | 0x02);
    vTaskDelay(pdMS_TO_TICKS(10));
}
 
void LCD1602::lcd_print(const char *str) {
    while (*str) {
        lcd_send_data((uint8_t)(*str));
        str++;
    }
}

void LCD1602::ClearScreen()
{
    lcd_send_cmd(LCD_CMD_CLEAR_DISPLAY);
    vTaskDelay(pdMS_TO_TICKS(10));
}