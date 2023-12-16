#pragma once

#include "sys.h"

typedef void (*funcPtr)(void);

typedef struct
{
    funcPtr eventFunc;
} event_s;

void dl_schedulerInit();
bool dl_addEvent(event_s ev);
void dl_run();

