#include <catch2/catch.hpp>

#include <kouh/FlatUnorderedSet.hpp>

template <typename Value, typename Pred = std::equal_to<Value>>
using FlatUnorderedSet = kouh::FlatUnorderedSet<Value, Pred>;

namespace
{
struct NoCopy
{
  NoCopy() : a(0)
  {
  }
  NoCopy(int b) : a(b)
  {
  }
  NoCopy(NoCopy const&) = delete;
  NoCopy& operator=(NoCopy const&) = delete;
  NoCopy(NoCopy&&) = default;
  NoCopy& operator=(NoCopy&&) = default;

  // For convenience
  inline bool operator==(int b) const noexcept
  {
    return this->a == b;
  }
  inline bool operator==(NoCopy const& b) const noexcept
  {
    return this->a == b.a;
  }

  int a;
};
}

TEST_CASE("[FlatUnorderedSet] Initialization", "[FlatUnorderedSet]")
{
  SECTION("Empty")
  {
    FlatUnorderedSet<int> fus{};
    CHECK(fus.size() == 0);
    CHECK(fus.empty());
  }

  SECTION("Init list")
  {
    FlatUnorderedSet<int> fus = {2, 3, 4};
    CHECK(fus.size() == 3);
    CHECK(!fus.empty());
  }

  SECTION("Init list class")
  {
    FlatUnorderedSet<std::string> fus = {"lel", "lol", "lowl", "lul"};
    CHECK(fus.size() == 4);
    CHECK(!fus.empty());
  }
}

TEST_CASE("[FlatUnorderedSet] contains / count", "[FlatUnorderedSet]")
{
  SECTION("Simple class")
  {
    FlatUnorderedSet<std::string> fus = {"4", "8", "42", "1337", "4269"};

    REQUIRE(fus.size() == 5);
    CHECK(fus.contains("4"));
    CHECK(fus.contains("8"));
    CHECK(fus.contains("4269"));
    CHECK(fus.count("4") == 1);
    CHECK(fus.count("8") == 1);
    CHECK(fus.count("4269") == 1);
    CHECK(!fus.contains("foo"));
    CHECK(fus.count("foo") == 0);
  }

  SECTION("Not copyable value")
  {
    FlatUnorderedSet<NoCopy> fus;

    fus.emplace(4);
    fus.emplace(8);
    fus.emplace(1337);
    fus.emplace(4269);
    REQUIRE(fus.size() == 4);

    CHECK(fus.contains(NoCopy{4}));
    CHECK(fus.contains(NoCopy{8}));
    CHECK(fus.contains(NoCopy{4269}));
    CHECK(fus.count(NoCopy{4}) == 1);
    CHECK(fus.count(NoCopy{8}) == 1);
    CHECK(fus.count(NoCopy{4269}) == 1);
    CHECK(!fus.contains(NoCopy{31}));
    CHECK(fus.count(NoCopy{31}) == 0);
  }
}

TEST_CASE("[FlatUnorderedSet] erase", "[FlatUnorderedSet]")
{
  FlatUnorderedSet<int> fus = {4, 8, 42, 1337, 4269};

  SECTION("Erase existing key")
  {
    fus.erase(42);

    CHECK(fus.size() == 4);
    CHECK(fus.contains(4));
    CHECK(fus.contains(8));
    CHECK(fus.contains(1337));
    CHECK(fus.contains(4269));
    CHECK(!fus.contains(42));
    CHECK(!fus.contains(31));
  }

  SECTION("Erase non-existing key")
  {
    fus.erase(31);

    CHECK(fus.size() == 5);
    CHECK(fus.contains(4));
    CHECK(fus.contains(8));
    CHECK(fus.contains(42));
    CHECK(fus.contains(1337));
    CHECK(fus.contains(4269));
    CHECK(!fus.contains(31));
  }

  SECTION("Erase last-key")
  {
    fus.erase(4269);

    CHECK(fus.size() == 4);
    CHECK(fus.contains(4));
    CHECK(fus.contains(8));
    CHECK(fus.contains(42));
    CHECK(fus.contains(1337));
    CHECK(!fus.contains(4269));
    CHECK(!fus.contains(31));
  }
}

TEST_CASE("[FlatUnorderedSet] find", "[FlatUnorderedSet]")
{
  FlatUnorderedSet<int> fus = {4, 8, 42, 1337, 4269};

  SECTION("Existing value")
  {
    auto it = fus.find(4);
    CHECK(*it == 4);
  }

  SECTION("Non-existing value")
  {
    auto it = fus.find(31);
    CHECK(it == fus.end());
  }

  SECTION("Existing value with const FlatUnorderedSet")
  {
    auto const& cfus = fus;
    auto it = cfus.find(4);
    CHECK(*it == 4);
  }

  SECTION("Non-existing value with const FlatUnorderedSet")
  {
    auto const& cfus = fus;
    auto it = cfus.find(31);
    CHECK(it == fus.end());
  }
}

TEST_CASE("[FlatUnorderedSet] emplace", "[FlatUnorderedSet]")
{
  SECTION("Simple scalar")
  {
    FlatUnorderedSet<int> fus = {4, 8, 42, 1337, 4269};

    SECTION("Emplace existing data")
    {
      auto const ret = fus.emplace(1337);
      CHECK(ret.first == fus.find(1337));
      CHECK(ret.second == false);
      CHECK(fus.contains(1337));
      CHECK(fus.size() == 5);
    }

    SECTION("Emplace new data")
    {
      auto const ret = fus.emplace(16);
      auto const it = fus.find(16);
      REQUIRE(it != fus.end());
      auto const expected = std::make_pair(it, true);
      CHECK(ret == expected);
      CHECK(fus.contains(16));
      CHECK(fus.size() == 6);
    }
  }

  SECTION("Emplacing non-copyable")
  {
    FlatUnorderedSet<NoCopy> fus;

    fus.emplace(4);
    fus.emplace(8);
    fus.emplace(1337);
    fus.emplace(4269);

    auto const ret = fus.emplace(16);
    auto const it = fus.find(16);
    REQUIRE(it != fus.end());
    auto const expected = std::make_pair(it, true);
    CHECK(ret == expected);
    CHECK(fus.contains(16));
    CHECK(fus.size() == 5);
  }
}
