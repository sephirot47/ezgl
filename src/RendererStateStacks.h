#pragma once

#include "DirectionalLight.h"
#include "GL.h"
#include "Material.h"
#include "PointLight.h"
#include "TupleOfStacks.h"
#include <memory>
#include <vector>

namespace egl
{
enum class ERendererStateId
{
  CAMERA,
  MODEL_MATRIX,
  MATERIAL,
  OVERRIDE_SHADER_PROGRAM,
  RENDER_TEXTURE,
  DEPTH_ENABLED,
  CULL_FACE_ENABLED,
  BLEND_ENABLED,
  BLEND_SOURCE_FACTOR,
  BLEND_DEST_FACTOR,
  POINT_SIZE,
  LINE_WIDTH,
  SCENE_AMBIENT_COLOR,
  DIRECTIONAL_LIGHTS,
  POINT_LIGHTS
};

class Camera;
class ShaderProgram;
class Texture2D;

using RenderStateTupleOfStacks = TupleOfStacks<ERendererStateId,
    std::shared_ptr<Camera>,           // ERendererStateId::CAMERA
    Mat4f,                             // ERendererStateId::MODEL_MATRIX
    Material,                          // ERendererStateId::MATERIAL
    std::shared_ptr<ShaderProgram>,    // ERendererStateId::OVERRIDE_SHADER_PROGRAM
    std::shared_ptr<Texture2D>,        // ERendererStateId::RENDER_TEXTURE
    bool,                              // ERendererStateId::DEPTH_ENABLED
    bool,                              // ERendererStateId::CULL_FACE_ENABLED
    bool,                              // ERendererStateId::BLEND_ENABLED
    GL::EBlendFactor,                  // ERendererStateId::BLEND_SOURCE_FACTOR
    GL::EBlendFactor,                  // ERendererStateId::BLEND_DEST_FACTOR
    float,                             // ERendererStateId::POINT_SIZE
    float,                             // ERendererStateId::LINE_WIDTH
    Color3f,                           // ERendererStateId::SCENE_AMBIENT_COLOR
    std::vector<GLSLDirectionalLight>, // ERendererStateId::DIRECTIONAL_LIGHTS
    std::vector<GLSLPointLight>>;      // ERendererStateId::POINT_LIGHTS

template <typename TRenderer>
class RendererStateStacks : public RenderStateTupleOfStacks
{
public:
  template <ERendererStateId TStateId>
  using StackType = typename RenderStateTupleOfStacks::StackType<TStateId>;
  template <ERendererStateId TStateId>
  using ValueType = typename RenderStateTupleOfStacks::StackValueType<TStateId>;

  using RenderStateTupleOfStacks::Pop;
  using RenderStateTupleOfStacks::Push;

  RendererStateStacks(TRenderer& ioRenderer) : mRenderer(ioRenderer) {}

  template <ERendererStateId TStateId>
  auto& GetCurrent()
  {
    return Top<TStateId>();
  }
  template <ERendererStateId TStateId>
  const auto& GetCurrent() const
  {
    return Top<TStateId>();
  }
  template <ERendererStateId TStateId>
  void PushTop()
  {
    PushTopStateFunctor<TStateId> push_top_functor;
    push_top_functor(GetStack<TStateId>());
  }

  void PushAllTops() { ApplyToAll<PushTopStateFunctor>(); }
  void PushAllDefaultValues() { ApplyToAll<PushDefaultValueToAllStacksFunctor>(); }
  void PopAll() { ApplyToAll<PopStateFunctor>(); }
  void ApplyCurrentState() { ApplyToAll<ApplyCurrentStateFunctor>(*this); }

  template <ERendererStateId TStateId>
  void Reset()
  {
    GetCurrent<TStateId>() = GetDefaultValue<TStateId>();
  }

private:
  TRenderer& mRenderer;

  // ApplyToAll Functors
  template <ERendererStateId TStateId>
  struct PushTopStateFunctor
  {
    void operator()(StackType<TStateId>& ioStack) const { return ioStack.push(ioStack.top()); }
  };

  template <ERendererStateId TStateId>
  struct PushDefaultValueToAllStacksFunctor
  {
    void operator()(StackType<TStateId>& ioStack) const { ioStack.push(GetDefaultValue<TStateId>()); }
  };

  template <ERendererStateId TStateId>
  struct PopStateFunctor
  {
    void operator()(StackType<TStateId>& ioStack) const { ioStack.pop(); }
  };

  template <ERendererStateId TStateId>
  struct ApplyCurrentStateFunctor
  {
    void operator()(const StackType<TStateId>& inStack, RendererStateStacks<TRenderer>& ioStateStacks) const
    {
      EXPECTS(inStack.size() >= 1);
      ApplyState<TStateId>(inStack.top(), ioStateStacks);
    }
  };

  template <ERendererStateId TStateId>
  static void Copy(const ValueType<TStateId>& inValue, ValueType<TStateId>& outCopiedValue);

  template <ERendererStateId TStateId>
  static void ApplyState(const ValueType<TStateId>& inValue, RendererStateStacks<TRenderer>& ioStateStacks);

  // State stack values
  template <ERendererStateId TStateId>
  static ValueType<TStateId> GetDefaultValue();
};
}

#include "RendererStateStacks.tcc"