#pragma once

#include "GLBindableObject.h"
#include "GLObject.h"
#include "Span.h"

namespace egl
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

  using GLBindableObject<BindingType>::GetGLId;

private:
  bool mInitialized = false;
};
}

#include "Buffer.tcc"