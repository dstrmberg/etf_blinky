
#include "timer.h"
#include "pattern.h"
#include "sys.h"
#include "scheduler.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

static volatile uint16_t cnt = 0;
static volatile bool done = false;
static volatile uint32_t uptime = 0;

void timer_init(void)
{
    TIMSK0 |= (1 << TOIE0);
}

void timer_start(void)
{
    TCCR0B |= (1 << CS02); // presc 256
}

void timer_stop(void)
{
    TCCR0B &= ~(1 << CS01);
    cnt = 0;
}

bool timer_done(void)
{
    if (done)
    {
        done = false;
        return true;
    }
    return false;
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
