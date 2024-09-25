
#include "pattern.h"
#include "bb_spi.h"
#include "button.h"

#include <stdint.h>
#include <string.h>
#include <util/delay.h>

#define MAX_PATTERNS 3
static patternFunc patterns[MAX_PATTERNS];

static uint8_t currPattern;

struct LED_s
{
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


    for (int i = 0; i < 10; i++)
    {
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

    // TODO(noxet): fix this, should probably not be done here
    // setLeds();
}


bool patternBootSequence(void)
{
    clearLeds();
    for (int i = 0; i < 10; i++)
    {
        ledState[i].g = 1 + (i + 1) * 5;
        setLeds();
        if (!btnPwrPressed()) return false;
        _delay_ms(100);
    }

    return true;
}


bool patternShutdownSequence(void)
{
    clearLeds();
    for (int i = 0; i < 10; i++)
    {
        ledState[i].r = 127;
    }

    setLeds();

    for (int i = 9; i >= 0; i--)
    {
        ledState[i].r = 0;
        if (!btnPwrPressed())
        {
            clearLeds();
            setLeds();
            return false;
        }
        _delay_ms(75);
        setLeds();
    }

    return true;
}


void patternBatteryLevel(uint8_t level)
{
    // clearLeds();
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


void patternAudioLevel(uint16_t level)
{
    // clearLeds();
    if (level > 250)
    {
        ledState[9].r = 127;
    }
    if (level > 225)
    {
        ledState[8].r = 127;
    }
    if (level > 200)
    {
        ledState[7].r = 127;
    }
    if (level > 175)
    {
        ledState[6].r = 127;
    }
    if (level > 150)
    {
        ledState[5].r = 127;
    }
    if (level > 125)
    {
        ledState[4].r = 127;
        ledState[4].g = 1;
    }
    if (level > 100)
    {
        ledState[3].r = 127;
        ledState[3].g = 5;
    }
    if (level > 75)
    {
        ledState[2].r = 127;
        ledState[2].g = 10;
    }
    if (level > 50)
    {
        ledState[1].r = 127;
        ledState[1].g = 15;
    }
    if (level > 0)
    {
        ledState[0].r = 127;
        ledState[0].g = 20;
    }
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
    currPattern++;
    if (currPattern == MAX_PATTERNS) currPattern = 0;

    return patterns[currPattern];
}


patternFunc patternPrevious(void)
{
    if (currPattern == 0) currPattern = MAX_PATTERNS - 1;
    else currPattern--;

    return patterns[currPattern];
}


void staticColorRed(void)
{
    bb_spi_byte(0);
    bb_spi_byte(0);
    bb_spi_byte(0);
    bb_spi_byte(0);

    for (int i = 0; i < 10; i++)
    {
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

    for (int i = 0; i < 10; i++)
    {
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

    for (int i = 0; i < 10; i++)
    {
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
