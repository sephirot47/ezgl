#pragma once

#include "ez/AABox.h"
#include "ez/HyperSphere.h"
#include "ez/MaterialRayTracing.h"
#include "ez/Transformation.h"
#include <variant>

namespace ez
{
class DrawableRayTracing final
{
public:
  using ValueType = float;
  static constexpr auto NumDimensions = 3;

  enum class ETypeId // Order must match between variant and enum
  {
    SPHERE,
    AABOX,
  };
  using VariantType = std::variant<Spheref, AABoxf>;

  template <ETypeId TTypeId>
  using ObjectType_t = std::decay_t<std::variant_alternative_t<static_cast<int>(TTypeId), VariantType>>;

  template <typename TObject>
  static constexpr auto TypeId_v = static_cast<ETypeId>(std::declval<VariantType>(std::declval<TObject>()).index());

  template <typename T>
  explicit DrawableRayTracing(const T& inObject, const MaterialRayTracing& inMaterial = {})
      : mObjectVariant { inObject }, mMaterial { inMaterial }
  {
  }

  template <DrawableRayTracing::ETypeId TTypeId>
  auto& GetObject()
  {
    EXPECTS(static_cast<int>(TTypeId) == mObjectVariant.index());
    return std::get<static_cast<int>(TTypeId)>(mObjectVariant);
  }

  // clang-format off
  template <DrawableRayTracing::ETypeId TTypeId> const auto& GetObject() const { return const_cast<DrawableRayTracing&>(*this).GetObject<TTypeId>(); }
  template <typename TObject> auto& GetObject() { return GetObject<TypeId_v<TObject>>(); }
  template <typename TObject> const auto& GetObject() { return GetObject<TypeId_v<TObject>>(); }
  template <typename T> void SetObject(const T& inObject) { mObjectVariant = inObject; }
  // clang-format on

  void SetMaterial(const MaterialRayTracing& inMaterial) { mMaterial = inMaterial; }
  const MaterialRayTracing& GetMaterial() const { return mMaterial; }

  const auto& GetObjectVariant() const { return mObjectVariant; }

private:
  VariantType mObjectVariant;
  MaterialRayTracing mMaterial;
};

template <EIntersectMode TIntersectMode, typename T, std::size_t N>
auto Intersect(const DrawableRayTracing& inDrawable, const Ray<T, N>& inRay)
{
  const auto intersection_result
      = std::visit([&](const auto& inObject) { return Intersect<TIntersectMode>(inObject, inRay); },
          inDrawable.GetObjectVariant());
  return intersection_result;
}
template <EIntersectMode TIntersectMode, typename T, std::size_t N>
auto Intersect(const Ray<T, N>& inRay, const DrawableRayTracing& inDrawable)
{
  return Intersect<TIntersectMode, T, N>(inDrawable, inRay);
}

AABoxf BoundingAAHyperBox(const DrawableRayTracing& inDrawable)
{
  const auto bounding_aabox = std::visit([&](const auto& inDrawable) { return BoundingAAHyperBox(inDrawable); },
      inDrawable.GetObjectVariant());
  return bounding_aabox;
}

template <typename T, std::size_t N>
AABoxf BoundingAAHyperBox(const DrawableRayTracing& inDrawable, const Transformation<T, N>& inTransformation)
{
  const auto bounding_aabox
      = std::visit([&](const auto& inDrawable) { return BoundingAAHyperBox<T, N>(inDrawable, inTransformation); },
          inDrawable.GetObjectVariant());
  return bounding_aabox;
}
}

#include "ez/DrawableRayTracing.tcc"