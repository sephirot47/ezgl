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

    std::optional<GL::Id> GetAttribLocation(std::string_view inName) const;
    std::optional<GL::Id> GetUniformLocation(std::string_view inName) const;
    GL::Id GetGLId() const { return mGLId; }

    void SetUniform(std::string_view inName, bool inValue);
    void SetUniform(std::string_view inName, int8_t inValue);
    void SetUniform(std::string_view inName, int16_t inValue);
    void SetUniform(std::string_view inName, int32_t inValue);
    void SetUniform(std::string_view inName, float inValue);
    void SetUniform(std::string_view inName, double inValue);
    void SetUniform(std::string_view inName, const Vec2i& inValue);
    void SetUniform(std::string_view inName, const Vec2f& inValue);
    void SetUniform(std::string_view inName, const Vec2d& inValue);
    void SetUniform(std::string_view inName, const Vec3i& inValue);
    void SetUniform(std::string_view inName, const Vec3f& inValue);
    void SetUniform(std::string_view inName, const Vec3d& inValue);
    void SetUniform(std::string_view inName, const Vec4i& inValue);
    void SetUniform(std::string_view inName, const Vec4f& inValue);
    void SetUniform(std::string_view inName, const Vec4d& inValue);
    void SetUniform(std::string_view inName, const Mat2f& inValue);
    void SetUniform(std::string_view inName, const Mat2d& inValue);
    void SetUniform(std::string_view inName, const Mat3f& inValue);
    void SetUniform(std::string_view inName, const Mat3d& inValue);
    void SetUniform(std::string_view inName, const Mat4f& inValue);
    void SetUniform(std::string_view inName, const Mat4d& inValue);

    template <typename T>
    void SetUniform(const T& inValue) = delete;

private:
    GL::Id mGLId = 0;
};
}

#include "ShaderProgram.tcc"
