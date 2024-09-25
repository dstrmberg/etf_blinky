#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

static volatile uint16_t cnt = 0;
static volatile uint32_t uptime = 0;


void timerInit(void)
{
    TIMSK0 |= (1 << TOIE0);
}


void timerStart(void)
{
    TCCR0B |= (1 << CS02); // presc 256
}


void timerStop(void)
{
    TCCR0B &= ~(1 << CS00) & ~(1 << CS01) & (1 << CS02);
    cnt = 0;
}


uint32_t timerGetUptime(void)
{
    uint32_t ut = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ut = uptime;
    }

    return ut;
}


ISR(TIM0_OVF_vect)
{
    // each increment is 8.192 ms
    uptime++;
}
