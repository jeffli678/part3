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

file (GLOB SOURCES
        src/*.cpp
        src/*.h)

if (APPLE)
#    https://stackoverflow.com/a/24313061
    # NOTE: Don't include the path in MACOSX_BUNDLE_ICON_FILE -- this is the property added to Info.plist
    set(MACOSX_BUNDLE_ICON_FILE icon.icns)

    # And this part tells CMake where to find and install the file itself
    set(part3_ICON ${CMAKE_CURRENT_SOURCE_DIR}/imgs/icon.icns)
    set_source_files_properties(${part3_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    add_executable(part3 MACOSX_BUNDLE ${part3_ICON} ${SOURCES})
    set_target_properties(part3 PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "@loader_path/../Frameworks")
else()
    add_executable(part3 ${SOURCES})
endif()

target_link_libraries(part3 PRIVATE Qt6::Widgets)

if (APPLE)
    if (NOT EXISTS ${CMAKE_SOURCE_DIR}/ffmpeg/Darwin/ffmpeg)
        file(DOWNLOAD https://github.com/jeffli678/ffmpeg-build-script/releases/download/v1.35/ffmpeg ${CMAKE_SOURCE_DIR}/ffmpeg/Darwin/ffmpeg
#            EXPECTED_HASH sha1sum=880d7dcf59a4d7b2113c8e52f729c06a4aedb91a
            )
    endif()

    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(DEPLOYQT_EXECUTABLE NAMES macdeployqt HINTS "${_qt_bin_dir}")

    add_custom_command(TARGET part3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/ffmpeg/${CMAKE_SYSTEM_NAME}
                ${PROJECT_BINARY_DIR}/part3.app/Contents/MacOS/ffmpeg
            COMMAND chmod +x ${PROJECT_BINARY_DIR}/part3.app/Contents/MacOS/ffmpeg/*
            )

    add_custom_target(package
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
            COMMAND ${DEPLOYQT_EXECUTABLE} ${PROJECT_BINARY_DIR}/part3.app
            COMMAND ${CMAKE_COMMAND} -E tar "cf" "part3.zip" --format=zip "${PROJECT_BINARY_DIR}/part3.app")

else()
    add_custom_command(TARGET part3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/ffmpeg/${CMAKE_SYSTEM_NAME}
                ${PROJECT_BINARY_DIR}/ffmpeg)
endif()
