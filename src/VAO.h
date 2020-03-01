#pragma once

#include <cstdint>

namespace egl
{

class EBO;
class VAOVertexAttrib;
class VBO;

class VAO
{
public:
    VAO();
    VAO(const VAO& inRHS) = delete;
    VAO& operator=(const VAO& inRHS) = delete;
    VAO(VAO&& inRHS) = default;
    VAO& operator=(VAO&& inRHS) = default;
    ~VAO();

    void Bind() const;
    void UnBind() const;
    void AddVBO(const VBO& inVBO, uint32_t inAttribIndex, const VAOVertexAttrib& inVertexAttrib);
    void SetEBO(const EBO& inEBO);

    void AddVertexAttrib(uint32_t inAttributeLocation, const VAOVertexAttrib& inVertexAttrib);
    void RemoveVertexAttrib(uint32_t inAttributeLocation);

    uint32_t GetGLId() const { return mGLId; }

private:
    uint32_t mGLId = 0;
};
}

#include "VAO.tcc"
