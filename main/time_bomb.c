#include "time_bomb.h"
#include <stdint.h>
#include "bsp.h"
#include "assert.h"

static Status TimeBomb_init(TimeBomb *const me, Event const *const e)
{
    me->state = WAIT4BUTTON_STATE;
    return INIT_STATUS;
}

static Status TimeBomb_wait4button_ENTRY(TimeBomb *const me, Event const *const e)
{
    BSP_ledGreenOn();
    return HANDLED_STATUS;
}

static Status TimeBomb_wait4button_PRESSED(TimeBomb *const me, Event const *const e)
{
    me->blink_counter = INITIAL_BLINK_COUNT;
    me->state = BLINK_STATE;
    return TRAN_STATUS;
}

static Status TimeBomb_wait4button_EXIT(TimeBomb *const me, Event const *const e)
{
    BSP_ledGreenOff();
    return HANDLED_STATUS;
}

static Status TimeBomb_blink_ENTRY(TimeBomb *const me, Event const *const e)
{
    BSP_ledRedOn();
    TimeEvent_arm(&(me->tEvt), pdMS_TO_TICKS(500), 0U);
    return HANDLED_STATUS;
}

static Status TimeBomb_pause_ENTRY(TimeBomb *const me, Event const *const e)
{
    TimeEvent_arm(&(me->tEvt), pdMS_TO_TICKS(500), 0U);
    return HANDLED_STATUS;
}

static Status TimeBomb_blink_EXIT(TimeBomb *const me, Event const *const e)
{
    BSP_ledRedOff();
    return HANDLED_STATUS;
}

static Status TimeBomb_boom_ENTRY(TimeBomb *const me, Event const *const e)
{
    BSP_ledBlueOn();
    BSP_ledGreenOn();
    BSP_ledRedOn();
    return HANDLED_STATUS;
}

static Status TimeBomb_pause_TIMEOUT(TimeBomb *const me, Event const *const e)
{
    (me->blink_counter)--;
    if (me->blink_counter > 0)
    {
        me->state = BLINK_STATE;
        return TRAN_STATUS;
    }
    else
    {
        me->state = BOOM_STATE;
        return TRAN_STATUS;
    }
}

static Status TimeBomb_blink_TIMEOUT(TimeBomb *const me, Event const *const e)
{
    me->state = PAUSE_STATE;
    return TRAN_STATUS;
}

static Status TimeBomb_ignore(TimeBomb *const me, Event const *const e)
{
    return IGNORED_STATUS;
}

TimeBombAction const TimeBomb_table[MAX_STATE][MAX_SIG] = {
    /* INIT | ENTRY | EXIT | PRESSED | RELEASED | TIMEOUT */
    /* wait4button */ {&TimeBomb_init, &TimeBomb_wait4button_ENTRY, &TimeBomb_wait4button_EXIT, &TimeBomb_wait4button_PRESSED, &TimeBomb_ignore, &TimeBomb_ignore},
    /* blink       */ {&TimeBomb_ignore, &TimeBomb_blink_ENTRY, &TimeBomb_blink_EXIT, &TimeBomb_ignore, &TimeBomb_ignore, &TimeBomb_blink_TIMEOUT},
    /* pause       */ {&TimeBomb_ignore, &TimeBomb_pause_ENTRY, &TimeBomb_ignore, &TimeBomb_ignore, &TimeBomb_ignore, &TimeBomb_pause_TIMEOUT},
    /* boom        */ {&TimeBomb_ignore, &TimeBomb_boom_ENTRY, &TimeBomb_ignore, &TimeBomb_ignore, &TimeBomb_ignore, &TimeBomb_ignore}};

void TimeBomb_dispatch(TimeBomb *const me, Event const *const e)
{
    Status stat;
    int prev_state = me->state;

    assert((me->state < MAX_STATE) && (e->sig < MAX_SIG));
    stat = TimeBomb_table[me->state][e->sig](me, e);

    if (stat == TRAN_STATUS)
    {
        assert(me->state < MAX_STATE);
        TimeBomb_table[prev_state][EXIT_SIG](me, (Event *)0);
        TimeBomb_table[me->state][ENTRY_SIG](me, (Event *)0);
    }
    else if (stat == INIT_STATUS)
    {
        TimeBomb_table[me->state][ENTRY_SIG](me, (Event *)0);
    }
}

void TimeBomb_ctor(TimeBomb *const me)
{
    Active_ctor(&(me->super), (DispatchHandler)TimeBomb_dispatch);
    TimeEvent_ctor(&(me->tEvt), TIMEOUT_SIG, &(me->super));
    me->state = WAIT4BUTTON_STATE;
    me->blink_counter = INITIAL_BLINK_COUNT;
}