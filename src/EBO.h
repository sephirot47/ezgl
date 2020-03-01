#pragma once

#include <cstdint>

namespace egl
{

class EBO
{
public:
    EBO();
    EBO(const void* inData, std::size_t inSize);
    EBO(const EBO& inRHS) = delete;
    EBO& operator=(const EBO& inRHS) = delete;
    EBO(EBO&& inRHS) = default;
    EBO& operator=(EBO&& inRHS) = default;
    ~EBO();

    void Bind() const;
    void UnBind() const;

    void BufferData(const void* inData, std::size_t inSize);

    uint32_t GetGLId() const { return mGLId; }

private:
    uint32_t mGLId = 0;
};
}
