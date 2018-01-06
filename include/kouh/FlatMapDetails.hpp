#ifndef KOUH_FLATMAPDETAILS_HPP_
#define KOUH_FLATMAPDETAILS_HPP_

#ifndef KOUH_FLATMAP_HPP_
#error "Never include this file directly. Use FlatMap.hpp"
#endif

#include <algorithm>
#include <stdexcept>

namespace kouh
{
template <typename KeyType, typename ValueType, typename Comp>
FlatMap<KeyType, ValueType, Comp>::FlatMap() noexcept
{
}

template <typename KeyType, typename ValueType, typename Comp>
FlatMap<KeyType, ValueType, Comp>::FlatMap(std::initializer_list<PairType> l)
  : container(l)
{
  std::sort(container.begin(),
            container.end(),
            [this](PairType const& pair1, PairType const& pair2) {
              return this->comp(pair1.first, pair2.first);
            });
}

template <typename KeyType, typename ValueType, typename Comp>
typename FlatMap<KeyType, ValueType, Comp>::size_type
FlatMap<KeyType, ValueType, Comp>::size() const noexcept
{
  return this->container.size();
}

template <typename KeyType, typename ValueType, typename Comp>
bool FlatMap<KeyType, ValueType, Comp>::empty() const noexcept
{
  return this->container.empty();
}

template <typename KeyType, typename ValueType, typename Comp>
typename FlatMap<KeyType, ValueType, Comp>::iterator
FlatMap<KeyType, ValueType, Comp>::begin() noexcept
{
  return this->container.begin();
}

template <typename KeyType, typename ValueType, typename Comp>
typename FlatMap<KeyType, ValueType, Comp>::iterator
FlatMap<KeyType, ValueType, Comp>::end() noexcept
{
  return this->container.end();
}

template <typename KeyType, typename ValueType, typename Comp>
typename FlatMap<KeyType, ValueType, Comp>::const_iterator
FlatMap<KeyType, ValueType, Comp>::begin() const noexcept
{
  return this->container.begin();
}

template <typename KeyType, typename ValueType, typename Comp>
typename FlatMap<KeyType, ValueType, Comp>::const_iterator
FlatMap<KeyType, ValueType, Comp>::end() const noexcept
{
  return this->container.end();
}

template <typename KeyType, typename ValueType, typename Comp>
typename FlatMap<KeyType, ValueType, Comp>::const_iterator
FlatMap<KeyType, ValueType, Comp>::cbegin() const noexcept
{
  return this->container.begin();
}

template <typename KeyType, typename ValueType, typename Comp>
typename FlatMap<KeyType, ValueType, Comp>::const_iterator
FlatMap<KeyType, ValueType, Comp>::cend() const noexcept
{
  return this->container.end();
}

template <typename KeyType, typename ValueType, typename Comp>
typename FlatMap<KeyType, ValueType, Comp>::iterator
FlatMap<KeyType, ValueType, Comp>::find(KeyType const& key) noexcept
{
  auto it = this->lowerBound(key);
  if (it != this->container.end() && this->isKeyEqual(key, it->first))
    return it;
  return this->end();
}

template <typename KeyType, typename ValueType, typename Comp>
typename FlatMap<KeyType, ValueType, Comp>::const_iterator
FlatMap<KeyType, ValueType, Comp>::find(KeyType const& key) const noexcept
{
  auto it = this->lowerBound(key);
  if (it != this->container.end() && this->isKeyEqual(key, it->first))
    return it;
  return this->end();
}

template <typename KeyType, typename ValueType, typename Comp>
ValueType& FlatMap<KeyType, ValueType, Comp>::operator[](KeyType const& key)
{
  auto it = this->lowerBound(key);
  if (it != this->container.end() && this->isKeyEqual(key, it->first))
    return it->second;
  return this->container.emplace(it, key, ValueType{})->second;
}

template <typename KeyType, typename ValueType, typename Comp>
ValueType& FlatMap<KeyType, ValueType, Comp>::at(KeyType const& key)
{
  auto it = this->lowerBound(key);
  if (it != this->container.end() && this->isKeyEqual(key, it->first))
    return it->second;
  throw std::out_of_range("Invalid access at FlatMap::at");
}

template <typename KeyType, typename ValueType, typename Comp>
ValueType const& FlatMap<KeyType, ValueType, Comp>::at(KeyType const& key) const
{
  auto it = this->lowerBound(key);
  if (it != this->container.end() && this->isKeyEqual(key, it->first))
    return it->second;
  throw std::out_of_range("Invalid access at FlatMap::at const");
}

template <typename KeyType, typename ValueType, typename Comp>
typename FlatMap<KeyType, ValueType, Comp>::iterator
FlatMap<KeyType, ValueType, Comp>::erase(KeyType const& key) noexcept
{
  auto it = this->lowerBound(key);
  if (it != this->container.end() && this->isKeyEqual(key, it->first))
    return this->container.erase(it);
  return this->container.end();
}

template <typename KeyType, typename ValueType, typename Comp>
typename FlatMap<KeyType, ValueType, Comp>::iterator
FlatMap<KeyType, ValueType, Comp>::erase(iterator it) noexcept
{
  return this->container.erase(it);
}

template <typename KeyType, typename ValueType, typename Comp>
void FlatMap<KeyType, ValueType, Comp>::clear() noexcept
{
  this->container.clear();
}

template <typename KeyType, typename ValueType, typename Comp>
template <typename... Args>
std::pair<typename FlatMap<KeyType, ValueType, Comp>::iterator, bool>
FlatMap<KeyType, ValueType, Comp>::emplace(Args&&... args)
{
  PairType pair{std::forward<Args>(args)...};
  auto it = this->lowerBound(pair.first);
  // Check if the key is already in there.
  if (it != this->container.end() && this->isKeyEqual(pair.first, it->first))
    return std::make_pair(it, false);
  it = this->container.emplace(it, std::move(pair));
  return std::make_pair(it, true);
}

template <typename KeyType, typename ValueType, typename Comp>
typename FlatMap<KeyType, ValueType, Comp>::ContainerType::iterator
FlatMap<KeyType, ValueType, Comp>::lowerBound(KeyType const& key) noexcept
{
  return std::lower_bound(this->begin(),
                          this->end(),
                          key,
                          [this](PairType const& pair, KeyType const& keyp) {
                            return this->comp(pair.first, keyp);
                          });
}

template <typename KeyType, typename ValueType, typename Comp>
typename FlatMap<KeyType, ValueType, Comp>::ContainerType::const_iterator
FlatMap<KeyType, ValueType, Comp>::lowerBound(KeyType const& key) const noexcept
{
  return std::lower_bound(this->begin(),
                          this->end(),
                          key,
                          [this](PairType const& pair, KeyType const& keyp) {
                            return this->comp(pair.first, keyp);
                          });
}

template <typename KeyType, typename ValueType, typename Comp>
bool FlatMap<KeyType, ValueType, Comp>::isKeyEqual(KeyType const& a,
                                                   KeyType const& b) const
    noexcept
{
  return this->comp(a, b) == false && this->comp(b, a) == false;
}
}

#endif /* !KOUH_FLATMAPDETAILS_HPP_ */
