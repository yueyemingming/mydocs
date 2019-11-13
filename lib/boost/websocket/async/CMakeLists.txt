cmake_minimum_required( VERSION 3.5 )
project( sync.server )
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE on)

add_executable( ${PROJECT_NAME} server.cpp )

target_link_libraries(
  ${PROJECT_NAME}
  boost_system
  pthread
)
