#pragma once

#include <chrono>

using namespace std::chrono_literals;

namespace egl
{
using TimeDuration = std::chrono::duration<float, std::ratio<1, 1>>;
using DeltaTime = TimeDuration;
using TimePoint = std::chrono::system_clock::time_point;

TimePoint Now();
}