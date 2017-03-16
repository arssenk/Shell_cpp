cmake_minimum_required(VERSION 3.6)

project(Shell_cpp)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)

add_executable(Shell_cpp ${SOURCE_FILES})

FIND_PACKAGE( Boost COMPONENTS program_options system REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

target_link_libraries( Shell_cpp ${Boost_LIBRARIES} )