
#include "adc.h"

#include <avr/interrupt.h>


#define AUDIO_CHANNEL	(1)
#define VBAT_LEVEL		(2)


static volatile u8 adcVal8bit = 0;
static volatile bool adcFlag = false;
static volatile bool adcSempahore = false;


void adc_init()
{	
	// Setting the adc channel to audio output from op-amp. REF is VCC.
	ADMUX |= (1 << MUX0);
	
	// Setting adc clock frequency to 125 kHz, 200 kHz is maximum.
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1); // prescaler to 64
	
	// Eabling the ADC
	ADCSRA |= (1 << ADEN);
}

void adc_start()
{
	ADCSRA |= (1 << ADSC);
}

bool adc_isDone(void)
{
    if (adcFlag)
    {
        adcFlag  = false;
        return true;
    }

    return false;
}

u16 adc_get_val()
{
    u16 adcVal;

    adcSempahore = true;
        adcVal = adcVal8bit;
    adcSempahore = false;
    
    return adcVal;
}

void adc_interrupt_enable()
{
	ADCSRA |= (1 << ADIE);
}

void adc_interrupt_disable()
{
	ADCSRA &= ~(1 << ADIE);
}

void adc_channel_toggle()
{
	if ((ADMUX & 0b00111111) == 1) {
		ADMUX &= ~(1 << MUX0);
		ADMUX |= (1 << MUX1);
	} else if ((ADMUX & 0b00111111) == 2) {
		ADMUX &= ~(1 << MUX1);
		ADMUX |= (1 << MUX0);
	}
}

void adc_channel_audio()
{
	ADMUX &= ~((1 << MUX5) | (1 << MUX4) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
	ADMUX |= (1 << MUX0);
}

void adc_setVbatChannel(void)
{
	ADMUX &= ~(1 << MUX1);
	ADMUX |= (1 << MUX1);
}

ISR(ADC_vect)
{
    if (!adcSempahore) {
        // Throw away the least 2 bits
	    adcVal8bit = (u8) (ADC >> 2);
        adcFlag = true;
    }
	    
    adcVal8bit = (u8) (ADC >> 2);
    adcFlag = true;
    /*
    red_led = adc_val_8bit;
	green_led = 0.5 * adc_val_8bit;
	blue_led = 0.1 * adc_val_8bit;
	
	delay_count = adc_val_8bit;
	*/
	
	
	adc_start();
}
