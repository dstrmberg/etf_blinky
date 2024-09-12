cmake_minimum_required(VERSION 3.20)

project(
    etf_blinky
    VERSION 2.0
    LANGUAGES C ASM
    HOMEPAGE_URL http://add.me
)

# Not the best practice to use this, but this is just a small project (tm)
aux_source_directory(src SRC)

add_executable(${PROJECT_NAME} ${SRC})

# create compile commands database and make a symbolic link in top dir
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(CREATE_LINK
    "${CMAKE_BINARY_DIR}/compile_commands.json"
    "${CMAKE_SOURCE_DIR}/compile_commands.json"
    SYMBOLIC
)
