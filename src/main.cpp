#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
// GLFW (include after glad)
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

constexpr int const WIDTH = 640;
constexpr int const HEIGHT = 480;

int main()
{
    if (glfwInit() == 0)
    {
        spdlog::error("GLFW Init Error!");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "[glad] GL with GLFW", nullptr, nullptr);
    if (window == nullptr)
    {
        spdlog::error("Window Error!");
        return 1;
    }

    glfwMakeContextCurrent(window);
    int const version = gladLoadGL(glfwGetProcAddress);

    spdlog::info("GL {0}.{1}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    return 0;
}
