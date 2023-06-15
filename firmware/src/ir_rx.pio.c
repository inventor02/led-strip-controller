#include "ir_rx.pio.h"

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

void ir_rx_program_init(PIO pio, uint sm, uint offset, uint gpio)
{
    // get the default config for this program
    pio_sm_config config = ir_rx_program_get_default_config(offset);

    // set up our pins
    pio_gpio_init(pio, gpio);
    gpio_pull_up(gpio);
    pio_sm_set_consecutive_pindirs(pio, sm, gpio, 1, false);
    sm_config_set_in_pins(&config, gpio);
    sm_config_set_jmp_pin(&config, gpio);
    
    // join the TX FIFO to the RX FIFO, doubling the length of the RX FIFO
    sm_config_set_fifo_join(&config, PIO_FIFO_JOIN_RX);

    // set the ISR as right to left (as LSB first), autopush after 32 bits
    sm_config_set_in_shift(&config, true, true, 24);

    // calculate how fast we want to run our clock (TSHORT should be 560us)
    float target_clock_hz = ir_rx_TSHORT / 560e-6;

    // calculate the clock divider
    float clkdiv = clock_get_hz(clk_sys) / target_clock_hz;
    sm_config_set_clkdiv(&config, clkdiv);

    // start the PIO
    pio_sm_set_config(pio, sm, &config);
    pio_sm_set_enabled(pio, sm, true);
}