#pragma once

#include "ez/DrawableRayTracing.h"

namespace ez
{
struct DrawableRayTracingWrapper
{
  using ValueType = ValueType_t<DrawableRayTracing>;
  static constexpr auto NumDimensions = NumDimensions_v<DrawableRayTracing>;

  DrawableRayTracingWrapper() = default;
  DrawableRayTracingWrapper(const DrawableRayTracing* inDrawable,
      const Transformation3f& inWorldTransformation,
      const AABoxf& inBoundingAABox)
      : mDrawable { inDrawable }, mWorldTransformation { inWorldTransformation }, mWorldBoundingAABox {
          inBoundingAABox
        }
  {
  }

  const DrawableRayTracing* mDrawable { nullptr };
  Transformation3f mWorldTransformation;
  AABoxf mWorldBoundingAABox;
};

template <EIntersectMode TIntersectMode, typename T, std::size_t N>
auto Intersect(const DrawableRayTracingWrapper& inDrawableWrapper, const Ray<T, N>& inRay)
{
  return Intersect<TIntersectMode>(inDrawableWrapper.mWorldBoundingAABox, inRay);
}
template <EIntersectMode TIntersectMode, typename T, std::size_t N>
auto Intersect(const Ray<T, N>& inRay, const DrawableRayTracingWrapper& inDrawableWrapper)
{
  return Intersect<TIntersectMode, T, N>(inDrawableWrapper, inRay);
}

template <EIntersectMode TIntersectMode, typename T, std::size_t N>
auto Intersect(const DrawableRayTracingWrapper& inDrawableWrapper, const AAHyperBox<T, N>& inAAHyperBox)
{
  return Intersect<TIntersectMode>(inDrawableWrapper.mWorldBoundingAABox, inAAHyperBox);
}
template <EIntersectMode TIntersectMode, typename T, std::size_t N>
auto Intersect(const AAHyperBox<T, N>& inAAHyperBox, const DrawableRayTracingWrapper& inDrawableWrapper)
{
  return Intersect<TIntersectMode, T, N>(inDrawableWrapper, inAAHyperBox);
}

AABoxf BoundingAAHyperBox(const DrawableRayTracingWrapper& inDrawableWrapper)
{
  return inDrawableWrapper.mWorldBoundingAABox;
}
}
