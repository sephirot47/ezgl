#pragma once

#include <ez/AAHyperBox.h>
#include <ez/CommonTypeTraits.h>
#include <ez/DirectionalLight.h>
#include <ez/HyperSphere.h>
#include <ez/MaterialRayTracing.h>
#include <ez/Transformation.h>
#include <variant>

namespace ez
{
class ObjectRayTracing final
{
public:
  using ValueType = float;
  static constexpr auto NumDimensions = 3;

  enum class ETypeId // Order must match between variant and enum
  {
    // Objects
    SPHERE,
    AABOX,

    // Lights
    DIRECTIONAL_LIGHT
  };
  using VariantType = std::variant<Spheref, AABoxf, DirectionalLight>;

  template <ETypeId TTypeId>
  using ObjectType_t = std::decay_t<std::variant_alternative_t<static_cast<int>(TTypeId), VariantType>>;

  template <typename TObject>
  static constexpr auto TypeId_v = static_cast<ETypeId>(VariantIndex_v<VariantType, TObject>);

  template <typename T>
  explicit ObjectRayTracing(const T& inObject) : mObjectVariant { inObject }
  {
  }

  template <typename T>
  explicit ObjectRayTracing(const T& inObject, const MaterialRayTracing& inMaterial)
      : mObjectVariant { inObject }, mMaterial { inMaterial }
  {
  }

  template <ObjectRayTracing::ETypeId TTypeId>
  auto& GetObject()
  {
    EXPECTS(static_cast<int>(TTypeId) == mObjectVariant.index());
    return std::get<static_cast<int>(TTypeId)>(mObjectVariant);
  }

  // clang-format off
  template <ObjectRayTracing::ETypeId TTypeId> const auto& GetObject() const { return const_cast<ObjectRayTracing&>(*this).GetObject<TTypeId>(); }
  template <typename TObject> auto& GetObject() { return GetObject<TypeId_v<TObject>>(); }
  template <typename TObject> const auto& GetObject() const { return GetObject<TypeId_v<TObject>>(); }
  template <typename T> void SetObject(const T& inObject) { mObjectVariant = inObject; }
  // clang-format on

  ObjectRayTracing::ETypeId GetType() const { return static_cast<ETypeId>(mObjectVariant.index()); }

  bool IsDrawable() const { return mObjectVariant.index() < static_cast<int>(ETypeId::DIRECTIONAL_LIGHT); }
  bool IsLight() const { return !IsDrawable(); }

  void SetMaterial(const MaterialRayTracing& inMaterial) { mMaterial = inMaterial; }
  const MaterialRayTracing& GetMaterial() const { return mMaterial; }

  const auto& GetObjectVariant() const { return mObjectVariant; }

private:
  VariantType mObjectVariant = Spheref {};
  MaterialRayTracing mMaterial;
};

template <EIntersectMode TIntersectMode, typename T, std::size_t N>
auto Intersect(const ObjectRayTracing& inObject, const Ray<T, N>& inRay)
{
  const auto intersection_result = std::visit(
      [&](const auto& inObject) {
        using TObject = std::remove_cvref_t<decltype(inObject)>;
        if constexpr (IsLight_v<TObject>)
        {
          assert(!"This should never be called");
          return Intersect<TIntersectMode>(inRay, AABoxf {}); // Dummy value. Must match common return type.
        }
        else
        {
          return Intersect<TIntersectMode>(inRay, inObject);
        }
      },
      inObject.GetObjectVariant());
  return intersection_result;
}

template <EIntersectMode TIntersectMode, typename T, std::size_t N>
auto Intersect(const Ray<T, N>& inRay, const ObjectRayTracing& inObject)
{
  return Intersect<TIntersectMode, T, N>(inObject, inRay);
}

auto BoundingAAHyperBox(const ObjectRayTracing& inObject)
{
  const auto bounding_aabox = std::visit(
      [&](const auto& inObject) {
        using TObject = std::remove_cvref_t<decltype(inObject)>;
        if constexpr (IsLight_v<TObject>)
        {
          assert(!"This should never be called");
          return AABoxf { Zero<Vec3f>(), Zero<Vec3f>() }; // Dummy value. Must match common return type.
        }
        else
        {
          return BoundingAAHyperBox(inObject);
        }
      },
      inObject.GetObjectVariant());
  return bounding_aabox;
}
}

#include "ez/ObjectRayTracing.tcc"