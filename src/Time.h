#pragma once

#include <chrono>

using namespace std::chrono_literals;

namespace egl
{
using Seconds = std::chrono::duration<float, std::ratio<1, 1>>;
using DeltaTime = Seconds;
}