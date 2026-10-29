#ifndef _ACTIVE_OBJECT_H_
#define _ACTIVE_OBJECT_H_

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

typedef uint16_t Signal;

enum ReservedSignals
{
    INIT_SIG, /* Initialization signal */
    USER_SIG  /* User defined signal */
};

typedef struct
{
    Signal sig; /* event signal */
} Event;

typedef struct Active Active;

typedef void (*DispatchHandler)(Active *const me, Event const *const e);

struct Active
{
    TaskHandle_t thread;      /* private thread */
    QueueHandle_t queue;      /* private queue */
    DispatchHandler dispatch; /* dispatch method for events */
};

void Active_ctor(Active *const me, DispatchHandler dispatch);
void Active_start(Active *const me,
                  const char *const object_name,
                  const uint32_t stack_depth,
                  UBaseType_t priority,
                  size_t queue_length,
                  size_t event_size);
void Active_post(Active *const me, Event const *const e);

/* Time Events */

typedef struct
{
    Event super;
    Active *act;
    uint32_t timeout;
    uint32_t interval;
} TimeEvent;

void TimeEvent_ctor(TimeEvent *const me, Signal sig, Active *act);
void TimeEvent_arm(TimeEvent *const me, uint32_t timeout, uint32_t interval);
void TimeEvent_disarm(TimeEvent *const me);

void TimeEvent_tick(void);

#endif