#include <catch2/catch.hpp>

#include <string>

#include <kouh/OwningPointerMark.hpp>

using kouh::OwningPointerMark;
using kouh::makeOwningPointerMark;

TEST_CASE("[OwningPointerMark] Initialization", "[OwningPointerMark]")
{
  auto* pint = new int(5);
  OwningPointerMark<int> r1{pint}; // r1 will delete pint

  REQUIRE(r1);
  REQUIRE(r1 == pint);
  CHECK(r1.get() == pint);
  CHECK(!r1.marker());
  CHECK(*r1 == 5);

  OwningPointerMark<int> r2{std::move(r1)};
  CHECK(!r1);
  CHECK(r1.get() == nullptr);
  REQUIRE(r2);
  REQUIRE(r2 == pint);
  CHECK(r2.get() == pint);
  CHECK(!r2.marker());
  CHECK(*r2 == 5);
}

TEST_CASE("[OwningPointerMark] makeOwningPointerMark", "[OwningPointerMark]")
{
  auto rstr = makeOwningPointerMark<std::string>("toto");

  REQUIRE(rstr);
  CHECK(*rstr == "toto");
  CHECK(rstr->size() == 4);
}

TEST_CASE("[OwningPointerMark] setMarker / unsetMarker", "[OwningPointerMark]")
{
  auto rstr = makeOwningPointerMark<std::string>("toto");

  REQUIRE(rstr);
  auto* addr = rstr.get();
  CHECK(*rstr == "toto");
  CHECK(rstr->size() == 4);
  REQUIRE(!rstr.marker());
  rstr.setMarker();
  CHECK(rstr.marker());
  // Setting mark should not alter the address.
  REQUIRE(rstr.get() == addr);
  CHECK(*rstr == "toto");
  CHECK(rstr->size() == 4);
}
