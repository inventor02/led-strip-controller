#include "samsung.pio.h"

#include "hardware/clocks.h"

void samsung_program_init(PIO pio, uint sm, uint offset, uint gpio)
{
    pio_gpio_init(pio, gpio);
    pio_sm_set_consecutive_pindirs(pio, sm, gpio, 1, false);

    pio_sm_config config = samsung_program_get_default_config(offset);
    sm_config_set_in_pins(&config, gpio);
    sm_config_set_jmp_pin(&config, gpio);
    sm_config_set_fifo_join(&config, PIO_FIFO_JOIN_RX);
    sm_config_set_in_shift(&config, true, true, 32);

    float sys_clk_hz = clock_get_hz(clk_sys);
    float target_hz = samsung_CYCLES_PER_BURST / (600e-6);
    float clk_div = sys_clk_hz / target_hz;

    sm_config_set_clkdiv(&config, clk_div);

    pio_sm_init(pio, sm, offset, &config);
    pio_sm_set_enabled(pio, sm, true);
}