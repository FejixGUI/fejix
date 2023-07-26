cmake_minimum_required(VERSION 3.16)

project(example)

set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(FEJIX_ROOT "${ROOT}/../..")
set(FEJIX_BUILD "${ROOT}/build/fejix")

add_executable(example)

add_subdirectory("${FEJIX_ROOT}" "${FEJIX_BUILD}")
target_link_libraries(example "fejix")

target_include_directories(example PRIVATE "${ROOT}")

target_sources(example PRIVATE
    "${ROOT}/main.c"
    "${ROOT}/printer.c"
    "${ROOT}/printer_configurator.c"
)