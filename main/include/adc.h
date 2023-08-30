#pragma once

#ifndef _ADC_H
#define _ADC_H

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#if CONFIG_IDF_TARGET_ESP32
#define EXAMPLE_ADC1_CHAN0  ADC_CHANNEL_4   // GPIO32
#else
#define EXAMPLE_ADC1_CHAN0  ADC_CHANNEL_2
#endif

#define EXAMPLE_ADC_UNIT ADC_UNIT_1
#define EXAMPLE_ADC_BITWIDTH ADC_BITWIDTH_DEFAULT

#if CONFIG_APPLICATION_SW_VARIANT_A
#pragma message("Application SW variant A selected")
#define EXAMPLE_ADC_ATTEN   ADC_ATTEN_DB_0
#define SW_VARIANT_STR      "A"
#elif CONFIG_APPLICATION_SW_VARIANT_B
#pragma message("Application SW variant B selected")
#define EXAMPLE_ADC_ATTEN   ADC_ATTEN_DB_2_5
#define SW_VARIANT_STR      "B"
#elif CONFIG_APPLICATION_SW_VARIANT_C
#pragma message("Application SW variant C selected")
#define EXAMPLE_ADC_ATTEN   ADC_ATTEN_DB_6
#define SW_VARIANT_STR      "C"
#elif CONFIG_APPLICATION_SW_VARIANT_D
#pragma message("Application SW variant D selected")
#define EXAMPLE_ADC_ATTEN   ADC_ATTEN_DB_11
#define SW_VARIANT_STR      "D"
#else
#error "Application SW variant not defined"
#endif

// typedef enum adc_setting_change_data_type_tag
// {
//     ADC_SETTING_MIN = 0,
//     ADC_SETTING_1 = 1,
//     ADC_SETTING_2 = 2,
//     ADC_SETTING_3 = 3,
//     ADC_SETTING_4 = 4,
//     ADC_SETTING_MAX
// } adc_setting_change_data_type;

void adc_init();
void adc_task(void *args);
bool example_adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle);
void example_adc_calibration_deinit(adc_cali_handle_t handle);
// void adc_change_setting(adc_setting_change_data_type setting);
// adc_setting_change_data_type adc_get_setting();

#endif