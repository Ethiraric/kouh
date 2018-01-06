#ifndef KOUH_FLATMAP_HPP_
#define KOUH_FLATMAP_HPP_

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <vector>

namespace kouh
{
/** A flattened associative container.
 *
 * The FlatMap stores its elements as key-value std::pairs in a std::vector.
 * This helps improving data locality, but the container's performace is
 * affected by the number of reallocations and copies needed on insertion and
 * deletion.
 *
 * Keys and values are not in separated containers.
 *
 * The container otherwise behaves as a standard std::map.
 */
template <typename KeyType,
          typename ValueType,
          typename Comp = std::less<KeyType>>
class FlatMap
{
public:
  using PairType = std::pair<KeyType, ValueType>;
  using value_type = PairType;
  using ContainerType = std::vector<PairType>;
  using size_type = typename ContainerType::size_type;
  using iterator = typename ContainerType::iterator;
  using const_iterator = typename ContainerType::const_iterator;
  using reverse_iterator = typename ContainerType::reverse_iterator;
  using const_reverse_iterator = typename ContainerType::const_reverse_iterator;

public:
  FlatMap() noexcept;
  FlatMap(std::initializer_list<PairType> l);

  /// Returns the number of elements in the FlatMap.
  size_type size() const noexcept;
  /// Returns true if there are no elements in the FlatMap, false otherwise.
  bool empty() const noexcept;

  // Iterators
  /// Returns an iterator to the first element in the FlatMap.
  iterator begin() noexcept;
  /// Returns an iterator past the last element in the FlatMap.
  iterator end() noexcept;
  /// Returns a const_iterator to the first element in the FlatMap.
  const_iterator begin() const noexcept;
  /// Returns a const_iterator past the last element in the FlatMap.
  const_iterator end() const noexcept;
  /// Returns a const_iterator to the first element in the FlatMap.
  const_iterator cbegin() const noexcept;
  /// Returns a const_iterator past the last element in the FlatMap.
  const_iterator cend() const noexcept;

  // Lookup
  /** Find the position of the value for given key.
   * Returns end() if no match was found.
   */
  iterator find(KeyType const& key) noexcept;
  /** Find the position of the value for given key.
   * Returns cend() if no match was found.
   */
  const_iterator find(KeyType const& key) const noexcept;
  ValueType& operator[](KeyType const& key);
  ValueType& at(KeyType const& key);
  ValueType const& at(KeyType const& key) const;

  // Modifiers
  /// Removes element whose key is key. Does nothing if key is not found.
  iterator erase(KeyType const& key) noexcept;
  iterator erase(iterator it) noexcept;
  /// Removes every element.
  void clear() noexcept;
  /// In-place insertion.
  template <typename... Args>
  std::pair<iterator, bool> emplace(Args&&... args);

private:
  iterator lowerBound(KeyType const& key) noexcept;
  const_iterator lowerBound(KeyType const& key) const noexcept;
  bool isKeyEqual(KeyType const& a, KeyType const& b) const noexcept;

  ContainerType container;
  Comp comp;
};
}

#include <kouh/FlatMapDetails.hpp>

#endif /* !KOUH_FLATMAP_HPP_ */
