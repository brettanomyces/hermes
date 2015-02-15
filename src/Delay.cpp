#include "Delay.h"

Delay::Delay(long _period) {
	period=_period;
	lastTime=0;
}

// reset the delay period
void Delay::reset() {
	lastTime=millis();
}

// check if delay period has been exceeded
// true if delay has been exceeded
bool Delay::ok() {
	if (millis()-lastTime > period) {
		return true;
	} else {
		return false;
	}
}
