cmake_minimum_required(VERSION 3.29)

include(cmake/PreventInSourceBuilds.cmake)
include(cmake/BuildEnvironmentSetup.cmake)

set(PROJECT_NAME "opengl-journey")

# Set the project name and language
project(
  ${PROJECT_NAME}
  VERSION 1.1.1
  DESCRIPTION ""
  LANGUAGES CXX C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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

if (ENABLE_TESTING)
  message("Building Tests")
  add_subdirectory(test)
endif()
