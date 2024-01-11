#include "scheduler.h"

#include "dl_utils.h"

#ifndef DL_MAX_EVENTS
    #warning "DL_MAX_EVENTS not defined, defaults to 5"
    #define DL_MAX_EVENTS 5
#endif


static event_s g_eventQueue[DL_MAX_EVENTS];
static u8 g_eventToRun = 0;
static u8 g_nextFreeEvent = 1;

void dl_schedulerInit()
{
    for (int i = 0; i < DL_MAX_EVENTS; i++)
    {
        //g_eventQueue[i].eventFunc = NULL;
    }
}

bool dl_addEvent(event_s ev)
{
    u8 status = sys_enterCritical();
    for (int i = 0; i < DL_MAX_EVENTS; i++)
    {
        if (g_eventQueue[i].eventFunc == NULL)
        {
            g_eventQueue[i].eventFunc = ev.eventFunc;
            sys_exitCritical(status);
            return true;
        }
    }

    sys_exitCritical(status);
    return false;
}

void dl_run()
{
    if (g_eventQueue[g_eventToRun].eventFunc == NULL) return;

    u8 status = sys_enterCritical();
    if (g_eventQueue[g_eventToRun].eventFunc == NULL)
    {
        sys_exitCritical(status);
        return;
    }

    event_s ev = g_eventQueue[g_eventToRun];
    g_eventQueue[g_eventToRun].eventFunc = NULL;
    g_eventToRun = (g_eventToRun + 1) % DL_MAX_EVENTS;

    sys_exitCritical(status);

    ev.eventFunc(ev.eventData);

}

