#pragma once

typedef void(*patternFunc)(void);


void patternInit(void);
patternFunc patternNext(void);
patternFunc patternPrevious(void);

// TODO(noxet): remove later
void staticColorBlue();
