cmake_minimum_required(VERSION 3.5)

project(ENGMemo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 请自行填写 qt 工具链所在路径
# set(CMAKE_PREFIX_PATH "D:/Qt/6.5.0/mingw_64")

# 根目录
set(ROOT_DIR ../)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core5Compat) 
endif()

# 搜寻源文件
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.c* include/*.h*)
file(GLOB_RECURSE qrcs ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)

# ui 文件路径
list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "ui")

# Specify MSVC UTF-8 encoding   
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# 添加源文件
add_executable(${PROJECT_NAME}
    # WIN32 # 若需要控制台调试可以注释这一条 # If you need a terminal for debug, please comment this statement
    ${srcs}
    ${uis}
    ${qrcs}
)

add_subdirectory(vendor)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE vendor)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core5Compat)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE include/component)
target_include_directories(${PROJECT_NAME} PRIVATE vendor/include)