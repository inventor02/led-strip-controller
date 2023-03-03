cmake_minimum_required(VERSION 3.13)

include(lib/pico-sdk/pico_sdk_init.cmake)

set(PROJECT_NAME led_strip_controller)

project(${PROJECT_NAME})

pico_sdk_init()

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
  PRIVATE src/main.c)

target_link_libraries(${PROJECT_NAME} pico_stdlib)

pico_add_extra_outputs(${PROJECT_NAME})
