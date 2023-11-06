
#include "dl_utils.h"
#include "bb_spi.h"
#include "adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


volatile uint8_t red_led, green_led, blue_led, led_pos;
volatile int8_t k;

void led_send();
static void systemInit(void);

int main(void)
{
	k = 0;
	led_pos = 0;
	
	DDRA |= (1 << PORTA6);
	
    systemInit();
	
    while (1) {
		//PORTA |= (1 << PORTA6);
		led_send();
		//PORTA &= ~(1 << PORTA6);
		//_delay_ms(1);
	}

}

static void systemInit(void)
{
	adc_init();
	adc_interrupt_enable();
	bb_spi_init();
	
    sei();
	
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
	
	u8 delay_count = adc_get_val();
	for (int j = 0; j < delay_count; j++) _delay_ms(.1);
	
	bb_spi_byte(0xFF);
	bb_spi_byte(0xFF);
	bb_spi_byte(0xFF);
	bb_spi_byte(0xFF);
	
}
