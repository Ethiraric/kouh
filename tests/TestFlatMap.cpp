#include <catch2/catch.hpp>

#include <kouh/FlatMap.hpp>

template <typename Key, typename Value>
using FlatMap = kouh::FlatMap<Key, Value>;

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

  int a;
};
}

TEST_CASE("Initialization", "[FlatMap]")
{
  SECTION("Empty")
  {
    FlatMap<int, int> fm{};
    CHECK(fm.size() == 0);
    CHECK(fm.empty());
  }

  SECTION("Init list")
  {
    FlatMap<int, int> fm = {{2, 5}, {3, 4}, {4, 2}};
    CHECK(fm.size() == 3);
    CHECK(!fm.empty());
  }

  SECTION("Init list class-key")
  {
    FlatMap<std::string, int> fm = {
        {"lel", 5}, {"lol", 6}, {"lowl", 0}, {"lul", 8}};
    CHECK(fm.size() == 4);
    CHECK(!fm.empty());
  }
}

TEST_CASE("operator[]", "[FlatMap]")
{
  SECTION("Simple class-key, scalar-value")
  {
    FlatMap<std::string, int> fm = {
        {"4", 4}, {"8", 8}, {"42", 42}, {"1337", 1337}, {"4269", 4269}};

    SECTION("Reading")
    {
      CHECK(fm["4"] == 4);
      CHECK(fm["8"] == 8);
      CHECK(fm["4269"] == 4269);
      CHECK(fm.size() == 5);
    }

    SECTION("Overwrite")
    {
      fm["4"] = 2;
      CHECK(fm["4"] == 2);
      CHECK(fm["1337"] == 1337);
      CHECK(fm.size() == 5);
    }

    SECTION("New entry")
    {
      fm["1"] = 1;
      CHECK(fm["1"] == 1);
      CHECK(fm["4"] == 4);
      CHECK(fm.size() == 6);
    }

    SECTION("New entry by const&")
    {
      int i = 1;
      int const& ref = i;

      fm["1"] = ref;
      CHECK(fm["1"] == 1);
      CHECK(fm["4"] == 4);
      CHECK(fm.size() == 6);
    }
  }

  SECTION("Not copyable value")
  {
    FlatMap<std::string, NoCopy> fm;

    fm["4"] = NoCopy{4};
    fm["8"] = NoCopy{8};
    fm["1337"] = NoCopy{1337};
    fm["4269"] = NoCopy{4269};
    CHECK(fm.size() == 4);

    SECTION("Reading")
    {
      CHECK(fm["4"] == 4);
      CHECK(fm["8"] == 8);
      CHECK(fm["4269"] == 4269);
      CHECK(fm.size() == 4);
    }

    SECTION("Overwrite")
    {
      fm["4"] = NoCopy{2};
      CHECK(fm["4"] == 2);
      CHECK(fm["1337"] == 1337);
      CHECK(fm.size() == 4);
    }

    SECTION("New entry by &&")
    {
      fm["1"] = NoCopy{1};
      CHECK(fm["1"] == 1);
      CHECK(fm["4"] == 4);
      CHECK(fm.size() == 5);
    }
  }
}

TEST_CASE("at", "[FlatMap]")
{
  SECTION("Simple class-key, scalar-value")
  {
    FlatMap<std::string, int> fm = {
        {"4", 4}, {"8", 8}, {"42", 42}, {"1337", 1337}, {"4269", 4269}};

    SECTION("Reading")
    {
      CHECK(fm.at("4") == 4);
      CHECK(fm.at("8") == 8);
      CHECK(fm.at("4269") == 4269);
    }

    SECTION("Overwrite")
    {
      fm.at("4") = 2;
      CHECK(fm.at("4") == 2);
      CHECK(fm.at("1337") == 1337);
      CHECK(fm.size() == 5);
    }

    SECTION("Throw on out of range")
    {
      CHECK_THROWS_AS(fm.at("312"), std::out_of_range);
      CHECK_THROWS_AS(fm.at("lol"), std::out_of_range);
      CHECK_THROWS_AS(fm.at("blup"), std::out_of_range);
    }
  }

  SECTION("Not copyable value")
  {
    FlatMap<std::string, NoCopy> fm;

    fm["4"] = NoCopy{4};
    fm["8"] = NoCopy{8};
    fm["1337"] = NoCopy{1337};
    fm["4269"] = NoCopy{4269};
    CHECK(fm.size() == 4);

    SECTION("Reading")
    {
      CHECK(fm.at("4") == 4);
      CHECK(fm.at("8") == 8);
      CHECK(fm.at("4269") == 4269);
    }

    SECTION("Overwrite")
    {
      fm["4"] = 2;
      CHECK(fm["4"] == 2);
      CHECK(fm["1337"] == 1337);
      CHECK(fm.size() == 4);
    }

    SECTION("Throw on out of range")
    {
      CHECK_THROWS_AS(fm.at("lol"), std::out_of_range);
      CHECK_THROWS_AS(fm.at("blup"), std::out_of_range);
    }
  }
}

