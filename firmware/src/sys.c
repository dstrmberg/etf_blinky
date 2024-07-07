#include "sys.h"

#include <avr/io.h>

#include "dl_utils.h"
#include "bb_spi.h"
#include "adc.h"
#include "button.h"
#include "pattern.h"

#define SYS_PWR_EN_PORT (PORTA)
#define SYS_PWR_EN_PIN  (3)

void sys_init(void)
{
    /*
	adc_init();
	adc_interrupt_enable();
    */
    sys_powerOn();
	bb_spi_init();
	button_init();
    patternBootSequence2();
    adc_init();
    while (1) sys_batteryCheck();
    sei();
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

void sys_batteryCheck(void)
{
    clearLeds();
    adc_setVbatChannel();
    adc_start();
    //while (!adc_isDone());
    u8 level = adc_get_val();
    patternBatteryLevel(level);
}
