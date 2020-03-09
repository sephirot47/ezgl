#include "Shader.h"

#include <iostream>

#include "GL.h"
#include "Macros.h"

namespace egl
{
Shader::Shader(const EShaderType inShaderType, const std::string_view& inSourceCode)
{
    mGLId = GL_SAFE_CALL_RET(glCreateShader(static_cast<GLuint>(inShaderType)));
    if (!mGLId)
        THROW_EXCEPTION("Error creating shader");

    const GLchar* source_code_ptr = inSourceCode.data();
    const GLint source_code_length = inSourceCode.size();
    GL_SAFE_CALL(glShaderSource(mGLId, 1, &source_code_ptr, &source_code_length));

    GL_SAFE_CALL(glCompileShader(mGLId));

    GLint compiled_correctly = false;
    GL_SAFE_CALL(glGetShaderiv(mGLId, GL_COMPILE_STATUS, &compiled_correctly));
    if (!compiled_correctly)
    {
        GLint max_error_length = 0;
        GL_SAFE_CALL(glGetShaderiv(mGLId, GL_INFO_LOG_LENGTH, &max_error_length));

        std::string error_msg;
        error_msg.resize(max_error_length);

        GLint error_length = 0;
        GL_SAFE_CALL(glGetShaderInfoLog(mGLId, max_error_length, &error_length, (GLchar*)error_msg.data()));
        error_msg.resize(error_length - 1);

        THROW_EXCEPTION("Error compiling shader: " << error_msg);
    }
}

Shader::~Shader()
{
    glDeleteShader(mGLId);
}
}
