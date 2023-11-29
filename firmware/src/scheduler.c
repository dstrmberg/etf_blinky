#include "scheduler.h"

#include "dl_utils.h"
#include "sys.h"

#define DL_MAX_EVENTS 5

typedef void (*funcPtr)(void);

typedef struct
{
    funcPtr eventFunc;
} event_s;

static event_s g_eventQueue[DL_MAX_EVENTS];
static u8 g_eventToRun = 0;
static u8 g_nextFreeEvent = 1;

void dl_schedulerInit()
{
    for (int i = 0; i < DL_MAX_EVENTS; i++)
    {
        g_eventQueue[i].eventFunc = NULL;
    }
}

bool dl_addEvent(funcPtr func)
{
    u8 status = sys_enterCritical();
    for (int i = 0; i < DL_MAX_EVENTS; i++)
    {
        if (g_eventQueue[i].eventFunc == NULL)
        {
            g_eventQueue[i].eventFunc = func;
            sys_exitCritical(status);
            return true;
        }
    }

    sys_exitCritical(status);
    return false;
}

funcPtr getEvent()
{
    u8 status = sys_enterCritical();
    if (g_eventQueue[g_eventToRun].eventFunc == NULL)
    {
        sys_exitCritical(status);
        return NULL;
    }

    funcPtr func = g_eventQueue[g_eventToRun].eventFunc;
    g_eventToRun = (g_eventToRun + 1) % DL_MAX_EVENTS;

    sys_exitCritical(status);

    return func;
}

