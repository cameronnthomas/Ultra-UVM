# NOTE: Please set appropriate directory for this file to work

cmake_minimum_required(VERSION 2.8)
project( SystemCOpenCV )

#This line is added for the modification made in systemc header. See troubleshoot 
add_definitions(-DUSING_OPENCV)

#please modify the next line and add your path to systemc directory
set (CMAKE_PREFIX_PATH /home/ctrca/systemc)

include_directories(${CMAKE_PREFIX_PATH}/include)
find_library(systemc systemc ${CMAKE_PREFIX_PATH}/lib-linux64)
link_directories(${CMAKE_PREFIX_PATH}/lib-linux64)
set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

file(GLOB SOURCES "*.cpp")
add_executable( SystemCOpenCV ${SOURCES} )
target_link_libraries( SystemCOpenCV ${OpenCV_LIBS} systemc)

