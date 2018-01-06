#ifndef KOUH_FLATUNORDEREDSET_HPP_
#define KOUH_FLATUNORDEREDSET_HPP_

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <vector>

namespace kouh
{
/** A flattened associative container.
 *
 * The FlatUnorderedSet stores its elements as keys in a std::vector.
 * This helps improving data locality, but the container's performace is
 * affected by the number of reallocations and copies needed on insertion and
 * deletion.
 *
 * The container otherwise behaves as a standard std::set.
 */
template <typename ValueType, typename Comparator = std::equal_to<ValueType>>
class FlatUnorderedSet
{
public:
  using value_type = ValueType;
  using ContainerType = std::vector<value_type>;
  using size_type = typename ContainerType::size_type;
  using iterator = typename ContainerType::iterator;
  using const_iterator = typename ContainerType::const_iterator;
  using reverse_iterator = typename ContainerType::reverse_iterator;
  using const_reverse_iterator = typename ContainerType::const_reverse_iterator;

  FlatUnorderedSet() noexcept = default;
  FlatUnorderedSet(std::initializer_list<value_type> l) : container{l}
  {
  }
  FlatUnorderedSet(FlatUnorderedSet const& b) = default;
  FlatUnorderedSet(FlatUnorderedSet&& b) noexcept = default;
  ~FlatUnorderedSet() noexcept = default;

  FlatUnorderedSet& operator=(FlatUnorderedSet const& rhs) = default;
  FlatUnorderedSet& operator=(FlatUnorderedSet&& rhs) noexcept = default;

  size_type size() const noexcept
  {
    return this->container.size();
  }
  bool empty() const noexcept
  {
    return this->container.empty();
  }

  iterator begin() noexcept
  {
    return this->container.begin();
  }
  iterator end() noexcept
  {
    return this->container.end();
  }
  const_iterator begin() const noexcept
  {
    return this->container.begin();
  }
  const_iterator end() const noexcept
  {
    return this->container.end();
  }
  const_iterator cbegin() const noexcept
  {
    return this->container.cbegin();
  }
  const_iterator cend() const noexcept
  {
    return this->container.cend();
  }

  iterator find(value_type const& val) noexcept
  {
    return std::find_if(
        this->begin(), this->end(), [&](value_type const& container_value) {
          return this->equals_pred(container_value, val);
        });
  }
  const_iterator find(value_type const& val) const noexcept
  {
    return std::find_if(
        this->begin(), this->end(), [&](value_type const& container_value) {
          return this->equals_pred(container_value, val);
        });
  }
  size_type count(value_type const& val) const noexcept
  {
    if (this->find(val) != this->end())
      return 1;
    return 0;
  }
  bool contains(value_type const& val) const noexcept
  {
    return this->count(val);
  }

  size_type erase(value_type const& val)
  {
    auto const it = this->find(val);
    if (it == this->end())
      return 0;
    this->erase(it);
    return 1;
  }
  iterator erase(iterator it)
  {
    auto const last_it = this->end() - 1;
    if (it != last_it)
    {
      // Move last element where deletion happens.
      auto const idx = it - this->begin();
      *it = std::move(*last_it);
      this->container.pop_back();
      return this->begin() + idx;
    }
    else
    {
      this->container.pop_back();
      return this->end();
    }
  }

  void clear()
  {
    this->container.clear();
  }

  template <typename... Args>
  std::pair<iterator, bool> emplace(Args&&... args)
  {
    this->container.emplace_back(std::forward<Args>(args)...);
    auto const it = this->find(this->container.back());
    // If find does not return an iterator pointing to the last element, this
    // means another one is already in the set. We have to remove the element
    // we inserted.
    if (it != this->end() - 1)
    {
      auto const idx = it - this->begin();
      this->container.pop_back();
      return {this->begin() + idx, false};
    }
    return {this->end() - 1, true};
  }

private:
  ContainerType container;
  Comparator equals_pred;
};
}

#endif /* !KOUH_FLATUNORDEREDSET_HPP_ */
