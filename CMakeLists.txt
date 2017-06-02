cmake_minimum_required(VERSION 3.6)

project(Shell_cpp_proj)

set(CMAKE_CXX_STANDARD 11)
FIND_PACKAGE(Boost 1.58.0)

set(SOURCE_FILES main.cpp)
set(SOURCE_FILES_LS main_ls.cpp ls.cpp ls.h )
set(SOURCE_FILES_MKDIR main_mkdir.cpp mkdir.cpp mkdir.h)
set(SOURCE_FILES_RM main_rm.cpp rm.cpp)

add_executable(Shell_cpp ${SOURCE_FILES} mkdir.cpp)
add_executable(ls_cpp ${SOURCE_FILES_LS})
add_executable(mkdir_cpp ${SOURCE_FILES_MKDIR})
add_executable(rm_cpp ${SOURCE_FILES_RM})

FIND_PACKAGE( Boost COMPONENTS filesystem program_options regex program_options system REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

target_link_libraries( Shell_cpp ${Boost_LIBRARIES} )
target_link_libraries( ls_cpp ${Boost_LIBRARIES} )
target_link_libraries( mkdir_cpp ${Boost_LIBRARIES} )
target_link_libraries( rm_cpp ${Boost_LIBRARIES} )

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()