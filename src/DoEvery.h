#ifndef DoEvery_h
#define DoEvery_h
#define LIBRARY_VERSION	1.0.0

#include <inttypes.h>
#include "IDeviceManager.h"

class DoEvery
{
public:
	DoEvery(long, IDeviceManager*);
	void reset();
	bool check();
	bool before(double);
private:
	unsigned long period;
	unsigned long lastTime;
        IDeviceManager* m_dm;
};

#endif
