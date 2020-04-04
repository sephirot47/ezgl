#pragma once

#include <cstdint>

#include "GL.h"

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
    bool IsBound() const;
    static GL::Id GetBoundGLId();

    void AddVBO(const VBO& inVBO, GL::Id inAttribLocation, const VAOVertexAttrib& inVertexAttrib);
    void SetEBO(const EBO& inEBO);

    void AddVertexAttrib(GL::Id inAttributeLocation, const VAOVertexAttrib& inVertexAttrib);
    void RemoveVertexAttrib(GL::Id inAttributeLocation);

    GL::Id GetGLId() const { return mGLId; }

private:
    GL::Id mGLId = 0;
};
}

#include "VAO.tcc"
