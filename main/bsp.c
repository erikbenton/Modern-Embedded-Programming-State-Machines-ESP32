#include "bsp.h"
#include "driver/gpio.h"

static uint32_t volatile l_tick_ctr;
static portMUX_TYPE isr_mutex = portMUX_INITIALIZER_UNLOCKED; /* mutex for critical sections */

// Must enable FreeRTOS tick hook in order to use: configUSE_TICK_HOOK
void vApplicationTickHook(void)
{
    ++l_tick_ctr;
}

uint32_t BSP_tick_ctr(void)
{
    uint32_t tick_ctr;

    taskENTER_CRITICAL(&isr_mutex);
    tick_ctr = l_tick_ctr;
    taskEXIT_CRITICAL(&isr_mutex);

    return tick_ctr;
}

uint8_t BSP_SW1(void)
{
    uint8_t sw1;
    taskENTER_CRITICAL(&isr_mutex);
    sw1 = (uint8_t)gpio_get_level(PIN_SW1);
    taskEXIT_CRITICAL(&isr_mutex);

    return (sw1 != 0U) ? 0U : 1U; // my setup has the SW1 logic flipped from Miro's
}

void init_leds(void)
{
    // Config GPIO for LED pins
    gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(RED_LED, GPIO_PULLDOWN_ONLY);

    gpio_set_direction(BLUE_LED, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(BLUE_LED, GPIO_PULLDOWN_ONLY);

    gpio_set_direction(GREEN_LED, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(GREEN_LED, GPIO_PULLDOWN_ONLY);
}

void init_switch(void)
{
    // Config GPIO for SW1
    gpio_set_direction(PIN_SW1, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN_SW1, GPIO_PULLDOWN_ONLY);
}

void BSP_init()
{
    init_leds();
    init_switch();
}

void BSP_ledRedOn(void)
{
    gpio_set_level(RED_LED, LED_ON);
}

void BSP_ledRedOff(void)
{
    gpio_set_level(RED_LED, LED_OFF);
}

void BSP_ledBlueOn(void)
{
    gpio_set_level(BLUE_LED, LED_ON);
}

void BSP_ledBlueOff(void)
{
    gpio_set_level(BLUE_LED, LED_OFF);
}

void BSP_ledGreenOn(void)
{
    gpio_set_level(GREEN_LED, LED_ON);
}

void BSP_ledGreenOff(void)
{
    gpio_set_level(GREEN_LED, LED_OFF);
}