#pragma once

#include <chrono>

using namespace std::chrono_literals;

namespace ez
{
using Seconds = std::chrono::duration<float, std::ratio<1, 1>>;
using DeltaTime = Seconds;
using TimePoint = std::chrono::system_clock::time_point;

TimePoint Now();
}