#include "sys.h"

#include <avr/io.h>

#include "dl_utils.h"
#include "bb_spi.h"
#include "adc.h"
#include "button.h"

#define SYS_PWR_EN_PORT (PORTA)
#define SYS_PWR_EN_PIN  (3)

void sys_init(void)
{
    /*
	adc_init();
	adc_interrupt_enable();
	bb_spi_init();
    */
    DDRA |= (1 << SYS_PWR_EN_PIN);
	button_init();
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
    //SYS_PWR_EN_PORT |= (1 << SYS_PWR_EN_PIN);
    PORTA |= (1 << 3);
}

void sys_powerOff(void)
{
    //SYS_PWR_EN_PORT &= ~(1 << SYS_PWR_EN_PIN);
    PORTA &= ~(1 << 3);
}

void sys_batteryCheck(void)
{
    
}
