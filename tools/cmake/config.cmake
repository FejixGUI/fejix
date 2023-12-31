# Output paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_C_STANDARD 99)

if(CMAKE_C_COMPILER_ID MATCHES "(Clang)|(GNU)")
    add_compile_options("-Wall" "-Wextra" "-Wpedantic" "-Wunused")
elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    add_compile_options("/W4")
endif()

# MSVC is not always correctly detected by the CMAKE_C_COMPILER_ID, so it is
# better to define this globally
add_compile_definitions("_CRT_SECURE_NO_WARNINGS")
