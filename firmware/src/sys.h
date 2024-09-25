#pragma once

#include "dl_utils.h"


void sys_init(void);
void sys_powerOn(void);
void sys_powerOff(void);
void sys_debugLedOn(bool on);
void sys_batteryCheck(void);
uint8_t sys_enterCritical(void);
void sys_exitCritical(uint8_t status);
