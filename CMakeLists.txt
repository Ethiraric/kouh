cmake_minimum_required(VERSION 2.6)

#configuration
project("kouh")

option(KOUH_ENABLE_TESTING "Enable testing of the kouh helpers" ON)

add_library(kouh INTERFACE)
target_include_directories(kouh INTERFACE include)

if(KOUH_ENABLE_TESTING)
  add_subdirectory(tests)
endif()

