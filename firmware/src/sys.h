#pragma once

#include "dl_utils.h"

#include <avr/interrupt.h>

void sys_init(void);
void sys_powerOn(void);
void sys_powerOff(void);
void sys_batteryCheck(void);
u8 sys_enterCritical(void);
void sys_exitCritical(u8 status);
