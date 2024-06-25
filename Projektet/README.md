// Led Strip ws2812 SPI
#define RGB_LED_PIN             GPIO_NUM_22     

// Ultrasonic 
#define TRIGGER_PIN             GPIO_NUM_19           
#define ECHO_PIN                GPIO_NUM_18     

// Joystick ADC
#define JOYSTICK_BUTTON_PIN     GPIO_NUM_15                     PULLDOWN SW
#define JOYSTICK_X_AXIS_CHANNEL GPIO_NUM_39 ( ADC1_CHANNEL_3 )
#define JOYSTICK_Y_AXIS_CHANNEL GPIO_NUM_36 ( ADC1_CHANNEL_0 )

// Servo motor PWM
#define SERVO_PIN               GPIO_NUM_2      
#define SERVO_LEDC_CHANNEL      LEDC_CHANNEL_0

// I2C Bus used for the Lcd
#define I2C_SDA_PIN             GPIO_NUM_13                     PULLUP SW
#define I2C_SCL_PIN             GPIO_NUM_14                     PULLUP SW
