#pragma once

#include <stdbool.h>

enum button
{
    BUTTON1 = 1,
    BUTTON2,
    BUTTON_PWR,
};

void btnInit();
bool btnPressed(enum button btn);
void btnDisableISR(enum button btn);
void btnEnableISR(enum button btn);
