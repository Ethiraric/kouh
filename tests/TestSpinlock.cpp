#include <chrono>
#include <mutex>
#include <thread>

#include <catch2/catch.hpp>

#include <kouh/Spinlock.hh>

using kouh::Spinlock;

TEST_CASE("[Spinlock] Sample exclusion", "[Spinlock]")
{
  Spinlock s;
  int check = 0;

  // Interleave locking and assignments.
  // [A] lock, start thread, check==0, set to 1, unlock
  // [B] lock, check==1, set to 2, unlock
  // [A] join thread, check==2

  s.lock();

  std::thread t{[&]() {
    s.lock();
    REQUIRE(check == 1);
    check = 2;
    s.unlock();
  }};

  // Tested with a 1sec delay here.
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  REQUIRE(check == 0);
  check = 1;
  s.unlock();
  t.join();
  REQUIRE(check == 2);
}

TEST_CASE("[Spinlock] lock_guard", "[Spinlock]")
{
  // Ensure it compiles
  Spinlock s;
  std::lock_guard<Spinlock> g{s};
}
