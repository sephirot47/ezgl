#pragma once

#include <cstdint>

#include "GL.h"

namespace egl
{

class EBO
{
public:
    EBO();

    template <typename T>
    EBO(const T* inData, std::size_t inSize);

    EBO(const EBO& inRHS) = delete;
    EBO& operator=(const EBO& inRHS) = delete;
    EBO(EBO&& inRHS) = default;
    EBO& operator=(EBO&& inRHS) = default;
    ~EBO();

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

#include "EBO.tcc"
