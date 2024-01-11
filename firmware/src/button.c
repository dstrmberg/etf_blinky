#include "button.h"

// PCINT7 and PCINT10
#define BTN2_PCINT	7
#define BTN1_PCINT	2

#define BTN1	2
#define BTN2	7

#define BTN1_INTERRUPT	PCINT1_vect
#define BTN2_INTERRUPT	PCINT0_vect


static volatile uint8_t btn1_pressed, btn1_prev, btn1_crnt, btn2_pressed, btn2_prev, btn2_crnt;

void button_init(){
	
	
	GIMSK |= (1<<PCIE1);
	PCMSK0 |= (1<<BTN1_PCINT);
	PCMSK1 |= (1<<BTN2_PCINT);
	
	DDRB &= ~(1<<BTN1);
	DDRA &= ~(1<<BTN2);
	
}


ISR(BTN1_INTERRUPT)
{
    btn1_crnt = (PINB & (1<<BTN1)) >> BTN1;
	
	if (btn1_crnt > btn1_prev) {
		btn1_pressed = 1;
	}
	
	btn1_prev = btn1_crnt;
}

ISR(BTN2_INTERRUPT)
{
    btn2_crnt = (PINA & (1<<BTN2)) >> BTN2;
    
    if (btn2_crnt > btn2_prev) {
	    btn2_pressed = 1;
    }
    
    btn2_prev = btn2_crnt;
}

