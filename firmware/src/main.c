#include <stdio.h>
#include <time.h>

#include "pico/stdlib.h"

#include "defs.h"

// % of full power
volatile double red_val = 0;
volatile double green_val = 0;
volatile double blue_val = 0;

volatile bool lights_on = false;

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

void ir_isr(uint gpio, uint32_t events){
  cli();

  // Check start bit


  // Store Data
  uint8_t code;
  
  switch (code)
  {
  case POWER_CODE:
    button_on_off_toggle();
    break;
  case RED_CODE:
    update_val(&red_val);
    break;
  case GREEN_CODE:
    update_val(&green_val);
    break;
  case BLUE_CODE:
    update_val(&blue_val);
    break;
  
  default:
    write_light_custom_vals(1, 1, 1);
    break;
  }

  write_light_stored_vals();
}

void button_on_off_toggle(){
  if(!lights_on){
    // Turn on
    lights_on = true;
    // Write the stored values to the lights
    write_light_stored_vals();
  } else {
    lights_on = false;
    // Don't update the values, just write 0 to all of the lights 
    write_light_custom_vals(0, 0, 0);
  }
}

void update_val(double* val){
  *val += 0.1;
  if(*val > 1) *val -= 1;
}

void write_light_stored_vals(){}
void write_light_custom_vals(double red, double green, double blue){}

int main() {
  init();
  self_test();

  gpio_set_irq_enabled_with_callback(PIN_IR, GPIO_IRQ_EDGE_FALL, true, &ir_isr);

  for (;;);
}
