#pragma once

#include <cstdint>

#include "GL.h"
#include "Span.h"

namespace egl
{

class VBO
{
public:
    VBO();

    template <typename T>
    VBO(const Span<T>& inData);

    VBO(const VBO& inRHS) = delete;
    VBO& operator=(const VBO& inRHS) = delete;
    VBO(VBO&& inRHS) = default;
    VBO& operator=(VBO&& inRHS) = default;
    ~VBO();

    void Bind() const;
    void UnBind() const;
    bool IsBound() const;
    static GL::Id GetBoundGLId();

    template <typename T>
    void BufferData(const Span<T>& inData);

    GL::Id GetGLId() const { return mGLId; }

private:
    GL::Id mGLId = 0;
};
}

#include "VBO.tcc"
