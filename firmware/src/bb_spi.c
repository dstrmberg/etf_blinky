
#include "bb_spi.h"

#include <avr/io.h>

#define PORT_MOSI	(PORTA)
#define PIN_MOSI	(6)
#define DDR_MOSI 	(DDRA)
#define DDR_SCK		(DDRA)
#define PORT_SCK	(PORTA)
#define PIN_SCK		(4)


void bb_spi_init()
{
	DDR_MOSI |= (1 << PIN_MOSI);
	DDR_SCK |= (1 << PIN_SCK);
	
}

// TODO(noxet): inline?
static void bb_spi_bit(uint8_t bit)
{
	if (bit) {
		PORT_SCK &= ~(1 << PIN_SCK);
		PORT_MOSI |= (1 << PIN_MOSI);
		asm volatile("nop\n\t" ::);
		asm volatile("nop\n\t" ::);
		asm volatile("nop\n\t" ::);
		PORT_SCK |= (1 << PIN_SCK);
		asm volatile("nop\n\t" ::);
		asm volatile("nop\n\t" ::);
		asm volatile("nop\n\t" ::);
		PORT_MOSI &= ~(1 << PIN_MOSI);
		PORT_SCK |= (1 << PIN_SCK);
	} else {
		PORT_SCK &= ~(1 << PIN_SCK);
		PORT_MOSI &= ~(1 << PIN_MOSI);
		asm volatile("nop\n\t" ::);
		asm volatile("nop\n\t" ::);
		asm volatile("nop\n\t" ::);
		PORT_SCK |= (1 << PIN_SCK);
		asm volatile("nop\n\t" ::);
		asm volatile("nop\n\t" ::);
		asm volatile("nop\n\t" ::);
		PORT_MOSI &= ~(1 << PIN_MOSI);
		PORT_SCK |= (1 << PIN_SCK);
	}
}


// It takes roughly 96 us to send one byte by calling this function.
void bb_spi_byte(u8 byte)
{
	for (i8 i = 7; i >= 0; i--) {
		u8 bit = (((byte & (1 << i)) >> i));
		bb_spi_bit(bit);
	}
}
