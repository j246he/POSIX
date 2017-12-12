cmake_minimum_required(VERSION 2.8)
project(ece650-concurrency)


# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(helper)

add_executable(checksum checksum.c )
add_executable(checksum-threaded checksum-threaded.c)
add_executable(checksum-threaded-bug-enhanced checksum-threaded-bug-enhanced.c)
add_executable(processjobs processjobs.c)

## Requires C++ CGI library
#add_executable(jobrequest-web_app jobrequest-web_app.cpp)
target_link_libraries(checksum-threaded ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(checksum-threaded-bug-enhanced ${CMAKE_THREAD_LIBS_INIT})
