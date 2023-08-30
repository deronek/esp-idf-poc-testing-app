#include "uart.h"

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "soc/soc_caps.h"
#include "utils.h"

static QueueHandle_t uart0_queue;
static const char *TAG = "UART";

void uart_task(void *args)
{
    uart_event_t event;
    uint8_t *dtmp = (uint8_t *)malloc(BUF_SIZE);
    for (;;)
    {
        // Waiting for UART event.
        if (xQueueReceive(uart0_queue, (void *)&event, (TickType_t)portMAX_DELAY))
        {
            bzero(dtmp, BUF_SIZE);
            // ESP_LOGI(TAG, "uart[%d] event:", EX_UART_NUM);
            switch (event.type)
            {
            // Event of UART receving data
            /*We'd better handler data event fast, there would be much more data events than
            other types of events. If we take too much time on data event, the queue might
            be full.*/
            case UART_DATA:
                if (event.size < 1)
                {
                    break;
                }
                uart_read_bytes(EX_UART_NUM, dtmp, event.size, portMAX_DELAY);
                uart_handle_command(dtmp[0]);
                break;
            // Event of HW FIFO overflow detected
            case UART_FIFO_OVF:
                ESP_LOGI(TAG, "[UART] HW FIFO overflow");
                // If fifo overflow happened, you should consider adding flow control for your application.
                // The ISR has already reset the rx FIFO,
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(EX_UART_NUM);
                xQueueReset(uart0_queue);
                break;
            // Event of UART ring buffer full
            case UART_BUFFER_FULL:
                ESP_LOGI(TAG, "[UART] Ring buffer full");
                // If buffer full happened, you should consider increasing your buffer size
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(EX_UART_NUM);
                xQueueReset(uart0_queue);
                break;
            // Event of UART RX break detected
            case UART_BREAK:
                ESP_LOGI(TAG, "uart rx break");
                break;
            // Event of UART parity check error
            case UART_PARITY_ERR:
                ESP_LOGE(TAG, "uart parity error");
                break;
            // Event of UART frame error
            case UART_FRAME_ERR:
                ESP_LOGE(TAG, "uart frame error");
                break;
            // UART_PATTERN_DET
            case UART_PATTERN_DET:
                // we do not use patterns
                break;
            // Others
            default:
                ESP_LOGI(TAG, "uart event type: %d", event.type);
                break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

void uart_init()
{
    esp_log_level_set(TAG, ESP_LOG_INFO);

    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // Install UART driver, and get the queue.
    uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart0_queue, 0);
    uart_param_config(EX_UART_NUM, &uart_config);

    // Set UART log level
    esp_log_level_set(TAG, ESP_LOG_INFO);
    // Set UART pins (using UART0 default pins ie no changes.)
    uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void uart_handle_command(uint8_t char_value)
{
    dac_setting_change_data_type setting;

    switch (char_value)
    {
    case INT_TO_CHAR(DAC_SETTING_1):
    case INT_TO_CHAR(DAC_SETTING_2):
    case INT_TO_CHAR(DAC_SETTING_3):
        setting = (dac_setting_change_data_type)CHAR_TO_INT(char_value);
        dac_change_setting(setting);
        break;
    case UART_COMMAND_STATUS:
        setting = dac_get_setting();
        ESP_LOGI(TAG, "[Setting] Current DAC setting: %d", setting);
        break;
    case UART_COMMAND_VARIANT:
        ESP_LOGI(TAG, "[Variant] SW variant: %s", SW_VARIANT_STR);
        break;
    default:
        ESP_LOGW(TAG, "[Command] Incorrect command provided: %c", char_value);
        return;
    }
}
