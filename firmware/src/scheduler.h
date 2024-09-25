#pragma once

#include <stdbool.h>
#include <stdint.h>


enum eventCode
{
    EV_NOP,
    EV_BUTTON_PRESSED,
    EV_BUTTON_RELEASED,
    EV_BUTTON_ISR_DISABLED,
};

#define NEW_EVENT() { EV_NOP, 0, 0 }


typedef struct
{
    enum eventCode code;
    uint16_t eventData;
    uint32_t timeToRun;
} event_s;


void evInit(void);
bool evAdd(event_s ev);
bool evSchedule(event_s ev, uint32_t delay, bool reSchedule);
event_s evRun(void);

