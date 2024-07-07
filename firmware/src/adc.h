#pragma once

#include "dl_utils.h"

#include <avr/io.h>

void adc_init();
void adc_start();
bool adc_isDone(void);
u16 adc_get_val();
void adc_interrupt_enable();
void adc_interrupt_disable();
void adc_channel_toggle();
void adc_channel_audio();
void adc_channel_vbat_level();

void adc_setVbatChannel(void);
