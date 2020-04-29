#include "ezgl/VBO.h"
#include "ezgl/GL.h"
#include "ezgl/Macros.h"

namespace egl
{
VBO::VBO() : Buffer<GL::EBufferType::VBO>() {}
}