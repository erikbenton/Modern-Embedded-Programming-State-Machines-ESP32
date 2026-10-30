#ifndef _TIME_BOMB_H_
#define _TIME_BOMB_H_

#include <stdint.h>
#include "active_object.h"

#define INITIAL_BLINK_COUNT 10

typedef struct
{
    Active super;
    enum
    {
        WAIT4BUTTON_STATE,
        BLINK_STATE,
        PAUSE_STATE,
        BOOM_STATE
    } state;
    TimeEvent tEvt;
    uint32_t blink_counter;
} TimeBomb;

void TimeBomb_ctor(TimeBomb *const me);

#endif