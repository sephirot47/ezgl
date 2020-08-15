#pragma once

#include "ez/Quat.h"
#include "ez/Vec.h"
#include "ez/Window.h"
#include <array>

namespace ez
{
template <typename TPrimitive, std::size_t NumPrimitives>
class TestPrimitiveController : public InputListener
{
public:
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
          if (key_event.mKey == EKey::W)
            mPrimitiveLength += delta_length;
          if (key_event.mKey == EKey::S)
            mPrimitiveLength = std::max(mPrimitiveLength - delta_length, MinLength);
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
    std::array<TPrimitive, NumPrimitives> primitives;
    for (int i = 0; i < primitives.size(); ++i)
    {
      const auto f = (float(i) / primitives.size());
      if constexpr (N == 2)
      {
        const auto primitive_local_rotation = (f * FullCircleRads<float>());
        const auto primitive_rotation = Rotated(mPrimitiveRotation, primitive_local_rotation);
        primitives[i]
            = TPrimitive { mPrimitiveTranslation + Rotated((MinLength * Right<Vecf<N>>()), primitive_rotation),
                mPrimitiveTranslation + Rotated((mPrimitiveLength * Right<Vecf<N>>()), primitive_rotation) };
      }
      else
      {
        const auto primitive_local_rotation = AngleAxis(f * FullCircleRads<float>(), Right<Vecf<N>>());
        const auto primitive_rotation = Rotated(mPrimitiveRotation, primitive_local_rotation);
        primitives[i]
            = TPrimitive { mPrimitiveTranslation + Rotated((MinLength * Forward<Vecf<N>>()), primitive_rotation),
                mPrimitiveTranslation + Rotated((mPrimitiveLength * Forward<Vecf<N>>()), primitive_rotation) };
      }
    }
    return primitives;
  }

  bool IsControlEnabled() const { return mControlEnabled; }

private:
  bool mControlEnabled = false;

  RotationType_t<float, N> mPrimitiveRotation;
  Vecf<N> mPrimitiveTranslation = Zero<Vecf<N>>();
  float mPrimitiveLength = 1.0f;
};
}