#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>
#include <unistd.h>


static pthread_mutex_t g_mut;
static atomic_int run = 1;

#define MAINTAG "[MAIN]: "
#define ISRTAG  "\t\t\t[ISR]: "


#include "scheduler.h"

void sys_enterCritical(void)
{
    pthread_mutex_lock(&g_mut);
}


void sys_exitCritical(void)
{
    pthread_mutex_unlock(&g_mut);
}


void* mainThread(void *arg)
{
    for (int i = 0; i < 20; i++)
    {
        event_s ev = evRun();
        switch (ev.code)
        {
            case 1:
                printf(MAINTAG "Event 1: %d\n", ev.eventData);
            break;
            case 2:
                printf(MAINTAG "Event 2: %d\n", ev.eventData);
            break;
            case 3:
                printf(MAINTAG "Event 3: %d\n", ev.eventData);
            break;
        }

        sleep(1);
    }

    run = 0;
}


void* isrThread(void *arg)
{
    static int nice = 69;
    static int chill = 420;
    while (run)
    {
        event_s ev = NEW_EVENT();
        ev.code = 1;
        ev.eventData = nice++;
        printf(ISRTAG "Add event %d: %d\n", ev.code, ev.eventData);
        bool ret = evAdd(ev);
        if (!ret) printf(ISRTAG "Event handler full!\n");

        usleep(600000);

        ev.code = 2;
        ev.eventData = chill++;
        printf(ISRTAG "Add event %d: %d\n", ev.code, ev.eventData);
        ret = evAdd(ev);
        if (!ret) printf(ISRTAG "Event handler full!\n");

        usleep(600000);
    }
}


int main(void)
{
    pthread_mutex_init(&g_mut, NULL);

    pthread_t tMain;
    pthread_t tISR;

    evInit();

    printf("Creating threads...\n");
    pthread_create(&tMain, NULL, mainThread, NULL);
    pthread_create(&tISR, NULL, isrThread, NULL);

    printf("Waiting for threads to finish...\n");
    pthread_join(tMain, NULL);
    pthread_join(tISR, NULL);
}
