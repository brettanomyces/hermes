#ifndef _HERMES_
#define _HERMES_

int updateInterval = 5000;

enum {
	kAcknowledge,
	kError,
	kSetFrSetTemp,
	kSetFzSetTemp,
	kPlotDataPoint,
	kLogFrTemp,
	kLogFzTemp,
	kLogBaffelState,
	kLogCompressorState,
	kLogFanState,
	kLogHeaterState,
};

void attachCommandCallbacks();
void onUnknownCommand();
void onArduinoReady();
void setFzSetTemp();
void setFrSetTemp();

#endif /* _HERMES_ */
