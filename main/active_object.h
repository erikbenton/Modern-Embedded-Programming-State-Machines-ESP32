#ifndef _ACTIVE_OBJECT_H_
#define _ACTIVE_OBJECT_H_

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

/* ----------- Event Facilities ----------- */

typedef uint16_t Signal;

enum ReservedSignals
{
    INIT_SIG,  /* Initialization signal */
    ENTRY_SIG, /* Trigger for the entry action */
    EXIT_SIG,  /* Trigger for the exit action */
    USER_SIG   /* User defined signal */
};

typedef struct
{
    Signal sig; /* event signal */
} Event;

/* ----------- Finite State Machine Facilities ----------- */

typedef struct Fsm Fsm;

typedef enum STATUS
{
    TRAN_STATUS,
    HANDLED_STATUS,
    IGNORED_STATUS,
    INIT_STATUS
} State;

typedef State (*StateHandler)(Fsm *const me, Event const *const e);

struct Fsm
{
    StateHandler state; /* the "state variable "*/
};

#define TRANSITION(target_) ((((Fsm *)me)->state) = (StateHandler)(target_), TRAN_STATUS)

void Fsm_ctor(Fsm *const me, StateHandler initial);
void Fsm_init(Fsm *const me, Event const *const e);
void Fsm_dispatch(Fsm *const me, Event const *const e);

/* ----------- Active Object Facilities ----------- */

typedef struct Active Active;

struct Active
{
    Fsm super;           /* finite state machine */
    TaskHandle_t thread; /* private thread */
    QueueHandle_t queue; /* private queue */
};

void Active_ctor(Active *const me, StateHandler initial);
void Active_start(Active *const me,
                  const char *const object_name,
                  const uint32_t stack_depth,
                  UBaseType_t priority,
                  size_t queue_length,
                  size_t event_size);
void Active_post(Active *const me, Event const *const e);

/* ----------- Time Event Facilities ----------- */

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