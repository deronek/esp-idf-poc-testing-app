/*
 * esp-idf-poc-testing-app
 * Created by Mateusz Dionizy
 * Proof-of-concept system for HIL testing workflow
 *
 * Based on ESP-IDF examples:
 * - adc\oneshot_read
 * - uart\uart_events
 * https://github.com/espressif/esp-idf
 * 
 * License of these examples:
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "adc.h"
#include "uart.h"
#include "dac.h"

static const char *TAG = "MAIN";

void app_main()
{
    adc_init();
    uart_init();
    dac_init();

    xTaskCreate(adc_task, "adc_task", 4096, NULL, 10, NULL);
    xTaskCreate(uart_task, "uart_task", 4096, NULL, 10, NULL);
}
