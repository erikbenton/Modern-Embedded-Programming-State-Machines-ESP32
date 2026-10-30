#ifndef _TIME_BOMB_H_
#define _TIME_BOMB_H_

#include <stdint.h>
#include "active_object.h"

#define INITIAL_BLINK_COUNT 5

typedef struct
{
    Active super;
    enum
    {
        WAIT4BUTTON_STATE,
        BLINK_STATE,
        PAUSE_STATE,
        BOOM_STATE,
        /* ... */
        MAX_STATE
    } state;
    TimeEvent tEvt;
    uint32_t blink_counter;
} TimeBomb;

typedef enum STATUS
{
    TRAN_STATUS,
    HANDLED_STATUS,
    IGNORED_STATUS,
    INIT_STATUS
} Status;

void TimeBomb_ctor(TimeBomb *const me);
typedef Status (*TimeBombAction)(TimeBomb *const me, Event const *const e);

#endif