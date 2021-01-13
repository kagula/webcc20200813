cmake_minimum_required(VERSION 3.10)

PROJECT(webccDemo)
 
#要显示执行构建过程中详细的信息(比如为了得到更详细的出错信息)
SET(CMAKE_VERBOSE_MAKEFILE ON)

#check c++ compiler feature
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX14)

if(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} doesn’t support C++14. Please upgrade or use a different C++ compiler.")
endif()
 
#添加子目录
ADD_SUBDIRECTORY(webcc20200813)
ADD_SUBDIRECTORY(webccDemo)