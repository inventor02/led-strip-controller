#ifndef _DEFS_H
#define _DEFS_H

#include "pico/stdlib.h"

const char *VERSION = "0.1.0"; 

const uint PIN_LED = PICO_DEFAULT_LED_PIN;

const uint PIN_IR = 2;
const uint PIN_PWM_RED = 18;
const uint PIN_PWM_GREEN = 19;
const uint PIN_PWM_BLUE = 20;

const uint PIN_UART_TX = 13;
const uint PIN_UART_RX = 12;

const uint POWER_CODE = 0x000;
const uint RED_CODE   = 0x001;
const uint GREEN_CODE = 0x002;
const uint BLUE_CODE  = 0x003;

const uint PWM_WRAP = 255;

#endif /* _DEFS_H */
