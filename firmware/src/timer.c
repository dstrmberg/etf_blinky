
#include "timer.h"
#include "pattern.h"
#include "sys.h"

#include "avr/io.h"
#include "avr/interrupt.h"

#define TIME_50_MS  (200)
#define TIME_75_MS  (300)
#define TIME_100_MS (400)
#define TIME_150_MS (600)
#define TIME_200_MS (800)

static uint16_t cnt = 0;
static bool done = false;

void timer_init(void)
{
    TIMSK0 |= (1 << TOIE0);
}

void timer_start(void)
{
    TCCR0B |= (1 << CS01); // presc 8
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

ISR(TIM0_OVF_vect)
{
    cnt++;
    if (cnt >= TIME_75_MS)
    {
        done = true;
        clearLeds();
        //sys_debugLedOn(false);
        timer_stop();
    }
}
