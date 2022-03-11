cmake_minimum_required(VERSION 3.20)
project(webserv)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 98)

add_compile_options(-Wall -Werror -Wextra)

set(EXEC webserv)

include_directories(includes)

file(GLOB SOURCES src/*.cpp src/**/*.cpp)
file(GLOB HEADERS includes/*.hpp includes/**/*.hpp)

message(SOURCES="${SOURCES}")
message(HEADERS="${HEADERS}")

add_executable(${EXEC} ${SOURCES} ${HEADERS})
