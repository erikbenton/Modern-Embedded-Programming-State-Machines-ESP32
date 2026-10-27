#include <stdio.h>
#include "active_object.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "button_switch.h"

#define PIN_SWITCH 22

TaskHandle_t gpio_task_handle;
ButtonSwitch button;
Active *AO_Button = &button.super;

static void IRAM_ATTR gpio_isr_handler(void *args)
{
    static uint32_t count = 0;
    xTaskNotifyFromISR(gpio_task_handle, count++, eSetValueWithOverwrite, NULL);
}

static void gpio_task(void *params)
{
    uint32_t count;
    for (;;)
    {
        xTaskNotifyWait(0, 0, &count, portMAX_DELAY);
        ESP_LOGI("TASK", "Button pressed %d times total", count);
        static Event const buttonPressedEvent = {BUTTON_PRESSED};
        Active_post(AO_Button, &buttonPressedEvent);
    }
}

void app_main(void)
{
    // Initialize the Button and start its event loop
    ButonSwitch_ctor(&button);
    Active_start(AO_Button, "Button AO", 1024, 1, 10, sizeof(Event));

    // Config GPIO for ISR
    gpio_set_direction(PIN_SWITCH, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN_SWITCH, GPIO_PULLDOWN_ONLY);
    gpio_set_intr_type(PIN_SWITCH, GPIO_INTR_POSEDGE);

    // Create task for handling button ISR
    xTaskCreate(gpio_task, "GPIO Task", 1024, NULL, 1, &gpio_task_handle);

    // Initialize the GPIO ISR + handler
    gpio_install_isr_service(0);
    gpio_isr_handler_add(PIN_SWITCH, gpio_isr_handler, NULL);
}
