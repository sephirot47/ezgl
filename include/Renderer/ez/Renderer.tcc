#include <ez/Macros.h>
#include <ez/Renderer.h>

namespace ez
{
template <auto TEStateIdToGuard>
[[nodiscard]] RendererStateGuard<TEStateIdToGuard> Renderer::GetGuard()
{
  using RendererType = typename RendererStateGuard<TEStateIdToGuard>::TRenderer;
  auto renderer_cast = dynamic_cast<RendererType*>(this);
  if (renderer_cast == nullptr)
    THROW_EXCEPTION("Trying to get an incompatible guard for a Renderer. "
                    "Make sure the State enum is of its type or a parent type.");
  return RendererStateGuard<TEStateIdToGuard> { *renderer_cast };
}
}