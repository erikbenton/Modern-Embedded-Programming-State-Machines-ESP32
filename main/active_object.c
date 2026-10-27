#include "active_object.h"
#include "assert.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

void Active_ctor(Active *const me, DispatchHandler dispatch)
{
    me->dispatch = dispatch;
}

static void Active_eventLoop(void *pdata)
{
    Active *me = (Active *)pdata; // get the active object

    static Event const initEvt = {INIT_SIG};
    (me->dispatch)(me, &initEvt); // dispatch initialization for the AO

    // event loop message-pump
    for (;;)
    {
        Event e;
        esp_err_t err;

        // wait & get the event
        err = xQueueReceive(me->queue, &e, portMAX_DELAY);
        assert(err == pdTRUE);

        // dispatch the event
        (me->dispatch)(me, &e);
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