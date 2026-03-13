
#include "shader.h"

#include <fstream>
#include <string>
#include <vector>

#include "spdlog/spdlog.h"

namespace
{

char const* readEntireFile(char const* source)
{
    std::ifstream file(source, std::ios::in);

    if (!file.is_open())
    {
        spdlog::error("Failed to open file! [{}]", source);
        return nullptr;
    }

    auto fileSize = file.tellg();

    auto buffer = std::string{};
    buffer.reserve(static_cast<std::size_t>(fileSize));

    file.seekg(0, std::ios::beg);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer.c_str();
}

GLuint createShaderFromData(char const* data, GLenum shaderType)
{
    GLuint shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &data, nullptr);
    glCompileShader(shaderId);

    GLint result = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

    if (result == 0)
    {
        GLint l = 0;

        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &l);

        if (l != 0)
        {
            auto infoLog = std::string{};
            infoLog.reserve(static_cast<std::size_t>(l));

            glGetShaderInfoLog(shaderId, l, &l, infoLog.data());

            spdlog::error("ERROR::SHADER::COMPILATION_FAILED [data = {0} info = {1}]", data, infoLog);
        }
        else
        {
            spdlog::error("ERROR::SHADER::COMPILATION_FAILED [data = {0} info = unknown error]", data);
        }

        glDeleteShader(shaderId);

        shaderId = 0;
        return shaderId;
    }

    return shaderId;
}

}// namespace

bool Shader::loadShaderProgramFromFile(char const* vertexShaderPath, char const* fragmentShaderPath)
{
    char const* vertexData = readEntireFile(vertexShaderPath);
    char const* fragmentData = readEntireFile(fragmentShaderPath);

    if (vertexData == nullptr || fragmentData == nullptr)
    {
        delete[] vertexData;
        delete[] fragmentData;

        return false;
    }

    bool ret = loadShaderProgramFromData(vertexData, fragmentData);

    delete[] vertexData;
    delete[] fragmentData;

    return ret;
}

bool Shader::loadShaderProgramFromData(
    char const* vertexShaderData, char const* fragmentShaderData)
{
    auto vertexId = createShaderFromData(vertexShaderData, GL_VERTEX_SHADER);

    if (vertexId == 0)
    {
        return false;
    }

    auto fragmentId = createShaderFromData(fragmentShaderData, GL_FRAGMENT_SHADER);

    if (fragmentId == 0)
    {
        glDeleteShader(vertexId);
        return false;
    }

    m_id = glCreateProgram();

    glAttachShader(m_id, vertexId);
    glAttachShader(m_id, fragmentId);

    glLinkProgram(m_id);

    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);

    GLint result = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, &result);

    if (result != GL_TRUE)
    {
        GLint l = 0;

        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &l);

        if (l != 0)
        {
            auto infoLog = std::string{};
            infoLog.reserve(static_cast<std::size_t>(l));

            glGetProgramInfoLog(m_id, l, &l, infoLog.data());

            spdlog::error("ERROR::SHADER::PROGRAM::LINK_FAILED [info = {0}]", infoLog);

        }

        glDeleteProgram(m_id);

        return false;
    }

    glValidateProgram(m_id);

    return true;
}

void Shader::bind()
{
    glUseProgram(m_id);
}

void Shader::clear()
{
    glDeleteProgram(m_id);
}
