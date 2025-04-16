cmake_minimum_required(VERSION 3.4.1)
project(termux-display)
include_directories(${CMAKE_SOURCE_DIR}/termux-display-server/include)
include_directories(${CMAKE_SOURCE_DIR}/termux-display-client/include)
include_directories(${CMAKE_SOURCE_DIR}/termux-display-server/inc)
include_directories(${CMAKE_SOURCE_DIR}/termux-display-client/inc)
add_library(Render
        SHARED
        android.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

target_link_libraries(Render
        log
        EGL
        GLESv3
        android
        termux-display-client
        termux-display-server
)
