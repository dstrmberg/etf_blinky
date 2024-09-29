#include "pattern.h"
#include "bb_spi.h"
#include "button.h"
#include "adc.h"
#include "timer.h"

#include <stdint.h>
#include <string.h>
#include <util/delay.h>


#define NUM_LEDS     10
#define MAX_PATTERNS 4
static patternFunc patterns[MAX_PATTERNS];
static uint8_t currPattern;

#define LED_INTENSITY_STEP 8
static uint8_t ledIntensity = 1;

struct LED_s
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t alpha;
};

struct LED_s ledState[10];


static void staticColorRed(void);
static void staticColorGreen(void);
static void staticColorBlue(void);
static void patternAudioCheck(void);
static void patternAudioLevel(uint16_t level);
static void setLeds(void);
static void clearLeds(void);


void patternInit(void)
{
    currPattern = MAX_PATTERNS - 1; // we call "patternNext" upon boot, so we start at the first
    patterns[0] = staticColorRed;
    patterns[1] = staticColorBlue;
    patterns[2] = staticColorGreen;
    patterns[3] = patternAudioCheck;
}


patternFunc patternNext(void)
{
    currPattern++;
    if (currPattern == MAX_PATTERNS) currPattern = 0;

    return patterns[currPattern];
}


patternFunc patternPrevious(void)
{
    if (currPattern == 0)
        currPattern = MAX_PATTERNS - 1;
    else
        currPattern--;

    return patterns[currPattern];
}


void patternIncreaseIntensity(void)
{
    // We only have 5 bits in the protocol, hence 31 being max
    if (ledIntensity > 31 - LED_INTENSITY_STEP)
    {
        ledIntensity = 31;
    }
    else
    {
        ledIntensity += LED_INTENSITY_STEP;
    }
}


void patternDecreaseIntensity(void)
{
    // min val is 1 otherwise the LEDs will be turned off, which might be confusing
    if (ledIntensity < LED_INTENSITY_STEP)
    {
        ledIntensity = 1;
    }
    else
    {
        ledIntensity -= LED_INTENSITY_STEP;
    }
}


bool patternBootSequence(void)
{
    clearLeds();
    for (int i = 0; i < NUM_LEDS; i++)
    {
        ledState[i].g = 1 + (i + 1) * 10;
        setLeds();
        if (!btnPressed(BUTTON_PWR)) return false;
        _delay_ms(100);
    }

    return true;
}


bool patternShutdownSequence(void)
{
    clearLeds();
    for (int i = 0; i < NUM_LEDS; i++)
    {
        ledState[i].r = 1 + (i + 1) * 10;
    }

    setLeds();

    for (int i = NUM_LEDS - 1; i >= 0; i--)
    {
        ledState[i].r = 0;
        if (!btnPressed(BUTTON_PWR))
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
    clearLeds();
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


static void patternAudioLevel(uint16_t level)
{
    clearLeds();
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


static void patternAudioCheck(void)
{
    adcSetAudioChannel();
    static uint16_t max = 0;
    static uint32_t prev = 0;
    uint16_t dcoff = 400; // 350;
    adc_start();
    while (!adc_isDone())
    {
    }
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
        prev = timerGetUptime();
    }

    if (timerGetUptime() >= prev + TIME_150_MS)
    {
        clearLeds();
        max = 0;
    }
}


static void staticColorRed(void)
{
    clearLeds();

    for (int i = 0; i < NUM_LEDS; i++)
    {
        ledState[i].r = 255;
    }

    setLeds();
}


static void staticColorBlue(void)
{
    clearLeds();

    for (int i = 0; i < NUM_LEDS; i++)
    {
        ledState[i].b = 255;
    }

    setLeds();
}


static void staticColorGreen(void)
{
    clearLeds();

    for (int i = 0; i < NUM_LEDS; i++)
    {
        ledState[i].g = 255;
    }

    setLeds();
}


static void setLeds(void)
{
    bb_spi_byte(0);
    bb_spi_byte(0);
    bb_spi_byte(0);
    bb_spi_byte(0);

    for (int i = 0; i < NUM_LEDS; i++)
    {
        bb_spi_byte(0xE0 | ledIntensity);
        bb_spi_byte(ledState[i].b);
        bb_spi_byte(ledState[i].g);
        bb_spi_byte(ledState[i].r);
    }

    bb_spi_byte(255);
    bb_spi_byte(255);
    bb_spi_byte(255);
    bb_spi_byte(255);
}


static void clearLeds(void)
{
    memset(ledState, 0, NUM_LEDS * sizeof(struct LED_s));
}
