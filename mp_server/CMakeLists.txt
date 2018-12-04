cmake_minimum_required(VERSION 3.12)
project(mp_server)

find_library(PTHREAD_LIBRARY pthread)

include_directories(include)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")

add_executable(mp_server main.cpp)
target_link_libraries(mp_server ${PTHREAD_LIBRARY})