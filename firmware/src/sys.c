#include "sys.h"

#include "dl_utils.h"
#include "bb_spi.h"
#include "adc.h"
#include "button.h"
#include "pattern.h"
#include "timer.h"
#include "scheduler.h"

#include <avr/io.h>

#define SYS_PWR_EN_PORT (PORTA)
#define SYS_PWR_EN_PIN  (3)
#define SYS_DBG_LED     (1 << PB1)

void sys_audioCheck(void);

void sys_init(void)
{
    DDRB |= SYS_DBG_LED;
    btnInit();
    sys_powerOn();
    evInit();
    bb_spi_init();
    if (!patternBootSequence()) sys_powerOff();

    // don't fully start the system until user releases the pwr button
    while (btnPwrPressed())
        ;

    adc_init();
    adc_channel_audio();
    adc_interrupt_enable();
    timer_init();
    timer_start();
    sei();
    // while (1) sys_audioCheck();
}

uint8_t sys_enterCritical(void)
{
    uint8_t status = SREG;
    cli();
    return status;
}

void sys_exitCritical(uint8_t status)
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
    // wait until the pwr button is released, otherwise we can not turn the device off
    while (btnPwrPressed())
        ;
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
    while (!adc_isDone())
        ;
    uint8_t level = adc_get_val();
    patternBatteryLevel(level);
}

int xn1 = 0, xn = 0, yn1 = 0, y_n = 0;
uint8_t retain = 0;
int max = 0;

void sys_audioCheck(void)
{
    // clearLeds();
    uint16_t dcoff = 400; // 350;
    adc_start();
    while (!adc_isDone())
        ;
    uint16_t level = adc_get_val();
    if (dcoff > level)
    {
        level = 0;
    }
    else
    {
        level -= dcoff;
    }
    if (level >= max)
    {
        max = level;
        patternAudioLevel(max);
        timer_stop();
        timer_start();
    }
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
