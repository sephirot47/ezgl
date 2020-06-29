#pragma once

#include "ez/GLBindableObject.h"
#include "ez/GLGuard.h"
#include "ez/GLObject.h"
#include "ez/Span.h"

namespace ez
{
template <GL::EBufferType TBufferType>
class Buffer : public GLBindableObject<GL::GetBindingType<TBufferType>()>
{
public:
  static constexpr auto BufferType = TBufferType;
  static constexpr auto ObjectType = GL::GetObjectType<BufferType>();
  static constexpr auto BindingType = GL::GetBindingType<BufferType>();

  Buffer() = default;
  template <typename T>
  explicit Buffer(const Span<T>& inData);
  Buffer(Buffer&& ioRHS) noexcept = default;
  virtual ~Buffer() = default;

  void BufferDataEmpty(const GL::Size inSizeInBytes);
  template <typename T>
  void BufferData(const Span<T>& inData);
  template <typename T>
  void BufferSubData(const Span<T>& inData, const GL::Size inOffset = 0);
  void* MapBuffer(const GL::EAccess inAccess);
  void UnmapBuffer();

  template <typename T>
  T ReadData();

  using GLBindableObject<BindingType>::GetGLId;

private:
  bool mInitialized = false;
};
}

#include "ez/Buffer.tcc"