#pragma once

#include <ez/AAHyperBox.h>
#include <ez/RenderTarget.h>
#include <ez/RendererStateGuard.h>
#include <ez/RendererStateStacks.h>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <stack>
#include <tuple>
#include <type_traits>

namespace ez
{
class Renderer
{
public:
  template <auto TEStateIdToGuard>
  [[nodiscard]] RendererStateGuard<TEStateIdToGuard> GetGuard();

protected:
  Renderer() = default;
  Renderer(const Renderer& inRHS) = default;
  Renderer& operator=(const Renderer& inRHS) = default;
  Renderer(Renderer&&) = default;
  Renderer& operator=(Renderer&& inRHS) = default;
  virtual ~Renderer() = default;
};
}

#include "ez/Renderer.tcc"