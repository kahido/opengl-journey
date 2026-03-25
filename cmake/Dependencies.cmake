include(cmake/CPM.cmake)

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

if(NOT TARGET imgui::imgui)
  CPMAddPackage(
    NAME
    imgui
    VERSION
    1.92.6
    GITHUB_REPOSITORY
    "ocornut/imgui")
endif()

# message("---- imgui_ADDED = ${imgui_ADDED}")
# message("---- imgui_SOURCE_DIR = ${imgui_SOURCE_DIR}")
# message("---- imgui_BINARY_DIR = ${imgui_BINARY_DIR}")
