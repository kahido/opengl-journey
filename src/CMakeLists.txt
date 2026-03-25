include_directories(${imgui_SOURCE_DIR})

set(imgui_SOURCES
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  )

add_executable(
  ${PROJECT_NAME}
  main.cpp
  glDebug.cpp
  shader.cpp
  ${imgui_SOURCES}
)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE project_options
          project_warnings
          spdlog::spdlog
          glad_gl_core_43
          glfw
          )
