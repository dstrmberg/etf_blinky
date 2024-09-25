#include "button.h"
#include "scheduler.h"
#include "sys.h"
#include "timer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

// PCINT7 and PCINT10
#define BTN1_PCINT	2
#define BTN2_PCINT	7

#define BTN_PWR 0
#define BTN1	2
#define BTN2	7

#define BTN1_INTERRUPT	PCINT1_vect
#define BTN2_INTERRUPT	PCINT0_vect


static volatile uint8_t btn1Prev = 0;
static volatile uint8_t btn2Prev = 0;

static volatile uint8_t btn1Pressed = 0, btn1Released = 0, btn2Pressed = 0, btn2Released = 0;


void btnInit()
{		
	GIMSK |= (1 << PCIE1) | (1 << PCIE0);
	PCMSK0 |= (1 << BTN2_PCINT); // PCINT7
	PCMSK1 |= (1 << BTN1_PCINT); // PCINT10
	
	DDRB &= ~(1 << BTN1);
	DDRA &= ~(1 << BTN2);
	DDRB &= ~(1 << BTN_PWR);
}


void btnPressed(u16 data)
{
    if (data == 1)
    {
        sys_debugLedOn(true);
    }
    else
    {
        sys_debugLedOn(false);
    }
}


bool btnPwrPressed(void)
{
    return (PINB & (1 << BTN_PWR)) ? true : false;
}


ISR(BTN1_INTERRUPT)
{
    //if (!(PINB & (1 << PB2))) return;
    uint8_t btn1Curr = (PINB & (1 << BTN1)) >> BTN1;

    event_s ev = NEW_EVENT();
	if (btn1Curr > btn1Prev)
    {
        btn1Pressed++;
        ev.code = EV_BUTTON_PRESSED;
        ev.eventData = BUTTON1;
        evAdd(ev);
	}
    else if (btn1Curr < btn1Prev)
    {
        btn1Released++;
        ev.code = EV_BUTTON_RELEASED;
        ev.eventData = BUTTON1;
        evSchedule(ev, TIME_1_s, false);
        //evAdd(ev);
    }

	btn1Prev = btn1Curr;
}


ISR(BTN2_INTERRUPT)
{
    //if (!(PINA & (1 << PA7))) return;
    uint8_t btn2Curr = (PINA & (1 << BTN2)) >> BTN2;

    event_s ev = NEW_EVENT();
    if (btn2Curr > btn2Prev)
    {
        btn2Pressed++;
        ev.code = EV_BUTTON_PRESSED;
        ev.eventData = BUTTON2;
        evAdd(ev);
    }
    else if (btn2Curr < btn2Prev)
    {
        btn2Released++;
        ev.code = EV_BUTTON_RELEASED;
        ev.eventData = BUTTON2;
        evAdd(ev);
    }

    btn2Prev = btn2Curr;

    PCMSK0 &= ~(1 << BTN2_PCINT);
    ev.code = EV_BUTTON_ISR_DISABLED;
    ev.eventData = BUTTON2;
    //ev.timeToRun = TIME_100_MS;
    evSchedule(ev, TIME_50_MS, false);
}

