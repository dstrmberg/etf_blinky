set(CMAKE_C_COMPILER "avr-gcc")
set(CMAKE_C_OBJCOPY "avr-objcopy")

set(MCU attiny44a)
set(F_CPU 8000000)

# compiler flags
set(C_MCU "-mmcu=${MCU}")
set(C_STD "-std=gnu99")
set(C_WARN "-Wall -Wextra -Wshadow -Wdouble-promotion -fno-common")
set(C_OPT "-Os")
set(C_DEBUG "-Og -ggdb")
set(C_DIAG "-fdiagnostics-color=always")

set(CMAKE_C_FLAGS_RELEASE "${C_MCU} ${C_STD} ${C_WARN} ${C_OPT} ${C_DIAG}")
set(CMAKE_C_FLAGS_DEBUG "${C_MCU} ${C_STD} ${C_WARN} ${C_DEBUG} ${C_DIAG}")

add_compile_definitions(F_CPU=${F_CPU})

add_custom_target(${PROJECT_NAME}.hex ALL DEPENDS ${PROJECT_NAME})
add_custom_command(TARGET ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_C_OBJCOPY} ARGS -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex)
