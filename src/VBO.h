#pragma once

#include <cstdint>

#include "GL.h"

namespace egl
{

class VBO
{
public:
    VBO();

    template <typename T>
    VBO(const T* inData, std::size_t inSize);

    VBO(const VBO& inRHS) = delete;
    VBO& operator=(const VBO& inRHS) = delete;
    VBO(VBO&& inRHS) = default;
    VBO& operator=(VBO&& inRHS) = default;
    ~VBO();

    void Bind() const;
    void UnBind() const;
    bool IsBound() const;
    static GLId GetBoundGLId();

    template <typename T>
    void BufferData(const T* inData, std::size_t inSize);

    GLId GetGLId() const { return mGLId; }

private:
    GLId mGLId = 0;
};
}

#include "VBO.tcc"
