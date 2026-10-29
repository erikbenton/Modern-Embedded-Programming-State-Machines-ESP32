#include "blinky_button.h"
#include <stdint.h>
#include "bsp.h"

void BlinkyButton_dispatch(BlinkyButton *const me, Event const *const e)
{
    switch (e->sig)
    {
    case INIT_SIG:
        BSP_ledRedOn();
        __fallthrough;
    case TIMEOUT_SIG:
        if (me->is_led_on)
        {
            BSP_ledGreenOff();
            me->is_led_on = false;
            TimeEvent_arm(&(me->tEvt), me->blink_time, 0U);
        }
        else
        {
            BSP_ledGreenOn();
            me->is_led_on = true;
            TimeEvent_arm(&(me->tEvt), me->blink_time * 3, 0U);
        }
        break;
    case BUTTON_PRESSED_SIG:
        me->blink_time >>= 1; /* half the blink time */
        if (me->blink_time == 0U)
        {
            me->blink_time = INITIAL_BLINK_TIME;
        }
        BSP_ledBlueOn();
        BSP_ledRedOn();
        break;

    case BUTTON_RELEASED_SIG:
        BSP_ledBlueOff();
        BSP_ledRedOff();
        break;
    default:
        break;
    }
}

void BlinkyButton_ctor(BlinkyButton *const me)
{
    Active_ctor(&(me->super), (DispatchHandler)BlinkyButton_dispatch);
    TimeEvent_ctor(&(me->tEvt), TIMEOUT_SIG, &(me->super));
    me->is_led_on = false;
    me->blink_time = INITIAL_BLINK_TIME;
}