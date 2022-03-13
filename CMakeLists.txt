cmake_minimum_required(VERSION 3.16)

project(part3 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

if (APPLE)
    set (CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()

if (APPLE)
    add_executable(part3 MACOSX_BUNDLE
            main.cpp
            mainwindow.cpp mainwindow.h)
else()
    add_executable(part3
            main.cpp
            mainwindow.cpp mainwindow.h)
endif()

target_link_libraries(part3 PRIVATE Qt6::Widgets)

if (APPLE)
    add_custom_command(TARGET part3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/ffmpeg/${CMAKE_SYSTEM_NAME}
            ${PROJECT_BINARY_DIR}/part3.app/Contents/MacOS/ffmpeg)
else()
    add_custom_command(TARGET part3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/ffmpeg/${CMAKE_SYSTEM_NAME}
                ${PROJECT_BINARY_DIR}/ffmpeg)
endif()
