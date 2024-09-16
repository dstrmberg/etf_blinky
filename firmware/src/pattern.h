#pragma once

#include "dl_utils.h"
#include <stdbool.h>


typedef void(*patternFunc)(void);


void patternInit(void);
bool patternBootSequence(void);
bool patternShutdownSequence(void);
void patternBatteryLevel(u8 level);
void patternAudioLevel(u16 level);
void clearLeds(void);
patternFunc patternNext(void);
patternFunc patternPrevious(void);

// TODO(noxet): remove later
void staticColorBlue();
