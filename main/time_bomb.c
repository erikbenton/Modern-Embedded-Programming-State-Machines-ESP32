#include "time_bomb.h"
#include <stdint.h>
#include "bsp.h"
#include "assert.h"

void TimeBomb_dispatch(TimeBomb *const me, Event const *const e)
{
    if (e->sig == INIT_SIG)
    {
        BSP_ledGreenOn();
        me->state = WAIT4BUTTON_STATE;
        return;
    }

    switch (me->state)
    {
    case WAIT4BUTTON_STATE:
    {
        switch (e->sig)
        {
        case BUTTON_PRESSED_SIG:
        {
            BSP_ledGreenOff();
            BSP_ledRedOn();
            TimeEvent_arm(&(me->tEvt), pdMS_TO_TICKS(500), 0U);
            me->blink_counter = INITIAL_BLINK_COUNT;
            me->state = BLINK_STATE;
            break;
        }
        default:
            break;
        }
        break;
    }
    case BLINK_STATE:
    {
        switch (e->sig)
        {
        case TIMEOUT_SIG:
        {
            BSP_ledRedOff();
            TimeEvent_arm(&(me->tEvt), pdMS_TO_TICKS(500), 0U);
            me->state = PAUSE_STATE;
            break;
        }

        default:
            break;
        }
        break;
    }
    case PAUSE_STATE:
    {
        switch (e->sig)
        {
        case TIMEOUT_SIG:
        {
            (me->blink_counter)--;
            if (me->blink_counter > 0)
            {
                BSP_ledRedOn();
                TimeEvent_arm(&(me->tEvt), pdMS_TO_TICKS(500), 0U);
                me->state = BLINK_STATE;
            }
            else
            {
                BSP_ledBlueOn();
                BSP_ledGreenOn();
                BSP_ledRedOn();
                me->state = BOOM_STATE;
            }
            break;
        }
        default:
            break;
        }
        break;
    }
    case BOOM_STATE:
    {
        switch (e->sig)
        {
        default:
            break;
        }
        break;
    }
    default:
    {
        assert(0);
        break;
    }
    }
}

void TimeBomb_ctor(TimeBomb *const me)
{
    Active_ctor(&(me->super), (DispatchHandler)TimeBomb_dispatch);
    TimeEvent_ctor(&(me->tEvt), TIMEOUT_SIG, &(me->super));
    me->state = WAIT4BUTTON_STATE;
    me->blink_counter = INITIAL_BLINK_COUNT;
}