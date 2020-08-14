#pragma once

#include "ez/Quat.h"
#include "ez/Segment.h"
#include "ez/Vec.h"
#include "ez/Window.h"
#include <array>

namespace ez
{
template <std::size_t N, std::size_t NumSegments>
class TestSegmentController : public InputListener
{
public:
  static constexpr auto MinLength = 0.1f;

  TestSegmentController()
  {
    if constexpr (N == 2)
    {
      mSegmentRotation = 0.0f;
    }
    else
    {
      mSegmentRotation = AngleAxis(QuarterCircleRads<float>(), Up<Vecf<N>>());
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
            mSegmentLength += delta_length;
          if (key_event.mKey == EKey::S)
            mSegmentLength = std::max(mSegmentLength - delta_length, MinLength);
        }
        else if (key_event.IsShiftModifierPressed())
        {
          const auto delta_angle = QuarterCircleRads<float>() * 0.05f;
          if constexpr (N == 2)
          {
            if (key_event.mKey == EKey::A)
              mSegmentRotation += delta_angle;
            if (key_event.mKey == EKey::D)
              mSegmentRotation -= delta_angle;
          }
          else
          {
            if (key_event.mKey == EKey::A)
              mSegmentRotation *= AngleAxis(delta_angle, Up<Vec3f>());
            if (key_event.mKey == EKey::D)
              mSegmentRotation *= AngleAxis(delta_angle, -Up<Vec3f>());
            if (key_event.mKey == EKey::S)
              mSegmentRotation *= AngleAxis(delta_angle, Forward<Vec3f>());
            if (key_event.mKey == EKey::W)
              mSegmentRotation *= AngleAxis(delta_angle, -Forward<Vec3f>());
            if (key_event.mKey == EKey::Q)
              mSegmentRotation *= AngleAxis(delta_angle, Right<Vec3f>());
            if (key_event.mKey == EKey::E)
              mSegmentRotation *= AngleAxis(delta_angle, -Right<Vec3f>());
          }
        }
        else
        {
          const auto delta_displacement = 0.1f;
          if (key_event.mKey == EKey::A)
            mSegmentTranslation += delta_displacement * Left<Vecf<N>>();
          if (key_event.mKey == EKey::D)
            mSegmentTranslation += delta_displacement * Right<Vecf<N>>();

          if constexpr (N == 2)
          {
            if (key_event.mKey == EKey::S)
              mSegmentTranslation += delta_displacement * Down<Vecf<N>>();
            if (key_event.mKey == EKey::W)
              mSegmentTranslation += delta_displacement * Up<Vecf<N>>();
          }
          else
          {
            if (key_event.mKey == EKey::E)
              mSegmentTranslation += delta_displacement * Down<Vecf<N>>();
            if (key_event.mKey == EKey::Q)
              mSegmentTranslation += delta_displacement * Up<Vecf<N>>();
            if (key_event.mKey == EKey::W)
              mSegmentTranslation += delta_displacement * Forward<Vecf<N>>();
            if (key_event.mKey == EKey::S)
              mSegmentTranslation += delta_displacement * Back<Vecf<N>>();
          }
        }
      }
    }
  }

  std::array<Segment<float, N>, NumSegments> GetSegments() const
  {
    std::array<Segment<float, N>, NumSegments> segments;
    for (int i = 0; i < segments.size(); ++i)
    {
      const auto f = (float(i) / segments.size());
      if constexpr (N == 2)
      {
        const auto segment_local_rotation = (f * FullCircleRads<float>());
        const auto segment_rotation = Rotated(mSegmentRotation, segment_local_rotation);
        segments[i]
            = Segment<float, N> { mSegmentTranslation + Rotated((MinLength * Right<Vecf<N>>()), segment_rotation),
                mSegmentTranslation + Rotated((mSegmentLength * Right<Vecf<N>>()), segment_rotation) };
      }
      else
      {
        const auto segment_local_rotation = AngleAxis(f * FullCircleRads<float>(), Right<Vecf<N>>());
        const auto segment_rotation = Rotated(mSegmentRotation, segment_local_rotation);
        segments[i]
            = Segment<float, N> { mSegmentTranslation + Rotated((MinLength * Forward<Vecf<N>>()), segment_rotation),
                mSegmentTranslation + Rotated((mSegmentLength * Forward<Vecf<N>>()), segment_rotation) };
      }
    }
    return segments;
  }

  bool IsControlEnabled() const { return mControlEnabled; }

private:
  bool mControlEnabled = false;

  RotationType_t<float, N> mSegmentRotation;
  Vecf<N> mSegmentTranslation = Zero<Vecf<N>>();
  float mSegmentLength = 1.0f;
};
}