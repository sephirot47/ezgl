#pragma once

#include "ez/AAHyperBox.h"
#include "ez/Capsule.h"
#include "ez/HyperBox.h"
#include "ez/HyperSphere.h"
#include "ez/MathTypeTraits.h"
#include "ez/Quat.h"
#include "ez/Vec.h"
#include "ez/Window.h"
#include <array>

namespace ez
{
template <typename TPrimitive, std::size_t NumPrimitives = 1>
class TestPrimitiveController : public InputListener
{
public:
  using PrimitiveType = TPrimitive;
  static constexpr auto N = NumComponents_v<TPrimitive>;
  static constexpr auto MinLength = 0.1f;

  TestPrimitiveController()
  {
    if constexpr (N == 2)
    {
      mPrimitiveRotation = 0.0f;
    }
    else
    {
      mPrimitiveRotation = AngleAxis(QuarterCircleRads<float>(), Up<Vecf<N>>());
    }
  }

  void OnInput(const InputEvent& inInputEvent) override
  {
    if (inInputEvent.GetType() == InputEvent::EType::KEY)
    {
      const auto key_event = inInputEvent.As<InputEvent::EType::KEY>();
      if (key_event.IsPressOrRepeat())
      {
        if (key_event.mKey == EKey::TAB)
          mControlEnabled = !mControlEnabled;

        if (!IsControlEnabled())
          return;

        if (key_event.IsAltModifierPressed())
        {
          const auto delta_length = 0.1f;
          if (key_event.mKey == EKey::D)
            mPrimitiveSize[0] += delta_length;
          else if (key_event.mKey == EKey::A)
            mPrimitiveSize[0] = std::max(mPrimitiveSize[0] - delta_length, MinLength);

          if constexpr (N == 2)
          {
            if (key_event.mKey == EKey::W)
              mPrimitiveSize[1] += delta_length;
            else if (key_event.mKey == EKey::S)
              mPrimitiveSize[1] = std::max(mPrimitiveSize[1] - delta_length, MinLength);
          }
          else
          {
            if (key_event.mKey == EKey::Q)
              mPrimitiveSize[1] += delta_length;
            else if (key_event.mKey == EKey::E)
              mPrimitiveSize[1] = std::max(mPrimitiveSize[0] - delta_length, MinLength);
            else if (key_event.mKey == EKey::W)
              mPrimitiveSize[2] += delta_length;
            else if (key_event.mKey == EKey::S)
              mPrimitiveSize[2] = std::max(mPrimitiveSize[2] - delta_length, MinLength);
          }
        }
        else if (key_event.IsShiftModifierPressed())
        {
          const auto delta_angle = QuarterCircleRads<float>() * 0.05f;
          if constexpr (N == 2)
          {
            if (key_event.mKey == EKey::A)
              mPrimitiveRotation += delta_angle;
            if (key_event.mKey == EKey::D)
              mPrimitiveRotation -= delta_angle;
          }
          else
          {
            if (key_event.mKey == EKey::A)
              mPrimitiveRotation *= AngleAxis(delta_angle, Up<Vec3f>());
            if (key_event.mKey == EKey::D)
              mPrimitiveRotation *= AngleAxis(delta_angle, -Up<Vec3f>());
            if (key_event.mKey == EKey::S)
              mPrimitiveRotation *= AngleAxis(delta_angle, Forward<Vec3f>());
            if (key_event.mKey == EKey::W)
              mPrimitiveRotation *= AngleAxis(delta_angle, -Forward<Vec3f>());
            if (key_event.mKey == EKey::Q)
              mPrimitiveRotation *= AngleAxis(delta_angle, Right<Vec3f>());
            if (key_event.mKey == EKey::E)
              mPrimitiveRotation *= AngleAxis(delta_angle, -Right<Vec3f>());
          }
        }
        else
        {
          const auto delta_displacement = 0.1f;
          if (key_event.mKey == EKey::A)
            mPrimitiveTranslation += delta_displacement * Left<Vecf<N>>();
          if (key_event.mKey == EKey::D)
            mPrimitiveTranslation += delta_displacement * Right<Vecf<N>>();

          if constexpr (N == 2)
          {
            if (key_event.mKey == EKey::S)
              mPrimitiveTranslation += delta_displacement * Down<Vecf<N>>();
            if (key_event.mKey == EKey::W)
              mPrimitiveTranslation += delta_displacement * Up<Vecf<N>>();
          }
          else
          {
            if (key_event.mKey == EKey::E)
              mPrimitiveTranslation += delta_displacement * Down<Vecf<N>>();
            if (key_event.mKey == EKey::Q)
              mPrimitiveTranslation += delta_displacement * Up<Vecf<N>>();
            if (key_event.mKey == EKey::W)
              mPrimitiveTranslation += delta_displacement * Forward<Vecf<N>>();
            if (key_event.mKey == EKey::S)
              mPrimitiveTranslation += delta_displacement * Back<Vecf<N>>();
          }
        }
      }
    }
  }

