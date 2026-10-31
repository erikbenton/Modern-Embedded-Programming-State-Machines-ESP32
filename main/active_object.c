#include "active_object.h"
#include "assert.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "portmacro.h"
#include "assert.h"

static Event const entry_evt = {ENTRY_SIG};
static Event const exit_evt = {EXIT_SIG};

void Fsm_ctor(Fsm *const me, StateHandler initial)
{
    me->state = initial;
}

void Fsm_init(Fsm *const me, Event const *const e)
{
    assert(me->state != (StateHandler)0);
    (me->state)(me, e);
    (me->state)(me, &entry_evt);
}

void Fsm_dispatch(Fsm *const me, Event const *const e)
{
    State state;
    StateHandler prev_state = me->state;

    assert(me->state != (StateHandler)0);
    state = (me->state)(me, e);

    if (state == TRAN_STATUS)
    {
        assert(me->state != (StateHandler)0);
        prev_state(me, &exit_evt);
        (me->state)(me, &entry_evt);
    }
}

void Active_ctor(Active *const me, StateHandler initial)
{
    Fsm_ctor(&(me->super), initial);
}

static void Active_eventLoop(void *pdata)
{
    Active *me = (Active *)pdata; // get the active object

    Fsm_init(&(me->super), (Event *)0);

    // event loop message-pump
    for (;;)
    {
        Event e;
        esp_err_t err;

        // wait & get the event
        err = xQueueReceive(me->queue, &e, portMAX_DELAY);
        assert(err == pdTRUE);

        // dispatch the event
        Fsm_dispatch(&(me->super), &e);
    }
}

void Active_start(Active *const me,
                  const char *const object_name,
                  const uint32_t stack_depth,
                  UBaseType_t priority,
                  size_t queue_length,
                  size_t event_size)
{
    assert(me); // ensure 'me' is not null

    // create the queue
    me->queue = xQueueCreate(queue_length, event_size);
    assert(me->queue);

    BaseType_t err = xTaskCreate(
        Active_eventLoop,
        object_name,
        stack_depth,
        (void *)me,
        priority,
        &(me->thread));

    assert(err);
}

void Active_post(Active *const me, Event const *const e)
{
    BaseType_t res = xQueueSend(me->queue, e, pdTICKS_TO_MS(100));
    assert(res == pdTRUE);
}

/* Time Events */

#define MAX_TIME_EVENTS 10

static portMUX_TYPE tEvt_mutex = portMUX_INITIALIZER_UNLOCKED; /* mutex for critical sections */

static TimeEvent *tEvts[MAX_TIME_EVENTS]; /* All TimeEvents in the application */
static uint8_t tEvtNum;                   /* Current number of TimeEvents */

void TimeEvent_ctor(TimeEvent *const me, Signal sig, Active *act)
{
    me->super.sig = sig;
    me->act = act;
    me->timeout = 0U;
    me->interval = 0U;

    taskENTER_CRITICAL(&tEvt_mutex);
    assert(tEvtNum < (sizeof(tEvts) / sizeof(tEvts[0])));
    tEvts[tEvtNum] = me;
    tEvtNum++;
    taskEXIT_CRITICAL(&tEvt_mutex);
}

void TimeEvent_arm(TimeEvent *const me, uint32_t timeout, uint32_t interval)
{
    taskENTER_CRITICAL(&tEvt_mutex);
    me->timeout = timeout;
    me->interval = interval;
    taskEXIT_CRITICAL(&tEvt_mutex);
}

void TimeEvent_disarm(TimeEvent *const me)
{
    taskENTER_CRITICAL(&tEvt_mutex);
    me->timeout = 0U;
    taskEXIT_CRITICAL(&tEvt_mutex);
}

void TimeEvent_tick(void)
{
    for (uint8_t i = 0; i < tEvtNum; i++)
    {
        TimeEvent *t = tEvts[i];
        assert(t);
        if (t->timeout > 0U) // if TimeEvent is armed
        {
            if (--(t->timeout) == 0U) // and if it has expired
            {
                Active_post(t->act, &t->super); // post the TimeEvent for its AO
                t->timeout = t->interval;       // reset the timeout interval
            }
        }
    }
}