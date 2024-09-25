
#include "pattern.h"
#include "scheduler.h"
#include "sys.h"
#include "bb_spi.h"
#include "adc.h"
#include "button.h"
#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


int main(void)
{
    sys_debugLedOn(true);
    sys_init();

    patternInit();

    patternFunc currentPat = patternNext();

    while(1)
    {

        event_s ev = evRun();
        switch (ev.code)
        {
            case EV_NOP:
                break;
            case EV_BUTTON_PRESSED:
                if (ev.eventData == BUTTON1) currentPat = patternPrevious();
                else if (ev.eventData == BUTTON2) currentPat = patternNext();
                break;
            case EV_BUTTON_RELEASED:
                if (ev.eventData == BUTTON1) sys_debugLedOn(true);
                else if (ev.eventData == BUTTON2) sys_debugLedOn(false);
                break;
            case EV_BUTTON_ISR_DISABLED:
                PCMSK0 |= (1 << 7);
                break;
        }


        if (btnPwrPressed())
        {
            if (patternShutdownSequence()) sys_powerOff();
        }

        currentPat();
    }
}


