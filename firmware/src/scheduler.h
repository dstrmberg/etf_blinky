#pragma once

#include "sys.h"

typedef void (*funcPtr)(u16);

typedef struct
{
    funcPtr eventFunc;
    u16 eventData;
} event_s;

void dl_schedulerInit();
bool dl_addEvent(event_s ev);
void dl_run();

