
#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
// GLFW (include after glad)
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <spdlog/spdlog.h>

#include "shader.h"
#include "glDebug.h"

constexpr int const WIDTH = 1024;
constexpr int const HEIGHT = 768;
constexpr char const* const WINDOW_NAME = "opengl_experiment";

namespace {

void framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
{
    spdlog::info("framebuffer size [{0} x {1}]", width, height);
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

// setup vertex data (and buffer(s)) and configure vertex attributes
// -----------------------------------------------------------------
float vertices[] = {
 // positions       colors
 // x y z           r  g  b
    0.5, 0.5, 0,    1, 0, 0, // vertex 1
   -0.5, 0.5, 0,    0, 1, 0, // vertex 2
   -0.5, -0.5, 0,   0, 0, 1, // vertex 3
    0.5, -0.5, 0,   0, 0, 1, // vertex 4
};

unsigned short indices[] = {
    0, 1, 2, // first triangle
    0, 2, 3  // second triangle
};

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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (window == nullptr)
    {
        spdlog::error("Failed create window!");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    int const version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        spdlog::error("Failed load GL!");
        return -1;
    }

    spdlog::info("GL_VERSION: {0}", reinterpret_cast<char const*>(glGetString(GL_VERSION)));
    spdlog::info("GL_SHADING_LANGUAGE_VERSION: {0}", reinterpret_cast<char const*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

    int32_t flags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    ImGui::CreateContext();
    // Setup Platform/Renderer backends
    // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");


    Shader shader;

    shader.loadShaderProgramFromFile("resources/myshader.vert", "resources/myshader.frag");

    shader.bind();

    // GLint u_time = shader.getUniformLocation("u_time");
    GLint u_color = shader.getUniformLocation("u_color");

    // Vertex Array Object
    uint32_t VAO = 0;
    glGenVertexArrays(1, &VAO);

    // Vertex Buffer Object
    uint32_t VBO = 0;
    glGenBuffers(1, &VBO);

    // Element Buffer Object
    uint32_t EBO = 0;
    glGenBuffers(1, &EBO);

    // 1. bind Vertex Array Object first, then bind and set vertex buffer(s), and then
    //    configure vertex attributes(s).
    glBindVertexArray(VAO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO
    // as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
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

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Window");
        ImGui::Text("Color test");
        static float color[3] = { 0.5, 0.5, 0.5 };
        ImGui::ColorPicker3("Color: ", color);
        ImGui::End();

        // draw first triangle
        // glUseProgram(shaderProgram);

        /*
         *  change traingle color using time value
         */

        // float timeValue = static_cast<float>(glfwGetTime()) * 10;
        // spdlog::info("timeValue {0}", timeValue);

        // glUniform1f(u_time, timeValue);

        shader.bind();

        glUniform3fv(u_color, 1, color);

        glBindVertexArray(VAO);

        // #1 option - Draw Triangle
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // #2 option - Draw 2 Triangles  as a square
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

        glBindVertexArray(0);

        // Rendering
        // (Your code clears your framebuffer, renders your other stuff etc.)
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (key pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // glDeleteProgram(shaderProgram);

    shader.clear();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
