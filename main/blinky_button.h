#ifndef _BLINKY_BUTTON_H_
#define _BLINKY_BUTTON_H_

#include <stdint.h>
#include "active_object.h"

#define INITIAL_BLINK_TIME 100

typedef struct
{
    Active super;
    enum {
        OFF_STATE,
        ON_STATE
    } state;
    TimeEvent tEvt;
    uint32_t blink_time;
} BlinkyButton;

void BlinkyButton_ctor(BlinkyButton *const me);

#endif