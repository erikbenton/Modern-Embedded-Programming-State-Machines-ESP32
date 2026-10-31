#ifndef _TIME_BOMB_H_
#define _TIME_BOMB_H_

#include <stdint.h>
#include "active_object.h"

#define INITIAL_BLINK_COUNT 5

typedef struct TimeBomb TimeBomb;

struct TimeBomb
{
    Active super;
    TimeEvent tEvt;
    uint32_t blink_counter;
};

void TimeBomb_ctor(TimeBomb *const me);
State TimeBomb_initial(TimeBomb *const me, Event const *const e);
State TimeBomb_wait4Button(TimeBomb *const me, Event const *const e);
State TimeBomb_blink(TimeBomb *const me, Event const *const e);
State TimeBomb_pause(TimeBomb *const me, Event const *const e);
State TimeBomb_boom(TimeBomb *const me, Event const *const e);
typedef State (*TimeBombAction)(TimeBomb *const me, Event const *const e);

#endif