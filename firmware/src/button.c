#include "button.h"
#include "scheduler.h"
#include "timer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

// PCINT7 and PCINT10
#define BTN1_PCINT 2
#define BTN2_PCINT 7

#define BTN_PWR 0
#define BTN1    2
#define BTN2    7

#define BTN1_INTERRUPT PCINT1_vect
#define BTN2_INTERRUPT PCINT0_vect


static volatile uint8_t btn1Prev = 0;
static volatile uint8_t btn2Prev = 0;


void btnInit()
{
    GIMSK |= (1 << PCIE1) | (1 << PCIE0);
    btnEnableISR(BUTTON1);
    btnEnableISR(BUTTON2);

    DDRB &= ~(1 << BTN1);
    DDRA &= ~(1 << BTN2);
    DDRB &= ~(1 << BTN_PWR);
}


// TODO(noxet): consider replacing this with 3 macros instead
inline void btnEnableISR(enum button btn)
{
    switch (btn)
    {
        case BUTTON1:
            PCMSK1 |= (1 << BTN1_PCINT);
            break;
        case BUTTON2:
            PCMSK0 |= (1 << BTN2_PCINT);
            break;
        default:
            break;
    }
}


// TODO(noxet): consider replacing this with 3 macros instead
inline void btnDisableISR(enum button btn)
{
    switch (btn)
    {
        case BUTTON1:
            PCMSK1 &= ~(1 << BTN1_PCINT);
            break;
        case BUTTON2:
            PCMSK0 &= ~(1 << BTN2_PCINT);
            break;
        default:
            break;
    }
}


// TODO(noxet): consider replacing this with 3 macros instead
bool btnPressed(enum button btn)
{
    switch (btn)
    {
        case BUTTON1:
            return (PINB & (1 << BTN1)) ? true : false;
        case BUTTON2:
            return (PINA & (1 << BTN2)) ? true : false;
        case BUTTON_PWR:
            return (PINB & (1 << BTN_PWR)) ? true : false;
    }
}


ISR(BTN1_INTERRUPT)
{
    uint8_t btn1Curr = (PINB & (1 << BTN1)) >> BTN1;

    event_s ev = NEW_EVENT();
    if (btn1Curr > btn1Prev)
    {
        ev.code = EV_BUTTON_PRESSED;
        ev.eventData = BUTTON1;
        // add small delay to be able to detect if user pressed both buttons at the same time
        evAdd(ev, TIME_50_MS);
    }
    else if (btn1Curr < btn1Prev)
    {
        ev.code = EV_BUTTON_RELEASED;
        ev.eventData = BUTTON1;
        // add delay here too, since we don't want to register release event before pressed
        evAdd(ev, TIME_50_MS);
    }

    btn1Prev = btn1Curr;

    // NOTE(noxet): There is a possible HW bug where we get multiple interrupt on BTN2.
    // possibly due to slow decay from debounce filter together with SPI being routed too close.
    btnDisableISR(BUTTON2);
    ev.code = EV_BUTTON_ISR_DISABLED;
    ev.eventData = BUTTON2;
    evAdd(ev, TIME_50_MS);
}


ISR(BTN2_INTERRUPT)
{
    uint8_t btn2Curr = (PINA & (1 << BTN2)) >> BTN2;

    event_s ev = NEW_EVENT();
    if (btn2Curr > btn2Prev)
    {
        ev.code = EV_BUTTON_PRESSED;
        ev.eventData = BUTTON2;
        // add small delay to be able to detect if user pressed both buttons at the same time
        evAdd(ev, TIME_50_MS);
    }
    else if (btn2Curr < btn2Prev)
    {
        ev.code = EV_BUTTON_RELEASED;
        ev.eventData = BUTTON2;
        // add delay here too, since we don't want to register release event before pressed
        evAdd(ev, TIME_50_MS);
    }

    btn2Prev = btn2Curr;

    // NOTE(noxet): There is a possible HW bug where we get multiple interrupt on BTN2.
    // possibly due to slow decay from debounce filter together with SPI being routed too close.
    btnDisableISR(BUTTON2);
    ev.code = EV_BUTTON_ISR_DISABLED;
    ev.eventData = BUTTON2;
    evAdd(ev, TIME_50_MS);
}
