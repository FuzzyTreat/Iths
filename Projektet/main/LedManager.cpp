#include <LedManager.h>


LedManager::LedManager(uint16_t brightness, uint16_t numberOfLeds, uint32_t maxDistance, uint32_t rangeStep):brightness(brightness),
numberOfLeds(numberOfLeds), maxDistance(maxDistance), rangeStep(rangeStep)
{

}
LedManager::~LedManager(){};

void LedManager::SetRangeLed(ws2812 *led_strip, uint32_t range)
{
    led_strip->ResetLeds();

    if(range > maxDistance)
    {
        return;    
    }

    uint32_t step = rangeStep;
    uint16_t ledNum = 0;

    for(int i = 0; i < 160; i++)
    {
        if(i % 10 == 0)
        {
            ledNum++;
        }
        else
        {
            if(ledNum == 0)
            {
                led_strip->leds[ledNum].isOn = true;
                led_strip->leds[ledNum].scaleFactor += 1;
                GetLedColor(led_strip->leds[ledNum]);
                led_strip->SetLedColor(led_strip->leds[ledNum]);  
            }
            else if(step <= range)
            {
                led_strip->leds[ledNum].isOn = true;
                led_strip->leds[ledNum].scaleFactor += 1;
                GetLedColor(led_strip->leds[ledNum]);
                led_strip->SetLedColor(led_strip->leds[ledNum]);  
            }
            else
            {
                led_strip->leds[ledNum].isOn = false;
                led_strip->leds[ledNum].scaleFactor = 0;
                led_strip->leds[ledNum].R = 0;
                led_strip->leds[ledNum].G = 0;
                led_strip->leds[ledNum].B = 0;
                led_strip->SetLedColor(led_strip->leds[ledNum]);  
            }

            step = step + rangeStep; 
        }
    }
}

// RED 255,0,0
// GREEN 0,255,0
// BLUE 0,0,255

// GREEN 0,255,0
// YELLOW 255,255,0
// ORANGE 255,128,0
// RED 255,0,0

/// @brief 0 - 255 set color register and brightness, so 0 is unlit, 255 is very bright
/// Reduce the brightness of all 3 colors by the same factor
/// @param index 
/// @param led 
void LedManager::GetLedColor(LedColor_t &led)
{
    switch(led.Index)
    {
        case 0:
        case 1:
        {
            led.R = brightness;
            led.G = 0;
            led.B = 0;
            led.isOn = true;
            break;
        }
        case 2:
        case 3:
        {
            led.R = brightness;
            led.G = brightness/2;
            led.B = 0;
            led.isOn = true;
            break;
        }
        case 4:
        case 5:
        {
            led.R = brightness;
            led.G = brightness;
            led.B = 0;
            led.isOn = true;
            break;
        }
        case 6:
        case 7:
        {
            led.R = 0;
            led.G = brightness;
            led.B = 0;
            led.isOn = true;
            break;
        }
        default:
        {
            led.R = 0;
            led.G = 0;
            led.B = 0;
            led.isOn = false;
            break;
        }
    }
}