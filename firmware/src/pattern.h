#pragma once

#include "dl_utils.h"


typedef void (*patternFunc)(void);


void patternInit(void);
bool patternBootSequence(void);
bool patternShutdownSequence(void);
void patternBatteryLevel(uint8_t level);
void clearLeds(void);
patternFunc patternNext(void);
patternFunc patternPrevious(void);
