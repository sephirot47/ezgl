#include "Camera.h"
#include "Macros.h"
#include "Math.h"

namespace egl
{
void Camera::SetPosition(const Vec3f& inPosition) { mTransformation.SetPosition(inPosition); }

void Camera::SetRotation(const Quatf& inRotation)
{
  EXPECTS(IsNormalized(inRotation));
  mTransformation.SetRotation(inRotation);
}

void Camera::LookAtPoint(const Vec3f& inPointToLookAt, const Vec3f& inUpNormalized)
{
  EXPECTS(IsNormalized(inUpNormalized));
  EXPECTS(inPointToLookAt != GetPosition());
  const auto forward_normalized = Normalized(inPointToLookAt - GetPosition());
  const auto new_rotation = LookInDirection(forward_normalized, inUpNormalized);
  mTransformation.SetRotation(new_rotation);
}

Vec3f Camera::GetForward() const { return GetRotation() * Forward<Vec3f>(); }

Vec3f Camera::GetRight() const { return GetRotation() * Right<Vec3f>(); }

Vec3f Camera::GetUp() const { return GetRotation() * Up<Vec3f>(); }

Mat4f Camera::GetModelMatrix() const { return mTransformation.GetMatrix(); }

Mat4f Camera::GetViewMatrix() const { return mTransformation.GetInverseMatrix(); }
}