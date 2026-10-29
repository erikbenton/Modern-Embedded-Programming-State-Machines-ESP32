#include "blinky_button.h"
#include <stdint.h>
#include "bsp.h"
#include "assert.h"

void BlinkyButton_dispatch(BlinkyButton *const me, Event const *const e)
{
    if (e->sig == INIT_SIG)
    {
        BSP_ledBlueOff();
        TimeEvent_arm(&(me->tEvt), me->blink_time * 3, 0U);
        me->state = OFF_STATE;
        return;
    }

    switch (me->state)
    {
    case OFF_STATE:
    {
        switch (e->sig)
        {
        case TIMEOUT_SIG:
        {
            BSP_ledGreenOn();
            TimeEvent_arm(&(me->tEvt), me->blink_time, 0U);
            me->state = ON_STATE;
            break;
        }
        case BUTTON_PRESSED_SIG:
        {
            me->blink_time >>= 1; /* half the blink time */
            if (me->blink_time == 0U)
            {
                me->blink_time = INITIAL_BLINK_TIME;
            }
            BSP_ledBlueOn();
            break;
        }
        case BUTTON_RELEASED_SIG:
        {
            BSP_ledBlueOff();
            break;
        }
        default:
            break;
        }
        break;
    }
    case ON_STATE:
    {
        switch (e->sig)
        {
        case TIMEOUT_SIG:
        {
            BSP_ledGreenOff();
            TimeEvent_arm(&(me->tEvt), me->blink_time * 3, 0U);
            me->state = OFF_STATE;
            break;
        }
        case BUTTON_PRESSED_SIG:
        {
            me->blink_time >>= 1; /* half the blink time */
            if (me->blink_time == 0U)
            {
                me->blink_time = INITIAL_BLINK_TIME;
            }
            BSP_ledBlueOn();
            break;
        }
        case BUTTON_RELEASED_SIG:
        {
            BSP_ledBlueOff();
            break;
        }
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

void BlinkyButton_ctor(BlinkyButton *const me)
{
    Active_ctor(&(me->super), (DispatchHandler)BlinkyButton_dispatch);
    TimeEvent_ctor(&(me->tEvt), TIMEOUT_SIG, &(me->super));
    me->state = OFF_STATE;
    me->blink_time = INITIAL_BLINK_TIME;
}