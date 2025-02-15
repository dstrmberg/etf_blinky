
#include "adc.h"

#include <avr/interrupt.h>


#define AUDIO_CHANNEL (1)
#define VBAT_LEVEL    (2)


static volatile uint16_t adcVal = 0;
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
        adcFlag = false;
        return true;
    }

    return false;
}

uint16_t adc_get_val()
{
    uint16_t adc;

    adcSempahore = true;
    adc = adcVal;
    adcSempahore = false;

    return adc;
}

void adc_interrupt_enable()
{
    ADCSRA |= (1 << ADIE);
}

void adc_interrupt_disable()
{
    ADCSRA &= ~(1 << ADIE);
}


void adcSetAudioChannel()
{
    ADMUX &= ~((1 << MUX5) | (1 << MUX4) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
    ADMUX |= (1 << MUX0);
}

void adcSetVbatChannel(void)
{
    ADMUX &= ~(1 << MUX0);
    ADMUX |= (1 << MUX1);
}

ISR(ADC_vect)
{
    if (!adcSempahore)
    {
        adcVal = ADC;
        adcFlag = true;
    }
    /*
    red_led = adc_val_8bit;
    green_led = 0.5 * adc_val_8bit;
    blue_led = 0.1 * adc_val_8bit;

    delay_count = adc_val_8bit;
    */


    // adc_start();
}
