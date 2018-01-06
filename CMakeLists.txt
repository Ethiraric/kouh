cmake_minimum_required(VERSION 2.6)

#configuration
project("kouh")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/)

option(KOUH_ENABLE_TESTING "Enable testing of the kouh helpers" ON)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (default: Debug)" FORCE)
  message(STATUS "Build type not set. Defaulting to Debug.")
else()
  set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Build type (default: Debug)" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wshadow -Wnull-dereference -Wdouble-promotion -Wformat=2 -pedantic")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#includes directories
include_directories(
  include/
)

if(KOUH_ENABLE_TESTING)
  add_subdirectory(tests)
endif()

