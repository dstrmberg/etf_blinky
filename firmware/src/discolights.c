
#include "pattern.h"
#include "scheduler.h"
#include "sys.h"
#include "bb_spi.h"
#include "adc.h"
#include "button.h"
#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


volatile uint8_t red_led, green_led, blue_led, led_pos;
volatile int8_t k;

#define MINILED (1 << PB1)

void led_send();


int main(void)
{
    sys_init();


    //dl_schedulerInit();
    //button_init();
    //staticColorGreen();

    patternInit();

    patternFunc currentPat = patternNext();

    while(1)
    {
        
        //dl_run();
        if (PINA & (1 << PA7))
        {
            currentPat = patternNext();
            _delay_ms(300);
            //sys_debugLedOn(true);
            //timer_start();
        }
        else if (PINB & (1 << PB2))
        {
            currentPat = patternPrevious();
            _delay_ms(300);
            //PORTB &= ~MINILED;
        }

        if (btnPwrPressed())
        {
            if (patternShutdownSequence()) sys_powerOff();
        }

        //led_send();

        currentPat();
    }
}


void led_send()
{
	
	bb_spi_byte(0);
	bb_spi_byte(0);
	bb_spi_byte(0);
	bb_spi_byte(0);
	

	for (int i = 0; i < 10; i++) {
		
		if (led_pos == i) {
			bb_spi_byte(0xFF);
			bb_spi_byte(255);
			bb_spi_byte(0);
			bb_spi_byte(0);
		} else {
			bb_spi_byte(0xFF);
			bb_spi_byte(0);
			bb_spi_byte(10);
			bb_spi_byte(0);
		}
				
	}
	
	led_pos = led_pos + k;
	
	if (led_pos == 9) k = -1;
	if (led_pos == 0)  k =  1;
	
	for (int j = 0; j < 128; j++) _delay_ms(.1);
	
	bb_spi_byte(0xFF);
	bb_spi_byte(0xFF);
	bb_spi_byte(0xFF);
	bb_spi_byte(0xFF);
	
}

