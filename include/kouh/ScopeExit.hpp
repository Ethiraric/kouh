#ifndef KOUH_SCOPEEXIT_HPP_
#define KOUH_SCOPEEXIT_HPP_

#include <utility>

namespace kouh
{
/** Calls a callback upon destruction.
 *
 * If multiple paths are to do the same task, one can use a ScopeExit to invoke
 * the task handler. The callback is called as soon as the ScopeExit is
 * destructed. Therefore, the callback is called even if a function throws in
 * the meantime.
 *
 * Constraint: The callback must be noexcept. Failure to observe this rule will
 * result in std::terminate to be called.
 */
template <typename Callback>
class ScopeExit
{
public:
  ScopeExit() noexcept = delete;
  explicit ScopeExit(Callback&& c) noexcept : cb{std::move(c)}
  {
  }
  ScopeExit(ScopeExit const& b) noexcept = default;
  ScopeExit(ScopeExit&& b) noexcept = default;
  ~ScopeExit() noexcept
  {
    this->cb();
  }

  ScopeExit& operator=(ScopeExit const& rhs) noexcept = default;
  ScopeExit& operator=(ScopeExit&& rhs) noexcept = default;

private:
  Callback cb;
};

template <typename Callback>
ScopeExit<Callback> makeScopeExit(Callback&& f) noexcept
{
  return ScopeExit<Callback>{std::move(f)};
}
}

#endif /* !KOUH_SCOPEEXIT_HPP_ */
