#pragma once

#include <stdbool.h>

enum button
{
    BUTTON1 = 1,
    BUTTON2,
    BUTTON_PWR,
};


// PCINT7 and PCINT10
#define BTN1_PCINT 2
#define BTN2_PCINT 7

#define BTN_PWR 0
#define BTN1    2
#define BTN2    7


/* Function macros to save flash by avoiding functions */
#define BTN1_IS_PRESSED()  (PINB & (1 << BTN1))
#define BTN1_ENABLE_ISR()  (PCMSK1 |= (1 << BTN1_PCINT))
#define BTN1_DISABLE_ISR() (PCMSK1 &= ~(1 << BTN1_PCINT))

#define BTN2_IS_PRESSED()  (PINA & (1 << BTN2))
#define BTN2_ENABLE_ISR()  (PCMSK0 |= (1 << BTN2_PCINT))
#define BTN2_DISABLE_ISR() (PCMSK0 &= ~(1 << BTN2_PCINT))

#define BTNPWR_IS_PRESSED() (PINB & (1 << BTN_PWR))


void btnInit();
bool btnPressed(enum button btn);
//void btnDisableISR(enum button btn);
void btnEnableISR(enum button btn);
