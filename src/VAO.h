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
    static GLId GetBoundGLId();

    void AddVBO(const VBO& inVBO, GLId inAttribLocation, const VAOVertexAttrib& inVertexAttrib);
    void SetEBO(const EBO& inEBO);

    void AddVertexAttrib(GLId inAttributeLocation, const VAOVertexAttrib& inVertexAttrib);
    void RemoveVertexAttrib(GLId inAttributeLocation);

    GLId GetGLId() const { return mGLId; }

private:
    GLId mGLId = 0;
};
}

#include "VAO.tcc"
