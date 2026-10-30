#ifndef _BSP_H_
#define _BSP_H_

#include "active_object.h"

#define RED_LED 21
#define BLUE_LED 23
#define GREEN_LED 18

#define LED_ON 1U
#define LED_OFF 0U

#define PIN_SW1 22
#define SW1 1U << PIN_SW1

enum EventSignals
{
    BUTTON_PRESSED_SIG = USER_SIG,
    BUTTON_RELEASED_SIG,
    TIMEOUT_SIG,
    /* ... */
    MAX_SIG
};

void BSP_init(Active *blinky_button_active);
void BSP_ledRedOn(void);
void BSP_ledRedOff(void);
void BSP_ledBlueOn(void);
void BSP_ledBlueOff(void);
void BSP_ledGreenOn(void);
void BSP_ledGreenOff(void);

#endif