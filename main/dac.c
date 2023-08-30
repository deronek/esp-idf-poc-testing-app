#include "dac.h"

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "driver/dac_oneshot.h"

static const char *TAG = "DAC";

dac_oneshot_handle_t chan0_handle;
dac_setting_change_data_type current_setting;
int current_voltage;

static void dac_set_voltage(int voltage);

void dac_init()
{
    /* DAC oneshot init */
    dac_oneshot_config_t chan0_cfg = {
        .chan_id = EXAMPLE_DAC_CHAN0,
    };
    ESP_ERROR_CHECK(dac_oneshot_new_channel(&chan0_cfg, &chan0_handle));
    // TODO: refactor
    ESP_ERROR_CHECK(dac_oneshot_output_voltage(chan0_handle, DAC_VOLTAGE_MAX * 2 / 4));
    current_setting = DAC_SETTING_2;
}

void dac_change_setting(dac_setting_change_data_type setting)
{
    switch (setting)
    {
    case DAC_SETTING_1:
        dac_set_voltage(DAC_VOLTAGE_MAX * 1 / 4);
        break;
    case DAC_SETTING_2:
        dac_set_voltage(DAC_VOLTAGE_MAX * 2 / 4);
        break;
    case DAC_SETTING_3:
        dac_set_voltage(DAC_VOLTAGE_MAX * 3 / 4);
        break;
    default:
        ESP_LOGE(TAG, "[Setting] Incorrect DAC setting provided: %d", setting);
        return;
    }
    current_setting = setting;
    ESP_LOGI(TAG, "[Setting] DAC setting set: %d", setting);
}

dac_setting_change_data_type dac_get_setting()
{
    return current_setting;
}

static void dac_set_voltage(int voltage)
{
    ESP_ERROR_CHECK(dac_oneshot_output_voltage(chan0_handle, voltage));
}