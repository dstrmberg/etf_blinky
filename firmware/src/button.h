#pragma once

#include <stdbool.h>

enum button
{
    BUTTON1 = 1,
    BUTTON2,
};

void btnInit();
bool btnPwrPressed(void);
void btnDisableISR(enum button btn);
void btnEnableISR(enum button btn);
