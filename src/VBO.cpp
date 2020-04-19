#include "VBO.h"
#include "GL.h"
#include "Macros.h"

namespace egl
{
VBO::VBO() : Buffer(GL::EBufferType::ARRAY_BUFFER) {}
}