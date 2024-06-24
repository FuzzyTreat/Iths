#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include <stdint.h>

// Gyroskop.
#include "mpu6050.h"


#include "esp_task_wdt.h"
#include "esp_timer.h"


#define I2C_MASTER_SCL_IO           22    /*!< gpio number for I2C master clock */
//#define I2C_MASTER_SCL_IO           14    /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO           21    /*!< gpio number for I2C master data  */
//#define I2C_MASTER_SDA_IO           13    /*!< gpio number for I2C master data  */
//#define I2C_MASTER_NUM              I2C_NUM_1 /*!< I2C port number for master dev */
#define I2C_MASTER_NUM              I2C_NUM_0 /*!< I2C port number for master dev */
//#define I2C_MASTER_NUM              0 /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ          100000  /* !< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0 /*!< I2C master doesn't need buffer */
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

// Gyroskop
#define MPU6050_I2C_ADDRESS         0x68u



static const char *TAG = "MAIN";


// Gyroskop:
mpu6050_handle_t mpu6050_handle;
mpu6050_raw_acce_value_t raw_acce;
mpu6050_raw_gyro_value_t raw_gyro;
uint8_t deviceid;
 
 
// Function prototypes
static esp_err_t i2c_master_init(void);
/*
static esp_err_t lcd_send_cmd(uint8_t cmd);
static esp_err_t lcd_send_data(uint8_t data);
static void lcd_init(void);
static void lcd_print(const char *str);
*/

// Egna LCD-funktioner.
//void lcd_set_cursor(uint8_t row, uint8_t col);
//void lcd_clear(void);
//static void lcd_show_value(const char *str, int16_t value);
int showIndex = 0;


// Gyro-funktioner
//void gyro_init(mpu6050_handle_t mpu6050_handle);
void gyro_init();

int testHeltal = 23;



void app_main(void)
{

    i2c_config_t conf;
        conf.mode = I2C_MODE_MASTER;
        conf.sda_io_num = I2C_MASTER_SDA_IO;
        conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
        conf.scl_io_num = I2C_MASTER_SCL_IO;
        conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
        conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    
    //conf.clk_flags = 0;

    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    ESP_ERROR_CHECK(
        i2c_driver_install(
            I2C_MASTER_NUM, 
            conf.mode,
            I2C_MASTER_RX_BUF_DISABLE,
            I2C_MASTER_TX_BUF_DISABLE, 
            0
        )
    );

    mpu6050_handle = mpu6050_create(I2C_MASTER_NUM, MPU6050_I2C_ADDRESS);

    mpu6050_config(mpu6050_handle, ACCE_FS_8G, GYRO_FS_500DPS);
    mpu6050_wake_up(mpu6050_handle);
    mpu6050_get_deviceid(mpu6050_handle, &deviceid);
    
    printf("Device-ID %d \n", deviceid);
}