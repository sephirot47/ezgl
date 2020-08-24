#pragma once

#include <ez/Camera.h>
#include <ez/OrthographicParameters.h>

namespace ez
{
template <typename T, std::size_t N>
class OrthographicCamera final : public Camera<T, N>
{
public:
  void SetOrthoMin(const Vec<T, N>& inOrthoMin) { GetOrthographicParameters().mMin = inOrthoMin; }
  const Vec<T, N>& GetOrthoMin() const { return GetOrthographicParameters().mMin; }

  void SetOrthoMax(const Vec<T, N>& inOrthoMax) { GetOrthographicParameters().mMax = inOrthoMax; }
  const Vec<T, N>& GetOrthoMax() const { return GetOrthographicParameters().mMax; }

  void SetOrthographicParameters(const OrthographicParameters<T, N>& inOrthographicParameters)
  {
    mOrthographicParameters = inOrthographicParameters;
  }
  OrthographicParameters<T, N>& GetOrthographicParameters() { return mOrthographicParameters; }
  const OrthographicParameters<T, N>& GetOrthographicParameters() const { return mOrthographicParameters; }

  virtual SquareMat<T, N + 1> GetProjectionMatrix() const final override
  {
    const auto orthographic_projection_matrix
        = OrthographicMat(mOrthographicParameters.mMin, mOrthographicParameters.mMax);
    return orthographic_projection_matrix;
  }

private:
  OrthographicParameters<T, N> mOrthographicParameters;
};

template <typename T>
using OrthographicCamera2 = OrthographicCamera<T, 2>;
using OrthographicCamera2f = OrthographicCamera2<float>;

template <typename T>
using OrthographicCamera3 = OrthographicCamera<T, 3>;
using OrthographicCamera3f = OrthographicCamera3<float>;
}