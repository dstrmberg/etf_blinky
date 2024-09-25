#include "button.h"
#include "scheduler.h"
#include "sys.h"
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


void btnPressed(uint16_t data)
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
    uint8_t btn1Curr = (PINB & (1 << BTN1)) >> BTN1;

    event_s ev = NEW_EVENT();
    if (btn1Curr > btn1Prev)
    {
        btn1Pressed++;
        ev.code = EV_BUTTON_PRESSED;
        ev.eventData = BUTTON1;
        evAdd(ev, TIME_NOW);
    }
    else if (btn1Curr < btn1Prev)
    {
        btn1Released++;
        ev.code = EV_BUTTON_RELEASED;
        ev.eventData = BUTTON1;
        evAdd(ev, TIME_1_s);
    }

    btn1Prev = btn1Curr;
}


ISR(BTN2_INTERRUPT)
{
    uint8_t btn2Curr = (PINA & (1 << BTN2)) >> BTN2;

    event_s ev = NEW_EVENT();
    if (btn2Curr > btn2Prev)
    {
        btn2Pressed++;
        ev.code = EV_BUTTON_PRESSED;
        ev.eventData = BUTTON2;
        evAdd(ev, TIME_NOW);
    }
    else if (btn2Curr < btn2Prev)
    {
        btn2Released++;
        ev.code = EV_BUTTON_RELEASED;
        ev.eventData = BUTTON2;
        evAdd(ev, TIME_NOW);
    }

    btn2Prev = btn2Curr;

    // TODO(noxet): There is a possible HW bug where we get multiple interrupt on BTN2.
    // possibly due to slow decay from debounce filter together with SPI being routed too close.
    PCMSK0 &= ~(1 << BTN2_PCINT);
    ev.code = EV_BUTTON_ISR_DISABLED;
    ev.eventData = BUTTON2;
    evAdd(ev, TIME_50_MS);
}
