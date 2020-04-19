#include "Camera.h"
#include "Macros.h"
#include "Math.h"

namespace egl
{
Camera::Camera()
{
  const PerspectiveParameters default_perspective_parameters;
  SetPerspectiveParameters(default_perspective_parameters);
}

void Camera::SetPosition(const Vec3f& inPosition) { mTransformation.SetPosition(inPosition); }

void Camera::SetRotation(const Quatf& inRotation)
{
  EXPECTS(IsNormalized(inRotation));
  mTransformation.SetRotation(inRotation);
}

void Camera::SetPerspectiveParameters(const PerspectiveParameters& inPerspectiveParameters)
{
  mProjectionParametersVariant = inPerspectiveParameters;
}

void Camera::SetOrthographicParameters(const OrthographicParameters& inOrthographicParameters)
{
  mProjectionParametersVariant = inOrthographicParameters;
}

void Camera::LookAtPoint(const Vec3f& inPointToLookAt, const Vec3f& inUpNormalized)
{
  EXPECTS(IsNormalized(inUpNormalized));
  EXPECTS(inPointToLookAt != GetPosition());
  const auto forward_normalized = Normalized(inPointToLookAt - GetPosition());
  const auto new_rotation = LookInDirection(forward_normalized, inUpNormalized);
  mTransformation.SetRotation(new_rotation);
}

const Vec3f& Camera::GetPosition() const { return mTransformation.GetPosition(); }

const Quatf& Camera::GetRotation() const { return mTransformation.GetRotation(); }

bool Camera::IsOrthographic() const
{
  return (mProjectionParametersVariant.index() == OrthographicParameters::VariantIndex);
}

bool Camera::IsPerspective() const
{
  return (mProjectionParametersVariant.index() == PerspectiveParameters::VariantIndex);
}

OrthographicParameters& Camera::GetOrthographicParameters()
{
  EXPECTS(IsOrthographic());
  return std::get<OrthographicParameters>(mProjectionParametersVariant);
}

PerspectiveParameters& Camera::GetPerspectiveParameters()
{
  EXPECTS(IsPerspective());
  return std::get<PerspectiveParameters>(mProjectionParametersVariant);
}

const OrthographicParameters& Camera::GetOrthographicParameters() const
{
  return const_cast<Camera&>(*this).GetOrthographicParameters();
}

const PerspectiveParameters& Camera::GetPerspectiveParameters() const
{
  return const_cast<Camera&>(*this).GetPerspectiveParameters();
}

Mat4f Camera::GetModelMatrix() const { return mTransformation.GetMatrix(); }

Mat4f Camera::GetViewMatrix() const { return mTransformation.GetInverseMatrix(); }

Mat4f Camera::GetProjectionMatrix() const
{
  if (IsOrthographic())
  {
    const auto& orthographic_parameters = GetOrthographicParameters();
    const auto orthographic_projection_matrix
        = OrthographicMat4(orthographic_parameters.mMin, orthographic_parameters.mMax);
    return orthographic_projection_matrix;
  }
  else
  {
    assert(IsPerspective());
    const auto& perspective_parameters = GetPerspectiveParameters();
    const auto perspective_projection_matrix = PerspectiveMat4(perspective_parameters.mAngleOfViewRads,
        perspective_parameters.mAspectRatio,
        perspective_parameters.mZNear,
        perspective_parameters.mZFar);
    return perspective_projection_matrix;
  }
}

}