cmake_minimum_required(VERSION 3.13)

include(lib/pico-sdk/pico_sdk_init.cmake)

set(FW_NAME led_strip_controller)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(${FW_NAME}
  VERSION 0.1.0.0)

pico_sdk_init()

add_executable(${FW_NAME})

target_sources(${FW_NAME}
  PRIVATE src/main.c src/samsung.pio.c)

pico_generate_pio_header(${FW_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/samsung.pio)

target_link_libraries(${FW_NAME} pico_stdlib hardware_pwm hardware_pio hardware_clocks)

pico_add_extra_outputs(${FW_NAME})
