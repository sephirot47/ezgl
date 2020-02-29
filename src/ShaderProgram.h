#pragma once

#include <cstdint>

#include "Shader.h"

namespace egl
{

class ShaderProgram
{
public:
    ShaderProgram(const VertexShader& inVertexShader, const FragmentShader& inFragmentShader);
    ShaderProgram(const ShaderProgram& inRHS) = delete;
    ShaderProgram& operator=(const ShaderProgram& inRHS) = delete;
    ShaderProgram(ShaderProgram&& inRHS) = default;
    ShaderProgram& operator=(ShaderProgram&& inRHS) = default;
    ~ShaderProgram();

    void Use() const;

    int32_t GetAttribLocation(const std::string& inAttributeName) const;
    uint32_t GetGLId() const { return mGLId; }

private:
    uint32_t mGLId = 0;
};
}

#include "ShaderProgram.tcc"
