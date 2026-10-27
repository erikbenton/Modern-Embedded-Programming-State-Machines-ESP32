#ifndef _BUTTON_SWITCH_H_
#define _BUTTON_SWITCH_H_

#include "active_object.h"

typedef struct
{
    Active super;
} ButtonSwitch;

enum EventSignals
{
    BUTTON_PRESSED = USER_SIG,
    BUTTON_RELEASED
};

void ButonSwitch_ctor(ButtonSwitch *const me);

#endif