#include "sys.h"

#include <avr/io.h>

#include "dl_utils.h"
#include "bb_spi.h"
#include "adc.h"
#include "button.h"
#include "pattern.h"

#include "timer.h"
#include "util/delay.h"

#define SYS_PWR_EN_PORT (PORTA)
#define SYS_PWR_EN_PIN  (3)
#define SYS_DBG_LED     (1 << PB1)

void sys_audioCheck(void);

void sys_init(void)
{
    /*
	adc_init();
    */
    DDRB |= SYS_DBG_LED;
    sys_powerOn();
    bb_spi_init();
    button_init();
    patternBootSequence2();
    adc_init();
    adc_channel_audio();
    adc_interrupt_enable();
    timer_init();
    sei();
    while (1) sys_audioCheck();
}

u8 sys_enterCritical(void)
{
    u8 status = SREG;
    cli();
    return status;
}

void sys_exitCritical(u8 status)
{
    SREG = status;
}

void sys_powerOn(void)
{
    DDRA |= (1 << SYS_PWR_EN_PIN);
    SYS_PWR_EN_PORT |= (1 << SYS_PWR_EN_PIN);

}

void sys_powerOff(void)
{
    SYS_PWR_EN_PORT &= ~(1 << SYS_PWR_EN_PIN);
}

void sys_debugLedOn(bool on)
{
    if (on)
    {
        PORTB |= SYS_DBG_LED;
    }
    else
    {
        PORTB &= ~SYS_DBG_LED;
    }
}

void sys_batteryCheck(void)
{
    clearLeds();
    adc_setVbatChannel();
    adc_start();
    while (!adc_isDone());
    u8 level = adc_get_val();
    patternBatteryLevel(level);
}

int xn1 = 0, xn = 0, yn1 = 0, y_n = 0;
uint8_t retain = 0;
int max = 0;

void sys_audioCheck(void)
{
    //clearLeds();
    adc_start();
    while (!adc_isDone());
    u16 level = adc_get_val();
    if (400 > level)
    {
        level = 0;
    }
    else
    {
        level -= 400;
    }
    patternAudioLevel(level >> 1);
    /*
    xn = level;
    y_n = xn - xn1 + 0.99f * yn1;
    if (y_n < 0) y_n = 0;
    xn1 = xn;
    yn1 = y_n;
    if (y_n > max)
    {
        max = y_n;
        patternAudioLevel(max);
        // restart timer to keep the retention of the latest larger value
        timer_stop();
        timer_start();
    }
    */

    if (timer_done()) max = 0;
}
