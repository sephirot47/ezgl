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
        : mNumComponents(inNumComponents)
        , mType(inType)
        , mNormalized(inNormalized)
        , mStride(inStride)
        , mOffset(inOffset)
    {
    }

    uint32_t mNumComponents = 1;
    EGLType mType = EGLType::BOOL;
    bool mNormalized = false;
    uint32_t mStride = 0;
    uint32_t mOffset = 0;
};

template <typename T = bool>
struct VAOVertexAttribT : public VAOVertexAttrib
{
    explicit VAOVertexAttribT(
        uint32_t inStride = EGLTypeTraits<T>::NumBytes,
        bool inNormalized = false,
        uint32_t inOffset = 0)
        : VAOVertexAttrib(
            EGLTypeTraits<T>::NumComponents,
            EGLTypeTraits<T>::GLComponentType,
            inStride,
            inNormalized,
            inOffset)
    {
    }
};
}