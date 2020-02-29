#include "Shader.h"

#include <GL/glew.h>
#include <iostream>

namespace egl
{
Shader::Shader(const EShaderType inShaderType, const std::string& inSourceCode)
{
    mGLId = glCreateShader(static_cast<GLuint>(inShaderType));
    if (!mGLId)
        throw std::runtime_error("Error creating shader");

    mSourceCode = inSourceCode;

    const GLchar* source_code_ptr = mSourceCode.c_str();
    const GLint source_code_length = mSourceCode.size();
    glShaderSource(mGLId, 1, &source_code_ptr, &source_code_length);

    glCompileShader(mGLId);

    GLint compiled_correctly = false;
    glGetShaderiv(mGLId, GL_COMPILE_STATUS, &compiled_correctly);
    if (!compiled_correctly)
    {
        int max_error_length;
        glGetShaderiv(mGLId, GL_INFO_LOG_LENGTH, &max_error_length);

        std::string error_msg;
        error_msg.resize(max_error_length);

        int error_length;
        glGetShaderInfoLog(mGLId, max_error_length, &error_length, (GLchar*)error_msg.data());
        error_msg.resize(error_length - 1);

        throw std::runtime_error("Error compiling shader: " + error_msg);
    }
}

Shader::~Shader()
{
    glDeleteShader(mGLId);
}
}
