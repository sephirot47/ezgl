#pragma once

#include <GL/glew.h>

#include "EGLType.h"

namespace egl
{
struct VAOVertexAttrib
{
    explicit VAOVertexAttrib(
        uint32_t inNumComponents,
        EGLType inType,
        uint32_t inStride,
        bool inNormalized = false,
        uint32_t inOffset = 0)
        : numComponents(inNumComponents)
        , type(inType)
        , normalized(inNormalized)
        , stride(inStride)
        , offset(inOffset)
    {
    }

    uint32_t numComponents = 1;
    EGLType type = EGLType::BOOL;
    bool normalized = false;
    uint32_t stride = 0;
    uint32_t offset = 0;
};

template <typename T = bool>
struct VAOVertexAttribT : public VAOVertexAttrib
{
    explicit VAOVertexAttribT(
        uint32_t inStride = egl::EGLTypeTraits<T>::NumBytes,
        bool inNormalized = false,
        uint32_t inOffset = 0)
        : VAOVertexAttrib(
              egl::EGLTypeTraits<T>::NumComponents,
              egl::EGLTypeTraits<T>::Type,
              inStride,
              inNormalized,
              inOffset)
    {
    }
};
}