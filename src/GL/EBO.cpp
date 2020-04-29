#include "ezgl/EBO.h"
#include "ezgl/GL.h"
#include "ezgl/Macros.h"

namespace egl
{
EBO::EBO() : Buffer<GL::EBufferType::EBO>() {}
}