
#pragma once
#include <glad/gl.h>

#include <string>

struct Shader
{
    GLuint m_id = 0;

    bool loadShaderProgramFromFile(char const* vertexShaderPath, char const* fragmentShaderPath);

    bool loadShaderProgramFromData(std::string const& vertexShaderData, std::string const& fragmentShaderData);

    void bind();

    void clear();

    GLint getUniformLocation(char const* name);
};
