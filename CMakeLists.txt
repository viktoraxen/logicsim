cmake_minimum_required(VERSION 3.10)

project(logicsim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug, Release, ...)" FORCE)
endif()

add_executable(logicsim 
    src/main.cpp
    src/node.cpp
    src/gate.cpp
    src/scheduler.cpp
)

get_target_property(EXECUTABLE_OUTPUT_NAME logicsim OUTPUT_NAME)

if(NOT EXECUTABLE_OUTPUT_NAME)
  set(EXECUTABLE_OUTPUT_NAME "logicsim")
endif()

file(WRITE ${CMAKE_BINARY_DIR}/.executable_name "${EXECUTABLE_OUTPUT_NAME}")

target_include_directories(logicsim PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Source and destination directories
set(RESOURCE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/res)
set(RESOURCE_DEST_DIR ${CMAKE_BINARY_DIR}/res)

add_custom_target(
  copy_resources ALL
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${RESOURCE_DEST_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCE_SOURCE_DIR}
          ${RESOURCE_DEST_DIR}
  COMMENT "Copying resources to build directory")

# Needs to be in top-level CMakeLists
enable_testing()

add_subdirectory(test)
