#include <stdio.h>
#include <time.h>

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

void ir_fall_isr(uint gpio, uint32_t events){
  cli();

  // Check start bit

  sei();
}

int main() {
  init();
  self_test();

  gpio_set_irq_enabled_with_callback(PIN_IR, GPIO_IRQ_EDGE_FALL, true, &ir_fall_isr);

  for (;;);
}
