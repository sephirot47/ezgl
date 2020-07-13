#pragma once

#include "ez/GLBindableObject.h"
#include "ez/GLGuard.h"
#include "ez/MathForward.h"
#include "ez/Vec.h"

namespace ez
{
// clang-format off
template <GL::ETextureTarget TTextureTarget> struct TextureNumDimensions { };
template <> struct TextureNumDimensions<GL::ETextureTarget::TEXTURE_1D> : std::integral_constant<std::size_t, 1> {};
template <> struct TextureNumDimensions<GL::ETextureTarget::TEXTURE_1D_ARRAY> : std::integral_constant<std::size_t, 2> {};
template <> struct TextureNumDimensions<GL::ETextureTarget::TEXTURE_2D> : std::integral_constant<std::size_t, 2> {};
template <> struct TextureNumDimensions<GL::ETextureTarget::TEXTURE_2D_ARRAY> : std::integral_constant<std::size_t, 3> {};
template <> struct TextureNumDimensions<GL::ETextureTarget::TEXTURE_2D_MULTISAMPLE> : std::integral_constant<std::size_t, 2> {};
template <> struct TextureNumDimensions<GL::ETextureTarget::TEXTURE_2D_MULTISAMPLE_ARRAY> : std::integral_constant<std::size_t, 2> {};
template <> struct TextureNumDimensions<GL::ETextureTarget::TEXTURE_3D> : std::integral_constant<std::size_t, 3> {};
template <> struct TextureNumDimensions<GL::ETextureTarget::TEXTURE_CUBE_MAP> : std::integral_constant<std::size_t, 3> {};
template <> struct TextureNumDimensions<GL::ETextureTarget::TEXTURE_CUBE_MAP_ARRAY> : std::integral_constant<std::size_t, 4> {};
template <GL::ETextureTarget TTextureTarget>
constexpr auto TextureNumDimensions_v = TextureNumDimensions<TTextureTarget>::value;
// clang-format on

template <GL::ETextureTarget TTextureTarget>
class Texture : public GLBindableObject<GL::GetBindingType<TTextureTarget>()>
{
public:
  static constexpr auto N = TextureNumDimensions_v<TTextureTarget>;
  static constexpr auto TextureTarget = TTextureTarget;
  static constexpr auto BindingType = GL::GetBindingType<TTextureTarget>();

  Texture();
  Texture(const Veci<N>& inSize, const GL::ETextureFormat inFormat);
  Texture(Texture&& ioRHS) noexcept = default;
  virtual ~Texture() override = default;

  void BindToTextureUnit(const GL::Size& inTextureUnit) const;
  void BindImageTexture(const GL::Uint inImageUnit,
      const GL::EAccess inAccess,
      const GL::Int inLevel = 0,
      const GL::Boolean inLayered = false,
      const GL::Int inLayer = 0) const;

  template <typename T>
  void TexImage(const Veci<N>& inSize,
      const GL::ETextureInputFormat inInputFormat,
      const GL::ETextureInputComponentFormat inInputComponentFormat,
      const Span<T>& inData,
      const GL::ETextureFormat inFormat = GL::ETextureFormat::RGBA8,
      const GL::Int inMipMapLevel = 0);

  void TexImageEmpty(const Veci<N>& inSize, const GL::ETextureFormat inFormat = GL::ETextureFormat::RGBA8);

  void
  TextureStorage(const Veci<N>& inSize, const GL::ETextureFormat inTextureFormat, const GL::Size inMipMapLevels = 1);

  template <typename T>
  void TextureSubImage(const Veci<N>& inOffset,
      const Veci<N>& inSize,
      const GL::ETextureInputFormat inInputFormat,
      const GL::EDataType inInputDataType,
      const Span<T>& inData,
      const GL::Int inMipMapLevel = 0);

  void CopySubData(const Veci<N>& inSourcePosition,
      const Veci<N>& inSourceSize,
      Texture<TTextureTarget>& ioDestinationTexture,
      const Veci<N>& inDestinationPosition,
      const GL::Int inSourceLevel = 0,
      const GL::Int inDestinationLevel = 0) const;

  void SetWrapMode(const GL::EWrapMode inWrapMode);
  void SetWrapModeR(const GL::EWrapMode inWrapMode);
  void SetWrapModeS(const GL::EWrapMode inWrapMode);
  void SetWrapModeT(const GL::EWrapMode inWrapMode);

  void SetMinFilterMode(const GL::EMinFilterMode inMinFilterMode);
  void SetMagFilterMode(const GL::EMagFilterMode inMagFilterMode);

  void GenerateMipMap();

  GL::ETextureFormat GetFormat() const { return mFormat; }

  using GLBindableObject<BindingType>::GetGLId;
  GL::ETextureTarget GetTextureTarget() const { return TTextureTarget; }

  void Resize(const Veci<N>& inSize);
  const Veci<N>& GetSize() const { return mSize; }

protected:
  Veci<N> mSize = Zero<Veci<N>>();
  GL::ETextureFormat mFormat = GL::ETextureFormat::RED;
};

}

#include "ez/Texture.tcc"