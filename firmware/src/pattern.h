#pragma once

typedef void(*patternFunc)(void);


void patternInit(void);
patternFunc patternNext(void);
patternFunc patternPrevious(void);
