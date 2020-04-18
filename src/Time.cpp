#include "Time.h"
#include <chrono>

using namespace std::chrono_literals;

namespace egl
{
TimePoint Now() { return std::chrono::system_clock::now(); }
}