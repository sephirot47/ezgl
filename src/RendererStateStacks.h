#pragma once

#include "TupleOfStacks.h"
#include <memory>
#include <vector>

namespace egl
{
template <typename TRenderer, typename TTupleOfStacks>
class RendererStateStacks final : public TTupleOfStacks
{
public:
  using TEStateId = typename TTupleOfStacks::IndexType;

  template <TEStateId StateId>
  using StackType = typename TTupleOfStacks::template StackType<StateId>;

  template <TEStateId StateId>
  using ValueType = typename TTupleOfStacks::template StackValueType<StateId>;

  using TTupleOfStacks::Pop;
  using TTupleOfStacks::Push;

  RendererStateStacks(TRenderer& ioRenderer) : mRenderer(ioRenderer) { }

  template <TEStateId StateId>
  auto& GetCurrent()
  {
    return TTupleOfStacks::template Top<StateId>();
  }
  template <TEStateId StateId>
  const auto& GetCurrent() const
  {
    return TTupleOfStacks::template Top<StateId>();
  }
  template <TEStateId StateId>
  void PushTop()
  {
    PushTopStateFunctor<StateId> push_top_functor;
    push_top_functor(TTupleOfStacks::template GetStack<StateId>());
  }

  void PushAllTops() { TTupleOfStacks::template ApplyToAll<PushTopStateFunctor>(); }
  void PushAllDefaultValues() { TTupleOfStacks::template ApplyToAll<PushDefaultValueToAllStacksFunctor>(); }
  void PopAll() { TTupleOfStacks::template ApplyToAll<PopStateFunctor>(); }
  void ApplyCurrentState() { TTupleOfStacks::template ApplyToAll<ApplyCurrentStateFunctor>(*this); }

  template <TEStateId StateId>
  void Reset()
  {
    GetCurrent<StateId>() = TRenderer::template GetDefaultValue<StateId>();
  }

  TRenderer& GetRenderer() { return mRenderer; }
  const TRenderer& GetRenderer() const { return mRenderer; }

private:
  TRenderer& mRenderer;

  // ApplyToAll Functors
  template <TEStateId StateId>
  struct PushTopStateFunctor
  {
    void operator()(StackType<StateId>& ioStack) const { return ioStack.push(ioStack.top()); }
  };

  template <TEStateId StateId>
  struct PushDefaultValueToAllStacksFunctor
  {
    void operator()(StackType<StateId>& ioStack) const { ioStack.push(TRenderer::template GetDefaultValue<StateId>()); }
  };

  template <TEStateId StateId>
  struct PopStateFunctor
  {
    void operator()(StackType<StateId>& ioStack) const { ioStack.pop(); }
  };

  template <TEStateId StateId>
  struct ApplyCurrentStateFunctor
  {
    void operator()(const StackType<StateId>& inStack, RendererStateStacks& ioStateStacks) const
    {
      EXPECTS(inStack.size() >= 1);
      TRenderer::template ApplyState<StateId>(inStack.top(), ioStateStacks);
    }
  };
};
}

#include "RendererStateStacks.tcc"