#include <stdio.h>
#include <time.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/sync.h"
#include "hardware/timer.h"

#include <time.h>

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
  uint32_t state = save_and_disable_interrupts();
  // GPIO_GET 0 - low, !=0 high

  // Check start bit - 4.5ms high, 4.5ms low => 4.5ms low, 4.5ms high
  bool was_low = true;
  bool was_high = true;
  busy_wait_ms(2);
  if(gpio_get(PIN_IR) != 0) was_low = false;
  busy_wait_ms(2);
  if(gpio_get(PIN_IR) != 0) was_low = false;
  // Should switch to high now after ~0.5ms
  // Now 1.5ms into high band
  busy_wait_ms(2);
  if(gpio_get(PIN_IR) == 0) was_high = false;
  busy_wait_ms(3);
  if(gpio_get(PIN_IR) == 0) was_high = false;

  uint8_t bits[32];
  uint32_t code;

  if(was_low && was_high) { // Start bit found
    for(uint8_t i = 0; i < 32; i++){
      busy_wait_ms(0.56); // Wait until start of second pulse
      // 1.69ms high for 1, 0.56ms high for 0
      busy_wait_ms(0.56);
      if(gpio_get(PIN_IR) != 0) code += 2^i;// Still high
      else continue;
    }
  } else {
    restore_interrupts(state);
    return;
  }


  // Store Data
  
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
  restore_interrupts(state);
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
