#include "button_switch.h"
#include "active_object.h"
#include "esp_log.h"

static const char *TAG = "Button SM";

static void Button_dispatch(ButtonSwitch *const me, Event const *const e)
{
    switch (e->sig)
    {
    case INIT_SIG:
        ESP_LOGI(TAG, "Switch Button initialized");
        break;

    case BUTTON_PRESSED:
        ESP_LOGI(TAG, "Switch Button pressed");
        break;

    case BUTTON_RELEASED:
        ESP_LOGI(TAG, "Switch Button released");
        break;

    default:
        break;
    }
}

void ButonSwitch_ctor(ButtonSwitch *const me)
{
    Active_ctor(&(me->super), (DispatchHandler)&Button_dispatch);
}