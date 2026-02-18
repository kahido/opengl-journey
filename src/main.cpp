#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
// GLFW (include after glad)
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

constexpr int const WIDTH = 640;
constexpr int const HEIGHT = 480;

namespace {

void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height)
{
    spdlog::info("new framebuffer size [{0} x {1}]", width, height);
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        spdlog::info("GLFW_KEY_ESCAPE pressed");
        glfwSetWindowShouldClose(window, 1);
    }
}
}

int main()
{
    if (glfwInit() == 0)
    {
        spdlog::error("GLFW Init failed!");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "[glad] GL with GLFW", nullptr, nullptr);
    if (window == nullptr)
    {
        spdlog::error("Failed create window!");
        return -1;
    }

    glfwMakeContextCurrent(window);

    int const version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        spdlog::error("Failed load GL!");
        return -1;
    }

    spdlog::info("Loaded GL {0}.{1}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // render loop
    while (glfwWindowShouldClose(window) == 0)
    {
        processInput(window);

        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        // check events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
