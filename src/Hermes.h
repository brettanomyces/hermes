#ifndef _HERMES_
#define _HERMES_

enum {
	kAcknowledge,
	kError,
	kSetFrSetTemp,
	kSetFzSetTemp,
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
