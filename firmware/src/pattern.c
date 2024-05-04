
#include "pattern.h"
#include "bb_spi.h"

#include <stdint.h>

#define MAX_PATTERNS 3
static patternFunc patterns[MAX_PATTERNS];

static uint8_t currPattern;

static void staticColorRed(void);
static void staticColorGreen(void);
static void staticColorBlue(void);


void patternInit(void)
{
    currPattern = 0;
    patterns[0] = staticColorRed;
    patterns[1] = staticColorBlue;
    patterns[2] = staticColorGreen;
}


patternFunc patternNext(void)
{
    currPattern = (currPattern + 1) % MAX_PATTERNS;
    return patterns[currPattern];
}


patternFunc patternPrevious(void)
{
    currPattern = (currPattern - 1) % MAX_PATTERNS;
    return patterns[currPattern];
}


static void staticColorRed(void)
{
    
	bb_spi_byte(0);
	bb_spi_byte(0);
	bb_spi_byte(0);
	bb_spi_byte(0);
	

	for (int i = 0; i < 10; i++) {
        bb_spi_byte(0xFF);
        bb_spi_byte(0);
        bb_spi_byte(0);
        bb_spi_byte(255);
	}
	
    bb_spi_byte(255);
	bb_spi_byte(255);
	bb_spi_byte(255);
	bb_spi_byte(255);
}


void staticColorBlue(void)
{    
	bb_spi_byte(0);
	bb_spi_byte(0);
	bb_spi_byte(0);
	bb_spi_byte(0);
	

	for (int i = 0; i < 10; i++) {
        bb_spi_byte(0xFF);
        bb_spi_byte(255);
        bb_spi_byte(0);
        bb_spi_byte(0);
	}
	
    bb_spi_byte(255);
	bb_spi_byte(255);
	bb_spi_byte(255);
	bb_spi_byte(255);


}

void staticColorGreen(void)
{    
	bb_spi_byte(0);
	bb_spi_byte(0);
	bb_spi_byte(0);
	bb_spi_byte(0);
	

	for (int i = 0; i < 10; i++) {
        bb_spi_byte(0xFF);
        bb_spi_byte(0);
        bb_spi_byte(255);
        bb_spi_byte(0);
	}
	
    bb_spi_byte(255);
	bb_spi_byte(255);
	bb_spi_byte(255);
	bb_spi_byte(255);


}
