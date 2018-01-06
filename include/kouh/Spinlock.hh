#ifndef KOUH_SPINLOCK_HH_
#define KOUH_SPINLOCK_HH_

#include <atomic>

namespace kouh
{
/** Basic spinlock.
 *
 * Meets the requirements of Lockable.
 *
 * Fast lock using an atomic boolean as a method of synchronizing.
 * The `lock()` method is a while loop trying to acquire the boolean (which
 * means it has high CPU consumption).
 *
 * This must only be used to lock short execution paths.
 */
class Spinlock
{
public:
  Spinlock() noexcept : lock_{false}
  {
  }

  Spinlock(Spinlock const& b) noexcept = delete;
  Spinlock(Spinlock&& b) noexcept = delete;
  ~Spinlock() noexcept = default;

  Spinlock& operator=(Spinlock const& rhs) noexcept = delete;
  Spinlock& operator=(Spinlock&& rhs) noexcept = delete;

  void lock()
  {
    while (!this->try_lock())
      ;
  }

  void unlock() noexcept
  {
    this->lock_.store(UNLOCKED, std::memory_order_release);
  }

  bool try_lock() noexcept
  {
    bool expected = UNLOCKED;
    return this->lock_.compare_exchange_weak(
        expected, true, std::memory_order_acquire);
  }

private:
  static constexpr bool UNLOCKED = false;
  static constexpr bool LOCKED = true;
  std::atomic<bool> lock_;
};
}

#endif /* !KOUH_SPINLOCK_HH_ */
