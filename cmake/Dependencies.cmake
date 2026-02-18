include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project

  if(NOT TARGET spdlog::spdlog)
    CPMAddPackage(
      NAME
      spdlog
      VERSION
      1.17.0
      GITHUB_REPOSITORY
      "gabime/spdlog"
      OPTIONS
      "SPDLOG_FMT_EXTERNAL ON")
  endif()

  if(NOT TARGET glfw)
    CPMAddPackage("gh:glfw/glfw#3.4")
  endif()

  FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        73db193f853e2ee079bf3ca8a64aa2eaf6459043 #v2.0.8
    GIT_PROGRESS   TRUE
    SOURCE_SUBDIR  cmake
  )

  FetchContent_MakeAvailable(glad)
  glad_add_library(glad_gl_core_43 STATIC REPRODUCIBLE LOADER API gl:core=4.3)
  #target_link_libraries(my_target PRIVATE glad_gl_core_43)

endfunction()
