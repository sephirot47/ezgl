#pragma once

#include <cstdint>

namespace egl
{

class VBO
{
public:
    VBO();
    VBO(const void* inData, std::size_t inSize);
    VBO(const VBO& inRHS) = delete;
    VBO& operator=(const VBO& inRHS) = delete;
    VBO(VBO&& inRHS) = default;
    VBO& operator=(VBO&& inRHS) = default;
    ~VBO();

    void Bind() const;
    void UnBind() const;

    void BufferData(const void* inData, std::size_t inSize);

    uint32_t GetGLId() const { return mGLId; }

private:
    uint32_t mGLId = 0;
};
}

#include "VBO.tcc"
