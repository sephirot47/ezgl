#pragma once

#include <cstdint>

namespace egl
{

class VAOVertexAttrib;

class VAO
{
public:
    VAO();
    VAO(const VAO& inRHS) = delete;
    VAO& operator=(const VAO& inRHS) = delete;
    VAO(VAO&& inRHS) = default;
    VAO& operator=(VAO&& inRHS) = default;
    ~VAO();

    template <typename T>
    void AddVertexAttrib(uint32_t inAttributeLocation, bool inNormalized = false, uint32_t inOffset = 0);
    void AddVertexAttrib(uint32_t inAttributeLocation, const VAOVertexAttrib& inVertexAttrib);
    void RemoveVertexAttrib(uint32_t inAttributeLocation);

    uint32_t GetGLId() const { return mGLId; }

private:
    uint32_t mGLId = 0;
};
}

#include "VAO.tcc"
