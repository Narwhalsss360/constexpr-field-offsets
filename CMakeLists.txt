cmake_minimum_required(VERSION 3.28.3)

project(ConstexprFieldOffsets CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
    main
    src/main.cpp
)
target_include_directories(
   main
   PUBLIC
   include
)
