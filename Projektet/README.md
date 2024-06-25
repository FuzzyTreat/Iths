
// Component PIN    GPIO PIN        DEFINE          PULLUP/DOWN

// Led Strip ws2812 SPI
S                   GPIO_NUM_22     RGB_LED_PIN     N/A
V                   3.3V
G                   GND

// Ultrasonic 
TRIG                GPIO_NUM_19     TRIGGER_PIN     N/A
ECHO                GPIO_NUM_18     ECHO_PIN        N/A
VCC                 5V
GND                 GND

// Joystick ADC
GND                 GND
+5V                 5V
VRX                 GPIO_NUM_39     JOYSTICK_X_AXIS_CHANNEL  ( ADC1_CHANNEL_3 )  N/A
VRY                 GPIO_NUM_36     JOYSTICK_Y_AXIS_CHANNEL  ( ADC1_CHANNEL_0 )  N/A
SW                  GPIO_NUM_15     JOYSTICK_BUTTON_PIN                          PULLDOWN SW

// Servo motor PWM
SIGNAL              GPIO_NUM_2      SERVO_PIN       N/A
VCC                 5V
GND                 GND

// I2C Bus used for the Lcd
GND                 GND
VDD                 5V
SDA                 GPIO_NUM_13     I2C_SDA_PIN     PULLUP SW
SCL                 GPIO_NUM_14     I2C_SCL_PIN     PULLUP SW
