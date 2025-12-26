#ifndef ROTARY_H
#define ROTARY_H

#include <Arduino.h>

class Rotary {
private:
    int oldA;
    int oldB;
    uint8_t DT;
    uint8_t CLK;
public:
     Rotary(uint8_t, uint8_t);
     int getValue();
};

#endif