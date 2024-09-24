#pragma once

#include <stdbool.h>
#include <stdint.h>

// X ms / 8.192
#define TIME_50_MS  (6)
#define TIME_75_MS  (9)
#define TIME_100_MS (12)
#define TIME_150_MS (18)
#define TIME_200_MS (24)
#define TIME_30_MIN (219726UL)

void timer_init(void);
void timer_start(void);
void timer_stop(void);
bool timer_done(void);
uint32_t timerGetUptime(void);
