#pragma once

#include <cstdint>
#include <string>

#include "EShaderType.h"

namespace egl
{
class Shader
{
public:
    const std::string& GetSourceCode() const { return mSourceCode; }
    uint32_t GetGLId() const { return mGLId; }

protected:
    Shader(EShaderType inShaderType, const std::string& inSourceCode);
    Shader(const Shader& inRHS) = delete;
    Shader& operator=(const Shader& inRHS) = delete;
    Shader(Shader&& inRHS) = default;
    Shader& operator=(Shader&& inRHS) = default;
    virtual ~Shader();

private:
    uint32_t mGLId = 0;
    std::string mSourceCode;
};

class VertexShader : public Shader
{
public:
    explicit VertexShader(const std::string& inSourceCode)
        : Shader(EShaderType::VERTEX, inSourceCode)
    {
    }
    VertexShader(const VertexShader& inRHS) = delete;
    VertexShader& operator=(const VertexShader& inRHS) = delete;
    VertexShader(VertexShader&& inRHS) = default;
    VertexShader& operator=(VertexShader&& inRHS) = default;
    ~VertexShader() override{};
};

class FragmentShader : public Shader
{
public:
    explicit FragmentShader(const std::string& inSourceCode)
        : Shader(EShaderType::FRAGMENT, inSourceCode)
    {
    }
    FragmentShader(const FragmentShader& inRHS) = delete;
    FragmentShader& operator=(const FragmentShader& inRHS) = delete;
    FragmentShader(FragmentShader&& inRHS) = default;
    FragmentShader& operator=(FragmentShader&& inRHS) = default;
    ~FragmentShader() override{};
};
}
