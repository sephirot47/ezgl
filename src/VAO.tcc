#include "VAO.h"

#include "VAOVertexAttrib.h"

namespace egl
{
template <typename T>
void VAO::AddVertexAttrib(uint32_t inAttribIndex, bool inNormalized, uint32_t inOffset)
{
    VAOVertexAttribT<T> vertex_attrib;
    vertex_attrib.normalized = inNormalized;
    vertex_attrib.offset = inOffset;
    AddVertexAttrib(inAttribIndex, vertex_attrib);
}
}