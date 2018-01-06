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
)

#binary
add_executable(kouh_tests ${SRCS})

