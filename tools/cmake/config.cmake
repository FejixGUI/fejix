# Output paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_C_STANDARD 99)

set(DETAILED_WARNINGS "")
if(CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID MATCHES "GNU")
    list(APPEND DETAILED_WARNINGS "-Wall" "-Wextra" "-Wpedantic" "-Wunused")
elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    list(APPEND DETAILED_WARNINGS "/W4")
endif()

add_compile_options("${DETAILED_WARNINGS}")

# Suppress pointless warnings from MSVC or compilers with MSVC frontends
add_compile_definitions("_CRT_SECURE_NO_WARNINGS")

