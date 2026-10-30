#include <stdio.h>
#include "active_object.h"
#include "bsp.h"
#include "time_bomb.h"

TimeBomb time_bomb;

void app_main(void)
{
    BSP_init(&time_bomb.super);

    TimeBomb_ctor(&time_bomb);
    Active_start(&time_bomb.super, "Time Bomb AO", 1024, 1, 10, sizeof(Event));
}