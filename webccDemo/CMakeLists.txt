cmake_minimum_required(VERSION 3.10)

#把编译出来的可执行文件输出到项目的bin目录下
#例如运行“cmake ..”的目录为build，则在build/bin目录下生成
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
 
#CMake运行时，打印路径
MESSAGE(${PROJECT_SOURCE_DIR}/webccDemo)

#查找第三方依赖库
find_package(Boost 1.66 COMPONENTS system filesystem thread date_time program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS} /usr/local/include/cjson/) 
LINK_DIRECTORIES(/usr/local/lib)

find_package(ZLIB)
 
#添加库文件搜索路径
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib /usr/lib64 /usr/local/lib64)

#用于将当前目录下的所有源文件的名字保存在变量 DIR_SRCS 中
aux_source_directory(. DIR_SRCS)    

#用于指定从一组源文件 source1 source2 … sourceN(在变量DIR_SRCS中定义) 编译出一个可执行文件且命名为CMake_Tutorial1
add_executable(webccDemo ${DIR_SRCS})  


TARGET_LINK_LIBRARIES(webccDemo ${Boost_LIBRARIES} webcc20200813 z cjson)