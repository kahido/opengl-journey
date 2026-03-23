
#pragma once
#include <glad/gl.h>

struct Shader
{
    GLuint m_id = 0;

    bool loadShaderProgramFromFile(char const* vertexShaderPath, char const* fragmentShaderPath);

    bool loadShaderProgramFromData(char const* vertexShaderData, char const* fragmentShaderData);

    void bind();

    void clear();

    GLint getUniformLocation(char const* name);
};
