#pragma once

#ifndef _UART_H
#define _UART_H

#include <stdint.h>
#include "adc.h"
#include "dac.h"

#define EX_UART_NUM UART_NUM_0
#define BUF_SIZE (1024)

#define UART_COMMAND_STATUS 's'
#define UART_COMMAND_VARIANT 'v'

void uart_init();
void uart_task(void *args);
void uart_handle_command(uint8_t char_value);

#endif