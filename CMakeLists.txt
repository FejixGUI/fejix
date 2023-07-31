# Reasons for not requiring lower versions:
# 3.13 for policy CMP0077 (options honor normal variables)
# - simpler build scripts for tests and small programs
# 3.16 for Objective-C language support
# - Objective-C is required on Apple systems
cmake_minimum_required(VERSION 3.16)

project(fejix VERSION 0.0.1)

# Source paths
set(FEJIX_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(FEJIX_SRC "${FEJIX_ROOT}/src")
set(FEJIX_INCLUDE "${FEJIX_ROOT}/include")
set(FEJIX_TOOLS "${FEJIX_ROOT}/tools")
set(FEJIX_CMAKE "${FEJIX_TOOLS}/cmake")
set(FEJIX_TESTS "${FEJIX_ROOT}/tests")

# Output paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Main target
add_library(fejix STATIC)

# Build
include("${FEJIX_CMAKE}/options.cmake")
include("${FEJIX_CMAKE}/sources.cmake")
include("${FEJIX_CMAKE}/dependencies.cmake")

if(FEJIX_BUILD_TESTS)
    include("${FEJIX_CMAKE}/tests.cmake")
endif()