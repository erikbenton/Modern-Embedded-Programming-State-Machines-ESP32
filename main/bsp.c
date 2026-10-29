#include "bsp.h"
#include "driver/gpio.h"
#include "soc/gpio_periph.h"
#include "hal/gpio_ll.h"

static Active *AO_BlinkyButton;

// Must enable FreeRTOS tick hook in order to use: configUSE_TICK_HOOK
void vApplicationTickHook(void)
{
    static struct ButtonsDebouncing
    {
        uint32_t depressed;
        uint32_t previous;
    } buttons = {0U, 0U};
    uint32_t current;
    uint32_t tmp;

    TimeEvent_tick(); /* process AO Time Events */

    // current = (uint32_t)gpio_get_level(PIN_SW1) << PIN_SW1;
    current = GPIO.in;                                 /* read level of 0-31 GPIO pins */
    tmp = buttons.depressed;                           /* save the debounced depressed buttons */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous = current;                        /* update the history */
    tmp ^= buttons.depressed;                          /* changed debounced depressed */
    if ((tmp & SW1) != 0U)
    { /* debounced SW1 state changed? */
        if ((buttons.depressed & SW1) != 0U)
        { /* is SW1 depressed? */
            /* post the "button-pressed" event */
            static Event const buttonPressedEvt = {BUTTON_PRESSED_SIG};
            Active_post(AO_BlinkyButton, &buttonPressedEvt);
        }
        else
        { /* the button is released */
            /* post the "button-released" event */
            static Event const buttonReleasedEvt = {BUTTON_RELEASED_SIG};
            Active_post(AO_BlinkyButton, &buttonReleasedEvt);
        }
    }
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

void BSP_init(Active *blinky_button_active)
{
    init_leds();
    init_switch();
    AO_BlinkyButton = blinky_button_active;
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