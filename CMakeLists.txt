cmake_minimum_required(VERSION 3.10)
project(list)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

# Add the list.hpp to the include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Create the executable 'code' from main.cpp
add_executable(code main.cpp)
