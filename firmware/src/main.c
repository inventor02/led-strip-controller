#include <stdio.h>
#include <time.h>
#include <math.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/sync.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "hardware/pio.h"

#include "defs.h"
#include "samsung.pio.h"

// % of full power
double red_val = 0;
double green_val = 0;
double blue_val = 0;

bool lights_on = false;

static PIO pio = pio0;
static uint pio_sm;

void self_test() {
  // TODO
}

void set_pwm(double red, double green, double blue) {
  uint red_pwm = floor(red * PWM_WRAP);
  uint green_pwm = floor(green * PWM_WRAP);
  uint blue_pwm = floor(blue * PWM_WRAP);

  pwm_set_gpio_level(PIN_PWM_RED, red_pwm);
  pwm_set_gpio_level(PIN_PWM_GREEN, green_pwm);
  pwm_set_gpio_level(PIN_PWM_BLUE, blue_pwm);
}

void write_light_stored_vals() {}
void write_light_custom_vals(double red, double green, double blue) {}

void button_on_off_toggle() {
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

void update_val(double* val) {
  *val += 0.1;
  if(*val > 1) *val -= 1;
}

void ir_isr(uint gpio, uint32_t events) {
  uint32_t state = save_and_disable_interrupts();
  // GPIO_GET 0 - low, !=0 high

  // Check start bit - 4.5ms high, 4.5ms low => 4.5ms low, 4.5ms high
  bool was_low = true;
  bool was_high = true;
  busy_wait_ms(2);
  if (gpio_get(PIN_IR) != 0) was_low = false;
  busy_wait_us(4500);
  // Should switch to high now after ~0.5ms
  // Now 1.5ms into high band
  if (gpio_get(PIN_IR) == 0) was_high = false;

  // Now at ~6.5ms
  busy_wait_us(2000);

  uint8_t bits[32];
  uint32_t code = 0;

  if (was_low && was_high) { // Start bit found
    for (uint8_t i = 0; i < 32; i++) {
      busy_wait_us(560); // Wait until start of second pulse
      // 1.69ms high for 1, 0.56ms high for 0
      
      busy_wait_us(560);
      if (gpio_get(PIN_IR) != 0) {
        code += 2^i;
        busy_wait_us(1130);
        printf("high\n");
      } // Still high
      else continue;
    }
  } else {
    restore_interrupts(0);
    return;
  }


  // Store Data
  switch (code) {
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

  printf("%d\n", code);

  write_light_stored_vals();
  restore_interrupts(0);
}

void init() {
  // setup LED
  gpio_init(PIN_LED);
  gpio_set_dir(PIN_LED, GPIO_OUT);
  gpio_put(PIN_LED, 1);

  // setup UART
  stdio_uart_init_full(uart0, 115200, PIN_UART_TX, PIN_UART_RX);
  printf("Hello; LED Strip Controller v%s\n\n", VERSION);

  // set pins for PWM
  gpio_set_function(PIN_PWM_RED, GPIO_FUNC_PWM);
  gpio_set_function(PIN_PWM_GREEN, GPIO_FUNC_PWM);
  gpio_set_function(PIN_PWM_BLUE, GPIO_FUNC_PWM);

  uint pwm_red_slice = pwm_gpio_to_slice_num(PIN_PWM_RED);
  uint pwm_green_slice = pwm_gpio_to_slice_num(PIN_PWM_GREEN);
  uint pwm_blue_slice = pwm_gpio_to_slice_num(PIN_PWM_BLUE);

  pwm_set_wrap(pwm_red_slice, PWM_WRAP);
  pwm_set_wrap(pwm_green_slice, PWM_WRAP);
  pwm_set_wrap(pwm_blue_slice, PWM_WRAP);

  pwm_set_gpio_level(PIN_PWM_RED, 0);
  pwm_set_gpio_level(PIN_PWM_GREEN, 0);
  pwm_set_gpio_level(PIN_PWM_BLUE, 0);

  pwm_set_enabled(pwm_red_slice, true);
  pwm_set_enabled(pwm_green_slice, true);
  pwm_set_enabled(pwm_blue_slice, true);

  // gpio_init(PIN_IR);
  // gpio_set_dir(PIN_IR, false);
  // gpio_pull_up(PIN_IR);
  // gpio_set_irq_enabled_with_callback(PIN_IR, GPIO_IRQ_EDGE_FALL, true, &ir_isr);

  pio_sm = pio_claim_unused_sm(pio, true);
  uint offset = pio_add_program(pio, &samsung_program);
  samsung_program_init(pio, pio_sm, offset, PIN_IR);
}

int main() {
  init();
  self_test();

  for (;;) {
    uint32_t rx = pio_sm_get_blocking(pio, pio_sm);
    printf("received: %x\n", rx);
  }
}
