cmake_minimum_required(VERSION 3.29)

# Disable modules support, this is broken with clang-tidy at the moment
cmake_policy(SET CMP0155 OLD)

# Only set the cxx_standard if it is not set by someone else
if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
endif()

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_NAME "opengl-journey")

# Set the project name and language
project(
  ${PROJECT_NAME}
  VERSION 0.0.1
  DESCRIPTION ""
  LANGUAGES CXX C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(cmake/PreventInSourceBuilds.cmake)
include(cmake/StandardProjectSettings.cmake)
include(cmake/ProjectOptions.cmake)
include(cmake/Dependencies.cmake)

add_subdirectory(src)

set(RESOURCES_DIR "resources")

add_custom_target(
  copy_resources ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${PROJECT_SOURCE_DIR}/${RESOURCES_DIR}
  ${PROJECT_BINARY_DIR}/${RESOURCES_DIR}
  COMMENT "Copying resources into binary directory")

add_dependencies(${PROJECT_NAME} copy_resources)

option(ENABLE_TESTING "Enable Test Builds" OFF)

if(ENABLE_TESTING)
  enable_testing()
  message("Building Tests")
  add_subdirectory(test)
endif()
