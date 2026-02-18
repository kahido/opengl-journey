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

include(cmake/PreventInSourceBuilds.cmake)
include(cmake/StandardProjectSettings.cmake)
include(cmake/ProjectOptions.cmake)

include(cmake/Dependencies.cmake)
setup_dependencies()

add_subdirectory(src)

if(ENABLE_TESTING)
  enable_testing()
  message("Building Tests")
  add_subdirectory(test)
endif()
