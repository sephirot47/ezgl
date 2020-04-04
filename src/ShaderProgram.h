#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#include "GL.h"
#include "Mat.h"
#include "Shader.h"
#include "Vec.h"

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

    void Bind() const;
    void UnBind() const;
    bool IsBound() const;
    static GL::Id GetBoundGLId();

    std::optional<GL::Id> GetAttribLocation(const std::string_view inAttribName) const;
    std::optional<GL::Id> GetUniformLocation(const std::string_view inUniformName) const;
    GL::Id GetGLId() const;

    template <typename T>
    void SetUniform(const GL::Id inUniformLocation, const T& inValue);
    template <typename T>
    void SetUniform(const std::string_view inName, const T& inValue);

private:
    GL::Id mGLId = 0;

    GL::Id GetUniformLocationWithException(const ShaderProgram& inShaderProgram, const std::string_view inName);
};
}

#include "ShaderProgram.tcc"
