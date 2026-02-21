#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
// GLFW (include after glad)
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

constexpr int const WIDTH = 640;
constexpr int const HEIGHT = 480;

namespace {

void framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
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
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    [[maybe_unused]] float vertices[] = {
        -0.5F, -0.5F, 0.0F,
        0.5F, -0.5F, 0.0F,
        0.0F, 0.5F, 0.0F
    };

    uint32_t VBO = 0;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    /*
     * VERTEX SHADER configuration
     */

    char const* vertexShaderSource =
        "#version 430 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    // unsigned int vertexShader = 0;
    uint32_t vertexShader = 0;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int32_t success = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

        spdlog::error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{0}", infoLog);
    }

    /*
     * FRAGMENT SHADER configuration
     */

    char const* fragmentShaderSource =
        "#version 430 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColot = vec4(1.0F, 0.5F, 0.2F, 1.0F);\n"
        "}\0";

    uint32_t fragmentShader = 0;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    success = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

        spdlog::error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{0}", infoLog);
    }

    /*
     * SHADER PROGRAM
     */

    uint32_t shaderProgram = 0;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    success = 0;
    glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);

        spdlog::error("ERROR::SHADER::PROGRAM::LINK_FAILED\n{0}", infoLog);
    }

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


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

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
