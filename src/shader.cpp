
#include "shader.h"

#include <fstream>
#include <vector>

#include "spdlog/spdlog.h"

namespace
{

std::string readEntireFile(char const* source)
{
    std::ifstream file(source, std::ios::in);

    if (!file.is_open())
    {
        spdlog::error("Failed to open file! [{}]", source);
        return {};
    }

    file.seekg(0, std::ios::end);

    auto fileSize = file.tellg();

    file.seekg(0, std::ios::beg);

    auto fileContent = std::string{};
    fileContent.resize(static_cast<std::size_t>(fileSize));

    file.read(&fileContent[0], fileSize);

    file.close();

    return fileContent;
}

GLuint createShaderFromData(std::string const& data, GLenum shaderType)
{
    GLuint shaderId = glCreateShader(shaderType);

    char const* shaderSource = data.data();

    glShaderSource(shaderId, 1, &shaderSource, nullptr);
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
            infoLog.resize(static_cast<std::size_t>(l));

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
    std::string vertexData = readEntireFile(vertexShaderPath);
    std::string fragmentData = readEntireFile(fragmentShaderPath);

    if (vertexData.empty() || fragmentData.empty())
    {
        return false;
    }

    bool ret = loadShaderProgramFromData(vertexData, fragmentData);

    return ret;
}

bool Shader::loadShaderProgramFromData(
    std::string const& vertexShaderData, std::string const& fragmentShaderData)
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
            infoLog.resize(static_cast<std::size_t>(l));

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

GLint Shader::getUniformLocation(char const* name)
{
    GLint rez = glGetUniformLocation(m_id, name);

    if (rez == -1)
    {
        spdlog::error("ERROR::SHADER::UNIFORM [name = {0}]", name);
    }

    return rez;
}
