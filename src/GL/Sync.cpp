#include "ez/Sync.h"

namespace ez
{
Sync::Sync() {}

Sync::Sync(Sync&& ioRHS) noexcept { std::swap(mSync, ioRHS.mSync); }

Sync& Sync::operator=(Sync&& ioRHS) noexcept
{
  if (this == &ioRHS)
    return *this;

  DeleteIfNonNull();
  std::swap(mSync, ioRHS.mSync);
  return *this;
}

Sync::~Sync() { DeleteIfNonNull(); }

void Sync::Set()
{
  DeleteIfNonNull();
  mSync = GL::FenceSync();
}

GL::EClientWaitSyncResult Sync::ClientWait(const bool inFlush, const uint64_t inTimeout) const
{
  return GL::ClientWaitSync(mSync, inFlush, inTimeout);
}

GL::EClientWaitSyncResult Sync::SetAndClientWait(const bool inFlush, const uint64_t inTimeout)
{
  Set();
  return ClientWait(inFlush, inTimeout);
}

GL::EClientWaitSyncResult Sync::StaticClientWait(const bool inFlush, const uint64_t inTimeout)
{
  return Sync {}.SetAndClientWait(inFlush, inTimeout);
}

void Sync::DeleteIfNonNull()
{
  if (mSync != 0)
  {
    GL::DeleteSync(mSync);
    mSync = 0;
  }
}

}