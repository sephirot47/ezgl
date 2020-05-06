#pragma once

#include <cstdint>
#include <stack>
#include <tuple>
#include <type_traits>

namespace ez
{
template <typename TIndexType = std::size_t, typename... TArgs>
class TupleOfStacks
{
public:
  using IndexType = TIndexType;

  using TupleType = std::tuple<std::stack<TArgs>...>;

  template <TIndexType Index>
  using StackType = std::remove_const_t<
      std::remove_reference_t<decltype(std::get<static_cast<std::size_t>(Index)>(std::declval<TupleType>()))>>;

  template <TIndexType Index>
  using StackValueType = StackType<Index>::value_type;

  template <TIndexType Index>
  auto& GetStack();
  template <TIndexType Index>
  const auto& GetStack() const;

  template <TIndexType Index>
  auto& Top();
  template <TIndexType Index>
  const auto& Top() const;

  template <TIndexType Index>
  void Push(const StackValueType<Index>& inValue);

  template <TIndexType Index>
  void Push(StackValueType<Index>&& inValue);

  template <TIndexType Index>
  void Pop();

  template <template <TIndexType Index> typename TFunctor, typename... TExtraFunctorArgs>
  void ApplyToAll(TExtraFunctorArgs&&... inExtraFunctorArgs);

  template <template <TIndexType Index> typename TFunctor, typename... TExtraFunctorArgs>
  void ApplyToAll(TExtraFunctorArgs&&... inExtraFunctorArgs) const;

private:
  TupleType mTupleOfStacks;

  template <template <TIndexType Index> typename TFunctor, TIndexType Index, typename... TExtraFunctorArgs>
  void ApplyToAllRec(TExtraFunctorArgs&&... inExtraFunctorArgs);

  template <template <TIndexType Index> typename TFunctor, TIndexType Index, typename... TExtraFunctorArgs>
  void ApplyToAllRec(TExtraFunctorArgs&&... inExtraFunctorArgs) const;
};

template <typename TIndexType, typename... TArgs>
template <TIndexType Index>
auto& TupleOfStacks<TIndexType, TArgs...>::GetStack()
{
  return std::get<static_cast<std::size_t>(Index)>(mTupleOfStacks);
}

template <typename TIndexType, typename... TArgs>
template <TIndexType Index>
const auto& TupleOfStacks<TIndexType, TArgs...>::GetStack() const
{
  return const_cast<TupleOfStacks&>(*this).GetStack();
}

template <typename TIndexType, typename... TArgs>
template <TIndexType Index>
auto& TupleOfStacks<TIndexType, TArgs...>::Top()
{
  EXPECTS(!GetStack<Index>().empty());
  return GetStack<Index>().top();
}

template <typename TIndexType, typename... TArgs>
template <TIndexType Index>
const auto& TupleOfStacks<TIndexType, TArgs...>::Top() const
{
  return const_cast<TupleOfStacks&>(*this).Top<Index>();
}

template <typename TIndexType, typename... TArgs>
template <TIndexType Index>
void TupleOfStacks<TIndexType, TArgs...>::Push(const StackValueType<Index>& inValue)
{
  GetStack<Index>().push(inValue);
}

template <typename TIndexType, typename... TArgs>
template <TIndexType Index>
void TupleOfStacks<TIndexType, TArgs...>::Push(StackValueType<Index>&& inValue)
{
  GetStack<Index>().push(std::move(inValue));
}

template <typename TIndexType, typename... TArgs>
template <TIndexType Index>
void TupleOfStacks<TIndexType, TArgs...>::Pop()
{
  auto& stack = GetStack<Index>();
  EXPECTS(!stack.empty());
  stack.pop();
}

template <typename TIndexType, typename... TArgs>
template <template <TIndexType Index> typename TFunctor, typename... TExtraFunctorArgs>
void TupleOfStacks<TIndexType, TArgs...>::ApplyToAll(TExtraFunctorArgs&&... inExtraFunctorArgs)
{
  // Start recursive call with index 0
  ApplyToAllRec<TFunctor, static_cast<TIndexType>(0), TExtraFunctorArgs...>(
      std::forward<TExtraFunctorArgs>(inExtraFunctorArgs)...);
}

template <typename TIndexType, typename... TArgs>
template <template <TIndexType Index> typename TFunctor, typename... TExtraFunctorArgs>
void TupleOfStacks<TIndexType, TArgs...>::ApplyToAll(TExtraFunctorArgs&&... inExtraFunctorArgs) const
{
  const_cast<TupleOfStacks&>(*this).ApplyToAll<TFunctor, TExtraFunctorArgs...>(
      std::forward<TExtraFunctorArgs>(inExtraFunctorArgs)...);
}

template <typename TIndexType, typename... TArgs>
template <template <TIndexType Index> typename TFunctor, TIndexType Index, typename... TExtraFunctorArgs>
void TupleOfStacks<TIndexType, TArgs...>::ApplyToAllRec(TExtraFunctorArgs&&... inExtraFunctorArgs)
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

template <typename TIndexType, typename... TArgs>
template <template <TIndexType Index> typename TFunctor, TIndexType Index, typename... TExtraFunctorArgs>
void TupleOfStacks<TIndexType, TArgs...>::ApplyToAllRec(TExtraFunctorArgs&&... inExtraFunctorArgs) const
{
  const_cast<TupleOfStacks&>(*this).ApplyToAllRec<TFunctor, Index, TExtraFunctorArgs...>(
      std::forward<TExtraFunctorArgs>(inExtraFunctorArgs)...);
}
}