#include <Arduino.h>
#include "Rotary.h"

Rotary::Rotary(uint8_t _DT, uint8_t _CLK) {
    DT = _DT;
    CLK = _CLK;
    oldA = HIGH;
    oldB = HIGH;
}

int Rotary::getValue() {
    int8_t result = 0;
	int newA = digitalRead(DT);
	int newB = digitalRead(CLK);
	if (newA != oldA || newB != oldB)
	{
		if (oldA == HIGH && newA == LOW)
		{
			result = (oldB * 2 - 1);
		}
	}
	oldA = newA;
	oldB = newB;
	return result;
}