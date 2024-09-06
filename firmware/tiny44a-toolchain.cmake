set(CMAKE_C_COMPILER "avr-gcc")

add_compile_options(-mmcu=attiny44a -Os)
add_compile_options(-Wall -Wextra -pedantic -std=gnu99)

set(CMAKE_C_FLAGS_RELEASE "-Os")
set(CMAKE_C_FLAGS_DEBUG "-Og -ggdb")

set(CMAKE_C_OBJCOPY avr-objcopy)
add_custom_target(${PROJECT_NAME}.hex ALL DEPENDS ${PROJECT_NAME})
add_custom_command(TARGET ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_C_OBJCOPY} ARGS -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex)
