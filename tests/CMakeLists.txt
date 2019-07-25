cmake_minimum_required(VERSION 2.6)

#configuration
project("kouh")

set(WARNING_FLAGS
  -Wall
  -Wextra
  -Wshadow
  -Wnon-virtual-dtor
  -pedantic
  -Wold-style-cast
  -Wcast-align
  -Wunused
  -Woverloaded-virtual
  -Wpedantic
  -Wconversion
  -Wsign-conversion
  -Wdouble-promotion
  -Wformat=2
)

add_executable(kouh_tests
  main.cpp
  TestFlatMap.cpp
  TestFlatUnorderedSet.cpp
  TestOwningPointerMark.cpp
  TestSpinlock.cpp
)
target_compile_options(kouh_tests PRIVATE ${WARNING_FLAGS})
target_link_libraries(kouh_tests kouh pthread)
target_include_directories(kouh_tests PRIVATE .)
set_property(TARGET kouh_tests PROPERTY CXX_STANDARD 14)
set_property(TARGET kouh_tests PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET kouh_tests PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
