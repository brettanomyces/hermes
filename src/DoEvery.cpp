/**********************************************************************************************
 *
 * This Code is licensed under a Creative Commons Attribution-ShareAlike 3.0 License.
 **********************************************************************************************/

#include "DoEvery.h"

DoEvery::DoEvery(long _period, IDeviceManager* deviceManager) {
	period=_period;
        m_dm = deviceManager;
	lastTime=0;
}

void DoEvery::reset() {
	lastTime=m_dm->getMillis();
}

bool DoEvery::check() {
	if (m_dm->getMillis()-lastTime > period) {
		lastTime+=period;
		return true;
	} else {
		return false;
	}
}

bool DoEvery::before(double threshTime) {
	if (m_dm->getMillis()-lastTime < threshTime) {
		return true;
	} else {
		return false;
	}
}
