#pragma once

#include <ez/Macros.h>
#include <ez/Time.h>
#include <string_view>

namespace ez
{
class Chrono
{
public:
  inline static bool PrintEnabled = true;

  Chrono(bool inPrintEnabled = true) : mPrintEnabled { inPrintEnabled } { Restart(); }

  void Restart() { mStart = Now(); }
  Seconds GetEllapsedTime() const { return (Now() - mStart); }

  void Print(const std::string_view inId = "")
  {
    if (mPrintEnabled)
      PRINT(inId << ": " << std::chrono::duration_cast<Seconds>(GetEllapsedTime()).count());
  }
  void PrintAndRestart(const std::string_view inId = "")
  {
    Print(inId);
    Restart();
  }

private:
  TimePoint mStart;
  bool mPrintEnabled = true;
};
}