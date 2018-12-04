cmake_minimum_required(VERSION 3.12)
project(mp_client)

find_library(PTHREAD_LIBRARY pthread)

include_directories(include)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")

add_executable(mp_client main.cpp)
target_link_libraries(mp_client ${PTHREAD_LIBRARY})