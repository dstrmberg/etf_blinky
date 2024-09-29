#include "button.h"
#include "scheduler.h"
#include "timer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>


#define BTN1_INTERRUPT PCINT1_vect
#define BTN2_INTERRUPT PCINT0_vect


static volatile uint8_t btn1Prev = 0;
static volatile uint8_t btn2Prev = 0;


void btnInit()
{
    GIMSK |= (1 << PCIE1) | (1 << PCIE0);
    //btnEnableISR(BUTTON1);
    //btnEnableISR(BUTTON2);
    BTN1_ENABLE_ISR();
    BTN2_ENABLE_ISR();

    DDRB &= ~(1 << BTN1);
    DDRA &= ~(1 << BTN2);
    DDRB &= ~(1 << BTN_PWR);
}


void btnEnableISR(enum button btn)
{
    if (btn == BUTTON1) BTN1_ENABLE_ISR();
    else BTN2_ENABLE_ISR();
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
    //    btnDisableISR(BUTTON2);
    BTN1_DISABLE_ISR();
    ev.code = EV_BUTTON_ISR_DISABLED;
    ev.eventData = BUTTON1;
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
    // btnDisableISR(BUTTON2);
    BTN2_DISABLE_ISR();
    ev.code = EV_BUTTON_ISR_DISABLED;
    ev.eventData = BUTTON2;
    evAdd(ev, TIME_50_MS);
}
