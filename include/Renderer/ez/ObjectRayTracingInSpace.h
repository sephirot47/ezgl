#pragma once

#include "ez/Mat.h"
#include "ez/ObjectRayTracing.h"

namespace ez
{
struct ObjectRayTracingInSpace
{
  using ValueType = ValueType_t<ObjectRayTracing>;
  static constexpr auto NumDimensions = NumDimensions_v<ObjectRayTracing>;

  ObjectRayTracingInSpace() = default;
  ObjectRayTracingInSpace(const ObjectRayTracing* inDrawable,
      const Mat4f& inWorldTransformation,
      const AABoxf& inBoundingAABox)
      : mDrawable { inDrawable }, mWorldTransformation { inWorldTransformation }, mWorldBoundingAABox {
          inBoundingAABox
        }
  {
  }

  const ObjectRayTracing* mDrawable { nullptr };
  Mat4f mWorldTransformation;
  AABoxf mWorldBoundingAABox;
};

template <EIntersectMode TIntersectMode, typename T, std::size_t N>
auto Intersect(const ObjectRayTracingInSpace& inObjectInSpace, const Ray<T, N>& inRay)
{
  return Intersect<TIntersectMode>(inObjectInSpace.mWorldBoundingAABox, inRay);
}
template <EIntersectMode TIntersectMode, typename T, std::size_t N>
auto Intersect(const Ray<T, N>& inRay, const ObjectRayTracingInSpace& inObjectInSpace)
{
  return Intersect<TIntersectMode, T, N>(inObjectInSpace, inRay);
}

template <EIntersectMode TIntersectMode, typename T, std::size_t N>
auto Intersect(const ObjectRayTracingInSpace& inObjectInSpace, const AAHyperBox<T, N>& inAAHyperBox)
{
  return Intersect<TIntersectMode>(inObjectInSpace.mWorldBoundingAABox, inAAHyperBox);
}
template <EIntersectMode TIntersectMode, typename T, std::size_t N>
auto Intersect(const AAHyperBox<T, N>& inAAHyperBox, const ObjectRayTracingInSpace& inObjectInSpace)
{
  return Intersect<TIntersectMode, T, N>(inObjectInSpace, inAAHyperBox);
}

AABoxf BoundingAAHyperBox(const ObjectRayTracingInSpace& inObjectInSpace)
{
  return inObjectInSpace.mWorldBoundingAABox;
}
}
