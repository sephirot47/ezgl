#pragma once

#include <cstdint>

#include "GL.h"
#include "Span.h"

namespace egl
{

class EBO
{
public:
    EBO();

    template <typename T>
    explicit EBO(const Span<T>& inData);

    EBO(const EBO& inRHS) = delete;
    EBO& operator=(const EBO& inRHS) = delete;
    EBO(EBO&& inRHS) = default;
    EBO& operator=(EBO&& inRHS) = default;
    ~EBO();

    void Bind() const;
    void UnBind() const;
    bool IsBound() const;
    static GL::Id GetBoundGLId();

    template <typename T>
    void BufferData(const Span<T>& inData);

    GL::Id GetGLId() const;

private:
    GL::Id mGLId = 0;
};
}

#include "EBO.tcc"
