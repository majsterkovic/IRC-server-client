cmake_minimum_required(VERSION 3.16)
project(irc_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(irc_server main.cpp Server.h Server.cpp Logger.cpp Logger.h Client.cpp Client.cpp)
target_link_libraries(irc_server pthread)