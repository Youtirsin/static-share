cmake_minimum_required(VERSION 3.0.0)
project(static_share VERSION 0.1.0)

set(BOOST_INCLUDE_DIR /home/youtirsin/boost_1_80_0)
set(BOOST_LIBRARY_DIR /home/youtirsin/boost_1_80_0/stage/lib)

include_directories(${BOOST_INCLUDE_DIR})
link_directories(${BOOST_LIBRARY_DIR})

add_executable(main main.cpp)
add_executable(test test.cpp)

target_link_libraries(main
  boost_filesystem
  boost_json
  pthread
)

target_link_libraries(test
  boost_filesystem
  boost_json
  pthread
)