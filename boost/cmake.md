# cmakelists.txt使用boost

```cmake
cmake_minimum_required(VERSION 3.13)
project(test)
set(CMAKE_CXX_STANDARD 11)

message("CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
message("CMAKE_ROOT = ${CMAKE_ROOT}")

set(BOOST_ROOT /root/eosio/2.0/src/boost_1_71_0/)

find_package(Boost REQUIRED COMPONENTS system thread)

if (NOT Boost_FOUND)
    message("Not found boost")
endif ()

message("${Boost_INCLUDE_DIRS}")
message("${Boost_LIBRARIES}")

add_executable(test main.cpp)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(test ${Boost_LIBRARIES})
```

