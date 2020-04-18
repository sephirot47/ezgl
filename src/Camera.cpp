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

void Camera::SetPosition(const Vec3f& inPosition) { mPosition = inPosition; }

void Camera::SetOrientation(const Quatf& inOrientation) { mOrientation = Normalized(inOrientation); }

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
  EXPECTS(inPointToLookAt != mPosition);
  const auto forward_normalized = Normalized(inPointToLookAt - mPosition);
  mOrientation = LookInDirection(forward_normalized, inUpNormalized);
}

const Vec3f& Camera::GetPosition() const { return mPosition; }

const Quatf& Camera::GetOrientation() const { return mOrientation; }

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

Mat4f Camera::GetModelMatrix() const
{
  const auto model_matrix = TranslationMat4(mPosition) * RotationMat4(mOrientation);
  return model_matrix;
}

Mat4f Camera::GetViewMatrix() const
{
  const auto view_matrix = RotationMat4(-mOrientation) * TranslationMat4(-mPosition);
  return view_matrix;
}

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