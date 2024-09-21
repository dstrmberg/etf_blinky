#pragma once

#include <stdbool.h>
#include <stdint.h>


enum eventCode
{
    EV_NOP,
    EV_BUTTON_PRESSED,
    EV_BUTTON_RELEASED,
};

#define NEW_EVENT() { EV_NOP, 0 }


typedef struct
{
    enum eventCode code;
    uint16_t eventData;
} event_s;


void evInit(void);
bool evAdd(event_s ev);
event_s evRun(void);

