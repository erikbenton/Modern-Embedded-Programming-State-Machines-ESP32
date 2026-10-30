#include <stdio.h>
#include "bsp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

enum
{
    SAMPLE_SIG
};

void input_state_machine(void *params)
{
    for (;;)
    {
        static enum
        {
            INITIAL,
            OFF_STATE,
            ON_STATE
        } state = INITIAL;
        static uint32_t start;
        static uint8_t prev_button = 1U;

        /* buffering of inputs */
        struct
        {
            uint16_t sig;
            uint32_t now;
            uint8_t button;
        } evt, *e;
        evt.sig = SAMPLE_SIG;
        evt.now = BSP_tick_ctr();
        evt.button = BSP_SW1();

        e = &evt;
        switch (state)
        {
        case INITIAL:
            start = e->now;
            state = OFF_STATE;
            break;

        case OFF_STATE:
            if ((e->now - start) > BSP_TICKS_PER_SECOND * 3U / 4U)
            {
                BSP_ledGreenOn();
                start = e->now;
                state = ON_STATE;
            }
            else if ((prev_button != 0U) && (e->button == 0U)) /* pressed button */
            {
                BSP_ledBlueOn();
                prev_button = e->button;
            }
            else if ((prev_button == 0U) && (e->button != 0U)) /* released button */
            {
                BSP_ledBlueOff();
                prev_button = e->button;
            }
            break;

        case ON_STATE:
            if ((e->now - start) > BSP_TICKS_PER_SECOND / 4U)
            {
                BSP_ledGreenOff();
                start = e->now;
                state = OFF_STATE;
            }
            else if ((prev_button != 0U) && (e->button == 0U)) /* pressed button */
            {
                BSP_ledBlueOn();
                prev_button = e->button;
            }
            else if ((prev_button == 0U) && (e->button != 0U)) /* released button */
            {
                BSP_ledBlueOff();
                prev_button = e->button;
            }
            break;

        default:
            break;
        }

        // small blocking needed because of ESP32 + FreeRTOS config
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void app_main(void)
{
    BSP_init();

    // create task to run the state machine since
    // the ESP FreeRTOS WTD doesn't want infinite loops in app_main
    xTaskCreate(input_state_machine, "Input SM", 2048, NULL, 1, NULL);
}