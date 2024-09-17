#include "scheduler.h"

#include "dl_utils.h"

#ifndef DL_MAX_EVENTS
    #warning "DL_MAX_EVENTS not defined, defaults to 5"
    #define DL_MAX_EVENTS 5
#endif


static event_s g_eventQueue[DL_MAX_EVENTS];
static u8 g_eventToRun = 0;
static u8 g_nextFreeEvent = 1;

void dl_schedulerInit(void)
{
    for (int i = 0; i < DL_MAX_EVENTS; i++)
    {
        g_eventQueue[i].code = EV_NOP;
    }
}

bool dl_addEvent(event_s ev)
{
    u8 status = sys_enterCritical();
    if (g_eventQueue[g_nextFreeEvent].code != EV_NOP)
    {
        sys_exitCritical(status);
        return false;
    }

    g_eventQueue[g_nextFreeEvent].code = ev.code;
    g_eventQueue[g_nextFreeEvent].eventData = ev.eventData;
    g_nextFreeEvent = (g_nextFreeEvent + 1) % DL_MAX_EVENTS;

    sys_exitCritical(status);
    return true;
}

event_s dl_run(void)
{
    if (g_eventQueue[g_eventToRun].code == EV_NOP) return (event_s) { EV_NOP, 0 };

    u8 status = sys_enterCritical();
    if (g_eventQueue[g_eventToRun].code == EV_NOP)
    {
        sys_exitCritical(status);
        return (event_s) { EV_NOP, 0 };
    }

    event_s ev = g_eventQueue[g_eventToRun];
    g_eventQueue[g_eventToRun].code = EV_NOP;
    g_eventQueue[g_eventToRun].eventData = 0;
    g_eventToRun = (g_eventToRun + 1) % DL_MAX_EVENTS;

    sys_exitCritical(status);
    
    return ev;
}

