#pragma once

#include "dl_utils.h"

#include <avr/io.h>

void adc_init();
void adc_start();
bool adc_isDone(void);
uint16_t adc_get_val();
void adc_interrupt_enable();
void adc_interrupt_disable();
void adc_channel_vbat_level();

void adcSetAudioChannel();
void adcSetVbatChannel(void);
