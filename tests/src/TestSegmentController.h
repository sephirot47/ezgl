#pragma once

#include "ez/Quat.h"
#include "ez/Segment.h"
#include "ez/Vec.h"
#include "ez/Window.h"
#include <array>

namespace ez
{
class TestSegmentController : public InputListener
{
public:
  static constexpr auto MinLength = 0.1f;

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

        if (key_event.IsShiftModifierPressed())
        {
          const auto delta_angle = QuarterCircleRads<float>() * 0.05f;
          if (key_event.mKey == EKey::A)
            mSegmentRotation *= AngleAxis(delta_angle, Up<Vec3f>());
          if (key_event.mKey == EKey::D)
            mSegmentRotation *= AngleAxis(delta_angle, -Up<Vec3f>());
          if (key_event.mKey == EKey::S)
            mSegmentRotation *= AngleAxis(delta_angle, Forward<Vec3f>());
          if (key_event.mKey == EKey::W)
            mSegmentRotation *= AngleAxis(delta_angle, -Forward<Vec3f>());
        }
        else if (key_event.IsAltModifierPressed())
        {
          const auto delta_length = 0.1f;
          if (key_event.mKey == EKey::W)
            mSegmentLength += delta_length;
          if (key_event.mKey == EKey::S)
            mSegmentLength = std::max(mSegmentLength - delta_length, MinLength);
        }
        else
        {
          const auto delta_displacement = 0.1f;
          if (key_event.mKey == EKey::A)
            mSegmentTranslation += delta_displacement * Left<Vec3f>();
          if (key_event.mKey == EKey::D)
            mSegmentTranslation += delta_displacement * Right<Vec3f>();
          if (key_event.mKey == EKey::E)
            mSegmentTranslation += delta_displacement * Down<Vec3f>();
          if (key_event.mKey == EKey::Q)
            mSegmentTranslation += delta_displacement * Up<Vec3f>();
          if (key_event.mKey == EKey::W)
            mSegmentTranslation += delta_displacement * Forward<Vec3f>();
          if (key_event.mKey == EKey::S)
            mSegmentTranslation += delta_displacement * Back<Vec3f>();
        }
      }
    }
  }

  std::array<Segment3f, 128> GetSegments() const
  {
    std::array<Segment3f, 128> segments;
    for (int i = 0; i < segments.size(); ++i)
    {
      const auto f = (float(i) / segments.size());
      const auto segment_local_rotation = AngleAxis(f * FullCircleRads<float>(), Right<Vec3f>());
      const auto segment_rotation = (mSegmentRotation * segment_local_rotation);
      segments[i] = Segment3f { mSegmentTranslation + segment_rotation * (MinLength * Forward<Vec3f>()),
        mSegmentTranslation + segment_rotation * (mSegmentLength * Forward<Vec3f>()) };
    }
    return segments;
  }

  bool IsControlEnabled() const { return mControlEnabled; }

private:
  bool mControlEnabled = false;

  Quatf mSegmentRotation = AngleAxis(QuarterCircleRads<float>(), Up<Vec3f>());
  Vec3f mSegmentTranslation = Zero<Vec3f>();
  float mSegmentLength = 1.0f;
};
}