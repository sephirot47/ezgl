#include "ez/Macros.h"
#include "ez/PerspectiveCamera.h"

namespace ez
{
template <typename T>
Ray3<T> PerspectiveCamera<T>::GetViewportRay(const Vec2<T> inViewportPoint) const
{
  EXPECTS(IsBetween(inViewportPoint, Zero<Vec2f>(), One<Vec2f>()));

  const auto ray_origin = Camera<T, 3>::GetPosition();

  const auto viewport_point
      = Map(inViewportPoint, Zero<Vec2<T>>(), One<Vec2<T>>(), All<Vec2<T>>(-1.0), All<Vec2<T>>(1.0));
  const auto ray_dir_in_projection_space = Vec3<T>(viewport_point[0], viewport_point[1], static_cast<T>(0));
  const auto viewport_point_in_near_plane_view_space
      = XYZ(Inverted(GetProjectionMatrix()) * XYZ1(ray_dir_in_projection_space));
  const auto ray_dir_world_space = XYZ(Camera<T, 3>::GetModelMatrix() * XYZ0(viewport_point_in_near_plane_view_space));
  const auto ray_dir_world_space_normalized = NormalizedSafe(ray_dir_world_space);

  const auto ray = Ray3<T>(ray_origin, ray_dir_world_space_normalized);
  return ray;
}
}