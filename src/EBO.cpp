#include "EBO.h"
#include "GL.h"
#include "Macros.h"

namespace egl
{
EBO::EBO() : Buffer<GL::EBindingType::EBO, GL::EBufferType::EBO>() {}
}