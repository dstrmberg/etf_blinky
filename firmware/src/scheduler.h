#pragma once

#include "sys.h"

#include <stdbool.h>


enum eventCode
{
    EV_NOP,
    EV_BUTTON_PRESSED,
    EV_BUTTON_RELEASED,
    
};

typedef struct
{
    enum eventCode code;
    u16 eventData;
} event_s;


void dl_schedulerInit(void);
bool dl_addEvent(event_s ev);
event_s dl_run(void);

