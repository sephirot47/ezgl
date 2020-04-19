#include "Transformation.h"
#include "Math.h"

namespace egl
{
Transformation::Transformation(const Vec3f& inPosition) : mPosition(inPosition) {}

Transformation::Transformation(const Vec3f& inPosition, const Quatf& inRotation)
    : mPosition(inPosition), mRotation(inRotation)
{
}

Transformation::Transformation(const Vec3f& inPosition, const Quatf& inRotation, const Vec3f& inScale)
    : mPosition(inPosition), mRotation(inRotation), mScale(inScale)
{
}

void Transformation::SetPosition(const Vec3f& inPosition) { mPosition = inPosition; }
const Vec3f& Transformation::GetPosition() const { return mPosition; }

void Transformation::SetRotation(const Quatf& inRotation) { EXPECTS(IsNormalized(mRotation)); mRotation = inRotation; }
const Quatf& Transformation::GetRotation() const { return mRotation; }

void Transformation::SetScale(const Vec3f& inScale) { mScale = inScale; }
const Vec3f& Transformation::GetScale() const { return mScale; }

Vec3f Transformation::Transformed(const Vec3f& inPoint) { return mPosition + (mRotation * (inPoint * mScale)); }

Vec3f Transformation::InverseTransformed(const Vec3f& inPoint)
{
  return ((-mRotation) * (inPoint - mPosition)) / mScale;
}

Mat4f Transformation::GetMatrix() const
{
  return TranslationMat4(mPosition) * RotationMat4(mRotation) * ScaleMat4(mScale);
}

Mat4f Transformation::GetInverseMatrix() const
{
  return ScaleMat4(1.0f / mScale) * RotationMat4(-mRotation) * TranslationMat4(-mPosition);
}

}