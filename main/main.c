#include <stdio.h>
#include "active_object.h"
#include "bsp.h"
#include "blinky_button.h"

BlinkyButton blinky_button;

void app_main(void)
{
    BSP_init(&blinky_button.super);

    BlinkyButton_ctor(&blinky_button);
    Active_start(&blinky_button.super, "Blinky Button AO", 1024, 1, 10, sizeof(Event));
}