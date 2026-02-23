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
    // GLFW: Initialize and configuration
    // ----------------------------------
    if (glfwInit() == 0)
    {
        spdlog::error("GLFW Init failed!");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFW: Create Window
    // -------------------
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "[glad] GL with GLFW", nullptr, nullptr);
    if (window == nullptr)
    {
        spdlog::error("Failed create window!");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    // glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    int const version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        spdlog::error("Failed load GL!");
        return -1;
    }

    spdlog::info("Loaded GL {0}.{1}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    // build and compile shader program
    // --------------------------------
    // vertex shader
    char const* vertexShaderSource =
        "#version 430 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int32_t success = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == 0)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

        spdlog::error("ERROR::SHADER::VERTEX::COMPILATION_FAILED [{0}]", infoLog);
    }

    // fragment shader
    char const* fragmentShaderSource =
        "#version 430 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    success = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success == 0)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

        spdlog::error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED [{0}]", infoLog);
    }

    // link shaders
    uint32_t shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    success = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);

        spdlog::error("ERROR::SHADER::PROGRAM::LINK_FAILED [{0}]", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // setup vertex data (and buffer(s)) and configure vertex attributes
    // -----------------------------------------------------------------
    // [[maybe_unused]] float vertices[] = {
    //     -0.5F, -0.5F, 0.0F, // left
    //     0.5F, -0.5F, 0.0F,  // right
    //     0.0F, 0.5F, 0.0F    // top
    // };

    [[maybe_unused]] float vertices[] = {
        0.5F, 0.5F, 0.0F, // top right
        0.5F, -0.5F, 0.0F, // bottom right
        -0.5F, -0.5F, 0.0F, // bottom left
        -0.5F, 0.5F, 0.0F, // top left
    };

    uint32_t indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };

    // Vertex Array Object
    uint32_t VAO = 0;
    glGenVertexArrays(1, &VAO);

    // Vertex Buffer Object
    uint32_t VBO = 0;
    glGenBuffers(1, &VBO);

    // Element Buffer Object
    uint32_t EBO = 0;
    glGenBuffers(1, &EBO);

    // 1. bind Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // render loop
    // -----------
    while (glfwWindowShouldClose(window) == 0)
    {
        processInput(window);

        // render
        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (key pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
