#ifndef KOUH_OWNINGPOINTERMARK_HPP_
#define KOUH_OWNINGPOINTERMARK_HPP_

#include <memory>
#include <type_traits>

#include <kouh/Diagnostics.hh>

namespace kouh
{
/** Owning pointer type with embedded boolean marker.
 *
 * Behaves much like a std::unique_ptr.
 * Mark is set to false by default.
 *
 * Uses the lo-bits of the pointer to store the mark.  This means the type must
 * at least have an alignment of 2 (or the pointer will be modified).
 * Breaks strict aliasing.
 */
template <typename T, typename Deleter = std::default_delete<T>>
class OwningPointerMark
{
public:
  using element_type = T;
  using pointer = element_type*;
  using const_pointer = element_type const*;
  using deleter_type = Deleter;

  constexpr OwningPointerMark() noexcept : ptr{nullptr}, deleter{}
  {
  }

  constexpr OwningPointerMark(std::nullptr_t) noexcept : ptr{nullptr}, deleter{}
  {
  }

  constexpr OwningPointerMark(pointer p) noexcept : ptr{p}, deleter{}
  {
  }
  
  constexpr OwningPointerMark(pointer p, Deleter d) noexcept
    : ptr{p}, deleter{std::move(d)}
  {
  }

  OwningPointerMark(OwningPointerMark const& b) noexcept = delete;
  constexpr OwningPointerMark(OwningPointerMark&& b) noexcept
    : ptr{b.ptr}, deleter{b.deleter}
  {
    b.ptr = nullptr;
  }

  ~OwningPointerMark() noexcept
  {
    this->deleter(this->get());
  }

  OwningPointerMark& operator=(OwningPointerMark const& rhs) noexcept = delete;
  constexpr OwningPointerMark& operator=(OwningPointerMark&& rhs) noexcept
  {
    this->ptr = rhs.ptr;
    this->deleter = rhs.deleter;
    rhs.ptr = nullptr;
    return *this;
  }

  constexpr element_type& operator*() noexcept
  {
    return *this->get();
  }

  constexpr element_type const& operator*() const noexcept
  {
    return *this->get();
  }

  constexpr pointer operator->() noexcept
  {
    return this->get();
  }

  constexpr const_pointer operator->() const noexcept
  {
    return this->get();
  }

  constexpr operator pointer() noexcept
  {
    return this->get();
  }

  constexpr operator const_pointer() const noexcept
  {
    return this->get();
  }

  constexpr explicit operator bool() const noexcept
  {
    return this->get() != nullptr;
  }

  constexpr pointer get() noexcept
  {
    return reinterpret_cast<pointer>(this->getIntptr() & ~0x1);
  }

  constexpr const_pointer get() const noexcept
  {
    return reinterpret_cast<pointer>(this->getIntptr() & ~0x1);
  }

  constexpr void setMarker() noexcept
  {
    this->getIntptr() |= 0x1;
  }

  constexpr void unsetMarker() noexcept
  {
    this->getIntptr() &= ~0x1;
  }

  constexpr bool marker() const noexcept
  {
    return (this->getIntptr() & 0x1) == 1;
  }

private:
  DIAGNOSTICS_PUSH()
  DIAGNOSTICS_IGNORE(strict-aliasing)
  constexpr std::intptr_t& getIntptr() noexcept
  {
    return *reinterpret_cast<std::intptr_t*>(&this->ptr);
  }

  constexpr std::intptr_t const& getIntptr() const noexcept
  {
    return *reinterpret_cast<std::intptr_t const*>(&this->ptr);
  }
  DIAGNOSTICS_POP()

  pointer ptr;
  Deleter deleter;
};

template <typename T, typename... Args>
OwningPointerMark<T> makeOwningPointerMark(Args&&... args)
{
  return OwningPointerMark<T>(new T(std::forward<Args>(args)...));
}
}

#endif /* !KOUH_OWNINGPOINTERMARK_HPP_ */
