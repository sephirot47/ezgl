#pragma once

#include <GL/glew.h>

#include <cstdint>

#include "EGLType.h"

namespace egl
{
template <typename T>
struct VAOVertexAttribT
{
    uint32_t numComponents = EGLTypeTraits<T>::NumComponents;
    EGLType type = EGLTypeTraits<T>::Type;
    bool normalized = false;
    uint32_t stride = EGLTypeTraits<T>::Size;
    uint32_t offset = 0;
};

class VAOVertexAttrib : public VAOVertexAttribT<int32_t>
{
};
}