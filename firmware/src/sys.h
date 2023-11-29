#pragma once

#include "dl_utils.h"

#include <avr/interrupt.h>

u8 sys_enterCritical(void);
void sys_exitCritical(u8 status);
