#ifndef LEDMANAGER_H
#define LEDMANAGER_H

#include "ws2812.h"

#ifdef __cplusplus
extern "C" {
#endif

class LedManager
{
    private:
    uint16_t brightness;
    uint16_t numberOfLeds;
    uint32_t maxDistance;
    uint32_t rangeStep;

    protected: 

    public:
    LedManager(uint16_t brightness, uint16_t numberOfLeds, uint32_t maxDistance, uint32_t rangeStep);
    ~LedManager();

    void GetLedColor(LedColor_t &led);
    void SetRangeLed(ws2812 *led_strip, uint32_t range);
};


#ifdef __cplusplus
}
#endif
#endif  // LEDMANAGER_H define