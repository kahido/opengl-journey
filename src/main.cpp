
#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
// GLFW (include after glad)
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "shader.h"
#include "glDebug.h"

constexpr int const WIDTH = 1024;
constexpr int const HEIGHT = 768;
constexpr char const* const WINDOW_NAME = "opengl_textures";

namespace {

void framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
{
    spdlog::info("FRAMEBUFFER: {0} x {1}", width, height);
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        spdlog::info("PRESSED -> [GLFW_KEY_ESCAPE]");
        glfwSetWindowShouldClose(window, 1);
    }
}

// setup vertex data (and buffer(s)) and configure vertex attributes
// -----------------------------------------------------------------
float vertices[] = {
 // positions               colors               texture
 //    x      y     z       r     g     b        s     t
    0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
   -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
   -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f, // top left
};

unsigned short indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

}

int main()
{
    // GLFW: Initialize and configuration
    // ----------------------------------
    if (glfwInit() == GLFW_FALSE)
    {
        spdlog::error("GLFW Init failed!");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFW: Create Window
    // ----------------------------------------------------------------------------------
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (window == nullptr)
    {
        spdlog::error("Failed create window!");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // GLAD: load all OpenGL function pointers
    // ----------------------------------------------------------------------------------
    int const version = gladLoadGL(glfwGetProcAddress);
    if (version == GL_FALSE)
    {
        spdlog::error("Failed load GL!");
        return -1;
    }

    spdlog::info("GL_VERSION: {0}", reinterpret_cast<char const*>(glGetString(GL_VERSION)));
    spdlog::info("GL_SHADING_LANGUAGE_VERSION: {0}", reinterpret_cast<char const*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

    // GLAD: enable Debug output
    // ----------------------------------------------------------------------------------
    int flags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    // SHADER: initialize shader
    // ----------------------------------------------------------------------------------
    Shader shader;
    shader.loadShaderProgramFromFile("resources/shader.vert", "resources/shader.frag");

    // Vertex Array Object
    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);

    // Vertex Buffer Object
    GLuint VBO = 0;
    glGenBuffers(1, &VBO);

    // Element Buffer Object
    GLuint EBO = 0;
    glGenBuffers(1, &EBO);

    // 1. bind Vertex Array Object first
    //    then bind and set vertex buffer(s)
    //    then configure vertex attributes(s).
    glBindVertexArray(VAO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO
    // as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // TEXTURE

    unsigned int texture1;
    glGenTextures(1, &texture1);
    // glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int width;
    int height;
    int nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("resources/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        spdlog::error("Failed to load texture!");
    }

    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    // glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("resources/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        spdlog::error("Failed to load texture!");
    }

    stbi_image_free(data);

    shader.bind();
    glUniform1i(shader.getUniformLocation("ourTexture1"), 0);
    glUniform1i(shader.getUniformLocation("ourTexture2"), 1);

    // render loop
    // -----------
    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        processInput(window);

        // clear backgroud
        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures and corresponding textures units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.bind();

        glBindVertexArray(VAO);

        // Draw rectangle
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (key pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    shader.clear();

    // GLFW: terminate, clearing all previously allocated GLFW resources.
    // ----------------------------------------------------------------------------------
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
