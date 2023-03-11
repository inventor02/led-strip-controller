#include <stdio.h>

#include "pico/stdlib.h"

#include "defs.h"

void init() {
  // setup LED
  gpio_init(PIN_LED);
  gpio_set_dir(PIN_LED, GPIO_OUT);
  gpio_put(PIN_LED, 1);

  // setup UART
  stdio_uart_init_full(uart0, 115200, PIN_UART_TX, PIN_UART_RX);
  printf("Hello; LED Strip Controller v%s\n\n", *VERSION);
}

void self_test() {
  // TODO
}

int main() {
  init();
  self_test();
  for (;;);
}
