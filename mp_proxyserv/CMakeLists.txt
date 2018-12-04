cmake_minimum_required(VERSION 3.12)
project(mp_proxyserv)

set(CMAKE_CXX_STANDARD 14)

add_executable(mp_proxyserv proxy.cpp proxy_parse.cpp proxy_parse.h proxy.h)