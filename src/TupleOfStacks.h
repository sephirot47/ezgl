#pragma once

#include <cstdint>
#include <stack>
#include <tuple>
#include <type_traits>

namespace egl
{
template <typename TIndexType = std::size_t, typename... TArgs>
class TupleOfStacks
{
public:
  using TupleType = std::tuple<std::stack<TArgs>...>;

  template <TIndexType Index>
  using StackType = std::remove_const_t<
      std::remove_reference_t<decltype(std::get<static_cast<std::size_t>(Index)>(std::declval<TupleType>()))>>;

  template <TIndexType Index>
  using StackValueType = StackType<Index>::value_type;

  template <TIndexType Index>
  auto& GetStack()
  {
    return std::get<static_cast<std::size_t>(Index)>(mTupleOfStacks);
  }

  template <TIndexType Index>
  const auto& GetStack() const
  {
    return const_cast<TupleOfStacks&>(*this).GetStack();
  }

  template <TIndexType Index>
  auto& Top()
  {
    return GetStack<Index>().top();
  }

  template <TIndexType Index>
  const auto& Top() const
  {
    return const_cast<TupleOfStacks&>(*this).Top<Index>();
  }

  template <TIndexType Index>
  void Push(const StackValueType<Index>& inValue)
  {
    GetStack<Index>().push(inValue);
  }

  template <TIndexType Index>
  void Pop()
  {
    auto& stack = GetStack<Index>();
    EXPECTS(stack.size() >= 1);
    stack.pop();
  }

  template <template <TIndexType Index> typename TFunctor, typename... TExtraFunctorArgs>
  void ApplyToAll(TExtraFunctorArgs&&... inExtraFunctorArgs)
  {
    // Start recursive call with index 0
    ApplyToAllRec<TFunctor, static_cast<TIndexType>(0), TExtraFunctorArgs...>(
        std::forward<TExtraFunctorArgs>(inExtraFunctorArgs)...);
  }

  template <template <TIndexType Index> typename TFunctor, typename... TExtraFunctorArgs>
  void ApplyToAll(TExtraFunctorArgs&&... inExtraFunctorArgs) const
  {
    const_cast<TupleOfStacks&>(*this).ApplyToAll<TFunctor, TExtraFunctorArgs...>(
        std::forward<TExtraFunctorArgs>(inExtraFunctorArgs)...);
  }

private:
  TupleType mTupleOfStacks;

  template <template <TIndexType Index> typename TFunctor, TIndexType Index, typename... TExtraFunctorArgs>
  void ApplyToAllRec(TExtraFunctorArgs&&... inExtraFunctorArgs)
  {
    if constexpr (static_cast<std::size_t>(Index) < std::tuple_size_v<TupleType>)
    {
      // Create and call to functor
      TFunctor<Index> functor;
      functor(GetStack<Index>(), std::forward<TExtraFunctorArgs>(inExtraFunctorArgs)...);

      // Recursive call
      ApplyToAllRec<TFunctor, static_cast<TIndexType>(static_cast<std::size_t>(Index) + 1), TExtraFunctorArgs...>(
          std::forward<TExtraFunctorArgs>(inExtraFunctorArgs)...);
    }
  }

  template <template <TIndexType Index> typename TFunctor, TIndexType Index, typename... TExtraFunctorArgs>
  void ApplyToAllRec(TExtraFunctorArgs&&... inExtraFunctorArgs) const
  {
    const_cast<TupleOfStacks&>(*this).ApplyToAllRec<TFunctor, Index, TExtraFunctorArgs...>(
        std::forward<TExtraFunctorArgs>(inExtraFunctorArgs)...);
  }
};
}