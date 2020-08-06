#pragma once

#include "ez/Macros.h"
#include <type_traits>

namespace ez
{
// clang-format off
template <typename T> struct RendererFromEStateId { static_assert(!std::is_same_v<T, T>); using Type = void; };
template <typename T> using RendererFromEStateId_t = typename RendererFromEStateId<T>::Type;
// clang-format on

// Renderer state guards
template <auto StateId>
class RendererStateGuard final
{
public:
  using TRenderer = RendererFromEStateId_t<std::remove_pointer_t<std::decay_t<decltype(StateId)>>>;

  RendererStateGuard(TRenderer& ioRenderer) : mRenderer(ioRenderer)
  {
    mRenderer.TRenderer::template GetState().template PushTop<StateId>();
  }
  RendererStateGuard(TRenderer* ioRenderer) : RendererStateGuard(*ioRenderer) {}
  ~RendererStateGuard() { mRenderer.TRenderer::template GetState().template Pop<StateId>(); }

private:
  TRenderer& mRenderer;
};

template <typename TRenderer>
class RendererStateGuardAll final
{
public:
  RendererStateGuardAll(TRenderer& ioRenderer) : mRenderer(ioRenderer) { mRenderer.TRenderer::template PushState(); }
  RendererStateGuardAll(TRenderer* ioRenderer) : RendererStateGuardAll(*ioRenderer) {}
  ~RendererStateGuardAll() { mRenderer.TRenderer::template PopState(); }

private:
  TRenderer& mRenderer;
};

#define RENDERER_STATE_GUARD_ALL(RENDERER_OBJECT)                                                                      \
  RendererStateGuardAll ANONYMOUS_VARIABLE_NAME() { RENDERER_OBJECT };                                                 \
  UNUSED(ANONYMOUS_VARIABLE_NAME());
#define RENDERER_STATE_GUARD(RENDERER_OBJECT, STATE_ID)                                                                \
  RendererStateGuard<STATE_ID> ANONYMOUS_VARIABLE_NAME() { RENDERER_OBJECT };                                          \
  UNUSED(ANONYMOUS_VARIABLE_NAME());
}