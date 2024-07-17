
#include "pattern.h"
#include "bb_spi.h"
#include "util/delay.h"

#include <stdint.h>
#include <string.h>

#define MAX_PATTERNS 3
static patternFunc patterns[MAX_PATTERNS];

static uint8_t currPattern;

struct LED_s {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t alpha;
};

struct LED_s ledState[10];

// TODO(noxet): make static again
void staticColorRed(void);
void staticColorGreen(void);
void staticColorBlue(void);


void setLeds(void)
{
	bb_spi_byte(0);
	bb_spi_byte(0);
	bb_spi_byte(0);
	bb_spi_byte(0);
	

	for (int i = 0; i < 10; i++) {
        bb_spi_byte(0xEF);
        bb_spi_byte(ledState[i].r);
        bb_spi_byte(ledState[i].b);
        bb_spi_byte(ledState[i].g);
	}
	
    bb_spi_byte(255);
	bb_spi_byte(255);
	bb_spi_byte(255);
	bb_spi_byte(255);
}


void clearLeds(void)
{
    memset(ledState, 0, 10 * sizeof(struct LED_s));
    setLeds();
}


bool patternBootSequence(void)
{
    for (int i = 0; i < 5; i++)
    {
        // TODO(noxet): check pwr button
        ledState[i].g = 255;
        ledState[9 - i].g = 255;
        setLeds();
        _delay_ms(100);

    }

    return true;
}


bool patternBootSequence2(void)
{
    for (int i = 0; i < 10; i++)
    {
        ledState[i].g = 1 + (i + 1) * 5;
        setLeds();
        _delay_ms(50);
    }

    return true;
}


void patternBatteryLevel(u8 level)
{
    if (level > 250) ledState[9].r = 42;
    if (level > 225) ledState[8].r = 42;
    if (level > 200) ledState[7].r = 42;
    if (level > 175) ledState[6].r = 42;
    if (level > 150) ledState[5].r = 42;
    if (level > 125) ledState[4].r = 42;
    if (level > 100) ledState[3].r = 42;
    if (level > 75) ledState[2].r = 42;
    if (level > 50) ledState[1].r = 42;
    if (level > 0) ledState[0].r = 42;
    setLeds();
}


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


void staticColorRed(void)
{
    
	bb_spi_byte(0);
	bb_spi_byte(0);
	bb_spi_byte(0);
	bb_spi_byte(0);
	

	for (int i = 0; i < 10; i++) {
        bb_spi_byte(0xE1);
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
        bb_spi_byte(0xE1);
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
        bb_spi_byte(0xE1);
        bb_spi_byte(0);
        bb_spi_byte(255);
        bb_spi_byte(0);
	}
	
    bb_spi_byte(255);
	bb_spi_byte(255);
	bb_spi_byte(255);
	bb_spi_byte(255);


}
