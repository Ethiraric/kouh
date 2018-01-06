cmake_minimum_required(VERSION 2.6)

#configuration
project("kouh")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)

#includes directories
include_directories(
  ../include
  ./
)

#sources files
set(SRCS
  main.cpp
  TestFlatMap.cpp
  TestFlatUnorderedSet.cpp
  TestOwningPointerMark.cpp
  TestSpinlock.cpp
)

#binary
add_executable(kouh_tests ${SRCS})
target_link_libraries(kouh_tests pthread)
