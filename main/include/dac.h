#pragma once

#ifndef _DAC_H
#define _DAC_H

#include "sdkconfig.h"

#define EXAMPLE_DAC_CHAN0   DAC_CHAN_0      // GPIO25

/*
* DAC can get a value between 0 and 255, which corresponds to values between 0 and Vref volts.
* Because of ADC constraint, we choose maximum output voltage to be between 0 and 1 V volts.
* Based on the current setting, this voltage will be 25%, 50% or 75% of the maximum voltage.
*/
#define DAC_VOLTAGE_MAX    (255 / 3.3) * 1.0

typedef enum dac_setting_change_data_type_tag
{
    DAC_SETTING_MIN = 0,
    DAC_SETTING_1 = 1,
    DAC_SETTING_2 = 2,
    DAC_SETTING_3 = 3,
    DAC_SETTING_MAX
} dac_setting_change_data_type;

void dac_init();
void dac_change_setting(dac_setting_change_data_type setting);
dac_setting_change_data_type dac_get_setting();

#endif