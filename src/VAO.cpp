#include "VAO.h"

#include <GL/glew.h>

namespace egl
{
void VAO::AddVertexAttrib(uint32_t inAttribIndex, const VAOVertexAttrib& inVertexAttrib)
{
    glEnableVertexAttribArray(inAttribIndex);
    glVertexAttribPointer(inAttribIndex,
        inVertexAttrib.numComponents,
        static_cast<GLenum>(inVertexAttrib.type),
        inVertexAttrib.normalized,
        inVertexAttrib.stride,
        reinterpret_cast<const void*>(inVertexAttrib.offset));
}

void VAO::RemoveVertexAttrib(uint32_t inAttribIndex)
{
    glDisableVertexAttribArray(inAttribIndex);
}
}
