#include <ez/Time.h>
#include <chrono>

using namespace std::chrono_literals;

namespace ez
{
TimePoint Now() { return std::chrono::system_clock::now(); }
}