TEST_CASE("erase", "[FlatMap]")
{
  FlatMap<std::string, int> fm = {
      {"4", 4}, {"8", 8}, {"42", 42}, {"1337", 1337}, {"4269", 4269}};

  SECTION("Erase existing key")
  {
    fm.erase("42");

    CHECK(fm.size() == 4);
    CHECK(fm.at("4") == 4);
    CHECK(fm.at("8") == 8);
    CHECK(fm.at("4269") == 4269);
    CHECK_THROWS_AS(fm.at("42"), std::out_of_range);
    CHECK_THROWS_AS(fm.at("blup"), std::out_of_range);
  }

  SECTION("Erase non-existing key")
  {
    fm.erase("1 key that does not exist");

    CHECK(fm.size() == 5);
    CHECK(fm.at("4") == 4);
    CHECK(fm.at("8") == 8);
    CHECK(fm.at("42") == 42);
    CHECK(fm.at("4269") == 4269);
    CHECK_THROWS_AS(fm.at("blup"), std::out_of_range);
  }
}

TEST_CASE("find", "[FlatMap]")
{
  FlatMap<std::string, int> fm = {
      {"4", 4}, {"8", 8}, {"42", 42}, {"1337", 1337}, {"4269", 4269}};

  SECTION("Existing value")
  {
    auto it = fm.find("4");
    CHECK(it->first == "4");
    CHECK(it->second == 4);
  }

  SECTION("Non-existing value")
  {
    auto it = fm.find("foo");
    CHECK(it == fm.end());
  }

  SECTION("Existing value with const FlatMap")
  {
    auto const& cfm = fm;
    auto it = cfm.find("4");
    CHECK(it->first == "4");
    CHECK(it->second == 4);
  }

  SECTION("Non-existing value")
  {
    auto const& cfm = fm;
    auto it = cfm.find("foo");
    CHECK(it == fm.end());
  }
}

TEST_CASE("emplace", "[FlatMap]")
{
  SECTION("Simple class-key, scalar-value")
  {
    FlatMap<std::string, int> fm = {
        {"4", 4}, {"8", 8}, {"42", 42}, {"1337", 1337}, {"4269", 4269}};

    SECTION("Emplace existing data")
    {
      auto const expected = std::make_pair(fm.find("1337"), false);
      CHECK(fm.emplace("1337", 1336) == expected);
      CHECK(fm["1337"] == 1337);
      CHECK(fm.size() == 5);
    }

    SECTION("Emplace new data")
    {
      auto const ret = fm.emplace("16", 16);
      auto const it = fm.find("16");
      REQUIRE(it != fm.end());
      auto const expected = std::make_pair(it, true);
      CHECK(ret == expected);
      CHECK(fm["16"] == 16);
      CHECK(fm.size() == 6);
    }

    SECTION("Using std::pair move operator")
    {
      auto const ret = fm.emplace(std::make_pair("16", 16));
      auto const it = fm.find("16");
      REQUIRE(it != fm.end());
      auto const expected = std::make_pair(it, true);
      CHECK(ret == expected);
      CHECK(fm["16"] == 16);
      CHECK(fm.size() == 6);
    }

    SECTION("Using piecewise_construct")
    {
      auto const ret = fm.emplace(std::piecewise_construct,
                                  std::forward_as_tuple("16"),
                                  std::forward_as_tuple(16));
      auto const it = fm.find("16");
      REQUIRE(it != fm.end());
      auto const expected = std::make_pair(it, true);
      CHECK(ret == expected);
      CHECK(fm["16"] == 16);
      CHECK(fm.size() == 6);
    }
  }

  SECTION("Emplacing non-copyable")
  {
    FlatMap<std::string, NoCopy> fm;

    fm["4"] = NoCopy{4};
    fm["8"] = NoCopy{8};
    fm["1337"] = NoCopy{1337};
    fm["4269"] = NoCopy{4269};

    auto const ret = fm.emplace("16", NoCopy{16});
    auto const it = fm.find("16");
    REQUIRE(it != fm.end());
    auto const expected = std::make_pair(it, true);
    CHECK(ret == expected);
    CHECK(fm["16"] == 16);
    CHECK(fm.size() == 5);
  }
}
