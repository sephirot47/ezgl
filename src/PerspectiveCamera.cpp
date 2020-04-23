#include "PerspectiveCamera.h"
#include "Macros.h"
#include "Math.h"

namespace egl
{

Mat4f PerspectiveCamera::GetProjectionMatrix() const
{
  const auto perspective_projection_matrix = PerspectiveMat4(mPerspectiveParameters.mAngleOfView,
      mPerspectiveParameters.mAspectRatio,
      mPerspectiveParameters.mZNear,
      mPerspectiveParameters.mZFar);
  return perspective_projection_matrix;
}

}