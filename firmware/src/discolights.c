
#include "scheduler.h"
#include "sys.h"
#include "bb_spi.h"
#include "adc.h"
#include "button.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


volatile uint8_t red_led, green_led, blue_led, led_pos;
volatile int8_t k;

void led_send();

int main(void)
{
    //_delay_ms(1000);
    
    sys_init();

  //  _delay_ms(2000);

//    sys_powerOff();
	
    dl_schedulerInit();
    //button_init();

    while(1)
    {
        //dl_run();
        if (PINA & (1 << PA7))
        {
            sys_powerOff();
        }
    }


    /*
    while (1)
    {
        // Check events
        bb_spi_byte(0x00);
        bb_spi_byte(0x00);
        bb_spi_byte(0x00);
        bb_spi_byte(0x00);
        
        bb_spi_byte(0xE0);
        bb_spi_byte(0xFF);
        bb_spi_byte(0x00);
        bb_spi_byte(0xFF);
        
        bb_spi_byte(0xE0);
        bb_spi_byte(0x00);
        bb_spi_byte(0xFF);
        bb_spi_byte(0xFF);
        
        bb_spi_byte(0xFF);
        bb_spi_byte(0xFF);
        bb_spi_byte(0xFF);
        bb_spi_byte(0xFF);
	}
    */

}

/*

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
	
	u8 delay_count = adc_get_val();
	for (int j = 0; j < delay_count; j++) _delay_ms(.1);
	
	bb_spi_byte(0xFF);
	bb_spi_byte(0xFF);
	bb_spi_byte(0xFF);
	bb_spi_byte(0xFF);
	
}

*/
