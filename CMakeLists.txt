cmake_minimum_required(VERSION 3.1.0)
project( test.a )

set (CMAKE_CXX_STANDARD 14)

add_executable( test.a
  main.cpp
)

target_link_libraries( test.a
  boost_system
  boost_thread
)