  std::array<TPrimitive, NumPrimitives> GetPrimitives() const
  {
    using T = ValueType_t<TPrimitive>;
    static constexpr auto N = NumComponents_v<TPrimitive>;

    std::array<TPrimitive, NumPrimitives> primitives;
    for (int i = 0; i < primitives.size(); ++i)
    {
      const auto f = (float(i) / primitives.size());
      Vecf<N> forward_vector;
      RotationType_t<float, N> primitive_local_rotation;
      if constexpr (N == 2)
      {
        primitive_local_rotation = (f * FullCircleRads<float>());
        forward_vector = Right<Vecf<N>>();
      }
      else
      {
        primitive_local_rotation = AngleAxis(f * FullCircleRads<float>(), Right<Vecf<N>>());
        forward_vector = Forward<Vecf<N>>();
      }

      const auto primitive_rotation = Rotated(mPrimitiveRotation, primitive_local_rotation);
      const auto forward_vector_rotated = NormalizedSafe(Rotated(forward_vector, primitive_rotation));
      if constexpr (IsLine_v<TPrimitive>)
      {
        primitives[i] = TPrimitive { mPrimitiveTranslation, forward_vector_rotated };
      }
      else if constexpr (IsRay_v<TPrimitive>)
      {
        primitives[i]
            = TPrimitive { mPrimitiveTranslation + MinLength * forward_vector_rotated, forward_vector_rotated };
      }
      else if constexpr (IsSegment_v<TPrimitive>)
      {
        primitives[i] = TPrimitive { mPrimitiveTranslation + MinLength * forward_vector_rotated,
          mPrimitiveTranslation + (MinLength + mPrimitiveSize[0]) * forward_vector_rotated };
      }
      else if constexpr (IsHyperSphere_v<TPrimitive>)
      {
        primitives[i] = HyperSpheref<N> { mPrimitiveTranslation, mPrimitiveSize[0] };
      }
      else if constexpr (IsAAHyperBox_v<TPrimitive>)
      {
        primitives[i] = MakeAAHyperBoxFromCenterSize(mPrimitiveTranslation, mPrimitiveSize);
      }
      else if constexpr (IsHyperBox_v<TPrimitive>)
      {
        primitives[i] = HyperBox<T, N> { mPrimitiveTranslation, mPrimitiveSize * 0.5f, primitive_rotation };
      }
      else if constexpr (IsCapsule_v<TPrimitive>)
      {
        primitives[i] = Capsulef<N> { mPrimitiveTranslation - mPrimitiveSize[0] * 0.5f * forward_vector_rotated,
          mPrimitiveTranslation + mPrimitiveSize[0] * 0.5f * forward_vector_rotated,
          mPrimitiveSize[1] };
      }
    }
    return primitives;
  }

  bool IsControlEnabled() const { return mControlEnabled; }

private:
  bool mControlEnabled = false;

  RotationType_t<float, N> mPrimitiveRotation;
  Vecf<N> mPrimitiveTranslation = Zero<Vecf<N>>();
  Vecf<N> mPrimitiveSize = One<Vecf<N>>();
};
}