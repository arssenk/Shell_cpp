cmake_minimum_required(VERSION 3.6)

project(Shell_cpp_proj)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp )
set(SOURCE_FILES_LS main_ls.cpp ls.cpp ls.h copy.cpp)

add_executable(Shell_cpp ${SOURCE_FILES})
add_executable(ls_cpp ${SOURCE_FILES_LS})

FIND_PACKAGE( Boost COMPONENTS filesystem program_options system REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

target_link_libraries( Shell_cpp ${Boost_LIBRARIES} )
target_link_libraries( ls_cpp ${Boost_LIBRARIES} )