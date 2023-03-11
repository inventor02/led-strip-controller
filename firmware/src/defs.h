#ifndef _DEFS_H
#define _DEFS_H

#include "pico/stdlib.h"

const char *VERSION = "0.1.0"; 

const uint PIN_LED = PICO_DEFAULT_LED_PIN;

const uint PIN_IR = 2;
const uint PIN_PWM1 = 18;
const uint PIN_PWM2 = 19;
const uint PIN_PWM3 = 20;

const uint PIN_UART_TX = 13;
const uint PIN_UART_RX = 12;

#endif /* _DEFS_H */