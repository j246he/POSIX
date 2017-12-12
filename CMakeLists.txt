cmake_minimum_required(VERSION 2.8)
project(ece650-concurrency)


# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")

add_executable(checksum checksum.c )
add_executable(checksum-threaded checksum-threaded.c)
add_executable(checksum-threaded-bug-enhanced checksum-threaded-bug-enhanced.c)
add_executable(processjobs processjobs.c)

## Requires C++ CGI library
#add_executable(jobrequest-web_app jobrequest-web_app.cpp)
