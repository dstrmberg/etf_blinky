
#include "pattern.h"
#include "scheduler.h"
#include "sys.h"
#include "bb_spi.h"
#include "button.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define BATTERY_STATUS_SHOW_TIME 2500

enum state
{
    ST_IDLE,
    ST_LIGHT_INCREASE,
    ST_LIGHT_DECREASE,
    ST_PATTERN_NEXT,
    ST_PATTERN_PREV,
};


int main(void)
{
    //sys_debugLedOn(true);
    sys_init();

    enum state st = ST_IDLE;

    patternInit();
    patternFunc currentPat = patternNext();

    while (1)
    {
        event_s ev = evRun();

        switch (st)
        {
            case ST_IDLE:
                if (ev.code == EV_BUTTON_PRESSED)
                {
                    if (btnPressed(BUTTON1) && btnPressed(BUTTON2))
                    {
                        sys_batteryCheck();
                        _delay_ms(BATTERY_STATUS_SHOW_TIME);
                    }
                    else
                    {
                        st = (ev.eventData == BUTTON1) ? ST_PATTERN_PREV : ST_PATTERN_NEXT;
                    }
                }
                break;
            case ST_LIGHT_INCREASE:
                if (ev.code == EV_BUTTON_RELEASED)
                {
                    if (ev.eventData == BUTTON1)
                    {
                        st = ST_IDLE;
                    }
                    else
                    {
                        //TODO: increase light
                        sys_debugLedOn(true);
                    }
                }
                break;
            case ST_LIGHT_DECREASE:
                if (ev.code == EV_BUTTON_RELEASED)
                {
                    if (ev.eventData == BUTTON2)
                    {
                        st = ST_IDLE;
                    }
                    else
                    {
                        //TODO: decrease light
                        sys_debugLedOn(false);
                    }
                }
                break;
            case ST_PATTERN_NEXT:
                if (ev.code == EV_BUTTON_RELEASED)
                {
                    if (ev.eventData == BUTTON2) currentPat = patternNext();
                    st = ST_IDLE;
                }
                else if (ev.code == EV_BUTTON_PRESSED)
                {
                    if (ev.eventData == BUTTON1)
                    {
                        // TODO: decrease light
                        st = ST_LIGHT_DECREASE;
                    }
                }
                break;
            case ST_PATTERN_PREV:
                if (ev.code == EV_BUTTON_RELEASED)
                {
                    if (ev.eventData == BUTTON1) currentPat = patternPrevious();
                    st = ST_IDLE;
                }
                else if (ev.code == EV_BUTTON_PRESSED)
                {
                    if (ev.eventData == BUTTON2)
                    {
                        // TODO: increase light
                        st = ST_LIGHT_INCREASE;
                    }
                }
                break;
        }

        if (ev.code == EV_BUTTON_ISR_DISABLED) btnEnableISR(ev.eventData);

        if (btnPressed(BUTTON_PWR))
        {
            if (patternShutdownSequence()) sys_powerOff();
        }

        currentPat();
    }
}
