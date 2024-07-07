#pragma once

#include "dl_utils.h"
#include <stdbool.h>


typedef void(*patternFunc)(void);


void patternInit(void);
bool patternBootSequence(void);
bool patternBootSequence2(void);
void patternBatteryLevel(u8 level);
void clearLeds(void);
patternFunc patternNext(void);
patternFunc patternPrevious(void);

// TODO(noxet): remove later
void staticColorBlue();
