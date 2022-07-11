cmake_minimum_required(VERSION 3.10)

# set the project name
project(ClashEngine)

# set the c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(ClashEngine
    ../ClashEngine/Engine/Format.hpp
    main.cpp
)