
#include "pattern.h"
#include "scheduler.h"
#include "sys.h"
#include "bb_spi.h"
#include "button.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


int main(void)
{
    sys_debugLedOn(true);
    sys_init();

    patternInit();

    patternFunc currentPat = patternNext();

    while (1)
    {

        event_s ev = evRun();
        switch (ev.code)
        {
            case EV_NOP:
                break;
            case EV_BUTTON_PRESSED:
                if (btnPressed(BUTTON1) && btnPressed(BUTTON2))
                {
                    sys_batteryCheck();
                    _delay_ms(1000);
                }
                else if (ev.eventData == BUTTON1)
                {
                    currentPat = patternPrevious();
                }
                else if (ev.eventData == BUTTON2)
                {
                    currentPat = patternNext();
                }
                break;
            case EV_BUTTON_RELEASED:
                if (ev.eventData == BUTTON1)
                {
                    sys_debugLedOn(true);
                }
                else if (ev.eventData == BUTTON2)
                {
                    sys_debugLedOn(false);
                }
                break;
            case EV_BUTTON_ISR_DISABLED:
                btnEnableISR(ev.eventData);
                break;
        }


        if (btnPressed(BUTTON_PWR))
        {
            if (patternShutdownSequence()) sys_powerOff();
        }

        currentPat();
    }
}
