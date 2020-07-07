#pragma once

#include "ez/GL.h"

namespace ez
{
class Sync
{
public:
  Sync();
  Sync(const Sync&) = delete;
  Sync& operator=(const Sync&) = delete;
  Sync(Sync&& ioRHS) noexcept;
  Sync& operator=(Sync&& ioRHS) noexcept;
  ~Sync();

  void Set();
  GL::EClientWaitSyncResult ClientWait(const bool inFlush = true, const uint64_t inTimeout = Max<uint64_t>()) const;
  GL::EClientWaitSyncResult SetAndClientWait(const bool inFlush = true, const uint64_t inTimeout = Max<uint64_t>());
  static GL::EClientWaitSyncResult StaticClientWait(const bool inFlush = true, const uint64_t inTimeout = Max<uint64_t>());

private:
  GL::Sync mSync = 0;

  void DeleteIfNonNull();
};
}