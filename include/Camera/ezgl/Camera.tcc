#include "ezgl/Camera.h"

namespace ez
{

template <typename T, std::size_t N>
void Camera<T, N>::SetRotation(const Camera::RotationType& inRotation)
{
  EXPECTS(IsNormalized(inRotation));
  mTransformation.SetRotation(inRotation);
}

template <typename T, std::size_t N>
void Camera<T, N>::LookAtPoint(const Vec<T, N>& inPointToLookAt, const Vec<T, N>& inUpNormalized)
{
  EXPECTS(IsNormalized(inUpNormalized));
  EXPECTS(inPointToLookAt != GetPosition());
  const auto forward_normalized = Normalized(inPointToLookAt - GetPosition());
  const auto new_rotation = LookInDirection(forward_normalized, inUpNormalized);
  mTransformation.SetRotation(new_rotation);
}

}