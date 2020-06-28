#include "ez/Macros.h"
#include "ez/PerspectiveCamera.h"

namespace ez
{
template <typename T>
Ray3<T> PerspectiveCamera<T>::GetViewportRay(const Vec2<T>& inViewportPoint) const
{
  EXPECTS(IsBetween(inViewportPoint, Zero<Vec2<T>>(), One<Vec2<T>>()));

  const auto world_ray_origin = Camera<T, 3>::GetPosition();

  const auto viewport_point
      = Map(inViewportPoint, Zero<Vec2<T>>(), One<Vec2<T>>(), All<Vec2<T>>(-1.0), All<Vec2<T>>(1.0));

  const auto z_near = GetZNear();

  const auto half_angle_of_view = GetFullAngleOfView() * 0.5f;
  const auto view_ray_dir_xy = (viewport_point * Vec2<T> { GetAspectRatio(), 1.0f }) * Tan(half_angle_of_view);
  const auto view_ray_dir = Normalized(Vec3<T> { view_ray_dir_xy[0], view_ray_dir_xy[1], -1.0f });
  const auto world_ray_dir = Camera<T, 3>::GetRotation() * view_ray_dir;

  const auto world_ray = Ray3<T> { world_ray_origin, world_ray_dir };
  return world_ray;
}
}