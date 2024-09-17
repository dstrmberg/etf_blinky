
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


    dl_schedulerInit();

    patternInit();

    patternFunc currentPat = patternNext();

    while(1)
    {
        
        event_s ev = dl_run();
        switch (ev.code)
        {
            case EV_NOP:
                break;
            case EV_BUTTON_PRESSED:
                if (ev.eventData == 1) currentPat = patternNext();
                else if (ev.eventData == 2) currentPat = patternPrevious();
                _delay_ms(300);
                break;
            case EV_BUTTON_RELEASED:
                break;
        }
        /*
        if (PINA & (1 << PA7))
        {
            currentPat = patternNext();
            _delay_ms(300);
            //sys_debugLedOn(true);
            //timer_start();
        }
        else if (PINB & (1 << PB2))
        {
            currentPat = patternPrevious();
            _delay_ms(300);
            //PORTB &= ~MINILED;
        }

        if (btnPwrPressed())
        {
            if (patternShutdownSequence()) sys_powerOff();
        }
        */

        //led_send();

        currentPat();
    }
}


