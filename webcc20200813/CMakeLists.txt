cmake_minimum_required(VERSION 3.10)

MESSAGE(STATUS "This is WEBCC_SOURCE_DIR="${webccDemo_SOURCE_DIR})
MESSAGE(STATUS "This is CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR})

#用于将当前目录下的所有源文件的名字保存在变量 LIB1_SRCS 中
AUX_SOURCE_DIRECTORY(. LIB_SRC)

add_definitions(-DASIO_NO_DEPRECATED)

#查找第三方依赖库
find_package(Boost 1.66 COMPONENTS filesystem thread date_time REQUIRED)
include_directories(${Boost_INCLUDE_DIRS}) 
LINK_DIRECTORIES(/usr/local/lib)

find_package(ZLIB)

#把编译出来的库文件输出到项目的lib目录下
#例如运行“cmake ..”的目录为build，则在build/lib目录下生成
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
 
#output library name is  libHttpClient.so
ADD_LIBRARY(webcc20200813 STATIC  ${LIB_SRC})
