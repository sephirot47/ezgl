#include "ShaderProgram.h"

#include <cassert>
#include <GL/glew.h>

#include "Shader.h"

namespace egl
{
ShaderProgram::ShaderProgram(const VertexShader& inVertexShader, const FragmentShader& inFragmentShader)
{
    mGLId = glCreateProgram();
    glAttachShader(mGLId, inVertexShader.GetGLId());
    glAttachShader(mGLId, inFragmentShader.GetGLId());

    glLinkProgram(mGLId);
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(mGLId);
}

void ShaderProgram::Use() const
{
    glUseProgram(mGLId);
}

int32_t ShaderProgram::GetAttribLocation(const std::string& inAttributeName) const
{
    glGetAttribLocation(mGLId, inAttributeName.c_str());
}
}