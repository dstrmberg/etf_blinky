#pragma once

#include "dl_utils.h"

// X ms / 8.192
#define TIME_NOW    (0)
#define TIME_50_MS  (6)
#define TIME_75_MS  (9)
#define TIME_100_MS (12)
#define TIME_150_MS (18)
#define TIME_200_MS (24)
#define TIME_500_MS (61)
#define TIME_1_S    (122)
#define TIME_30_MIN (219726UL)

void timerInit(void);
void timerStart(void);
void timerStop(void);
uint32_t timerGetUptime(void);
