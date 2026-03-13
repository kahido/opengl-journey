add_executable(
  ${PROJECT_NAME}
  main.cpp
  glDebug.cpp
  shader.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ../resources)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE project_options
          project_warnings
          spdlog::spdlog
          glad_gl_core_43
          glfw
          )
