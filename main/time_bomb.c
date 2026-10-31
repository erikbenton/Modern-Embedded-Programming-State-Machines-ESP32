#include "time_bomb.h"
#include <stdint.h>
#include "bsp.h"
#include "assert.h"
#include "active_object.h"

State TimeBomb_initial(TimeBomb *const me, Event const *const e)
{
    return TRANSITION(TimeBomb_wait4Button);
}

State TimeBomb_wait4Button(TimeBomb *const me, Event const *const e)
{
    State status;
    switch (e->sig)
    {
    case ENTRY_SIG:
    {
        BSP_ledBlueOff();
        BSP_ledRedOff();
        BSP_ledGreenOn();
        status = HANDLED_STATUS;
        break;
    }
    case EXIT_SIG:
    {
        BSP_ledGreenOff();
        status = HANDLED_STATUS;
        break;
    }
    case BUTTON_PRESSED_SIG:
    {
        me->blink_counter = INITIAL_BLINK_COUNT;
        status = TRAN_STATUS;
        status = TRANSITION(TimeBomb_blink);
        break;
    }
    default:
        status = IGNORED_STATUS;
        break;
    }

    return status;
}

State TimeBomb_blink(TimeBomb *const me, Event const *const e)
{
    State status;
    switch (e->sig)
    {
    case ENTRY_SIG:
    {
        BSP_ledRedOn();
        TimeEvent_arm(&(me->tEvt), pdMS_TO_TICKS(500), 0U);
        status = HANDLED_STATUS;
        break;
    }
    case EXIT_SIG:
    {
        BSP_ledRedOff();
        status = HANDLED_STATUS;
        break;
    }
    case TIMEOUT_SIG:
    {
        status = TRANSITION(TimeBomb_pause);
        break;
    }
    default:
        status = IGNORED_STATUS;
        break;
    }

    return status;
}

State TimeBomb_pause(TimeBomb *const me, Event const *const e)
{
    State status;
    switch (e->sig)
    {
    case ENTRY_SIG:
    {
        TimeEvent_arm(&(me->tEvt), pdMS_TO_TICKS(500), 0U);
        status = HANDLED_STATUS;
        break;
    }
    case TIMEOUT_SIG:
    {
        (me->blink_counter)--;
        if (me->blink_counter > 0)
        {
            status = TRANSITION(TimeBomb_blink);
        }
        else
        {
            status = TRANSITION(TimeBomb_boom);
        }
        break;
    }
    default:
        status = IGNORED_STATUS;
        break;
    }

    return status;
}

State TimeBomb_boom(TimeBomb *const me, Event const *const e)
{
    State status;
    switch (e->sig)
    {
    case ENTRY_SIG:
    {
        BSP_ledBlueOn();
        BSP_ledGreenOn();
        BSP_ledRedOn();
        status = HANDLED_STATUS;
        break;
    }
    default:
        status = IGNORED_STATUS;
        break;
    }

    return status;
}

void TimeBomb_ctor(TimeBomb *const me)
{
    Active_ctor(&(me->super), (StateHandler)TimeBomb_initial);
    TimeEvent_ctor(&(me->tEvt), TIMEOUT_SIG, &(me->super));
    me->blink_counter = INITIAL_BLINK_COUNT;
}