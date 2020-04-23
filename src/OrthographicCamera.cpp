#include "OrthographicCamera.h"
#include "Macros.h"
#include "Math.h"

namespace egl
{

Mat4f OrthographicCamera::GetProjectionMatrix() const
{
  const auto orthographic_projection_matrix
      = OrthographicMat4(mOrthographicParameters.mMin, mOrthographicParameters.mMax);
  return orthographic_projection_matrix;
}

}