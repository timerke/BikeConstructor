cmake_minimum_required(VERSION 3.1.0)

project(bike LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Gui Sql Widgets REQUIRED)

set(SRC_PATH src)
set(SOURCES
    ${SRC_PATH}/accessory.cpp
    ${SRC_PATH}/basket.cpp
    ${SRC_PATH}/database.cpp
    ${SRC_PATH}/functions.cpp
    ${SRC_PATH}/mainwindow.cpp
    ${SRC_PATH}/main.cpp)

set(HEADERS
    ${SRC_PATH}/accessory.h
    ${SRC_PATH}/basket.h
    ${SRC_PATH}/const.h
    ${SRC_PATH}/database.h
    ${SRC_PATH}/functions.h
    ${SRC_PATH}/mainwindow.h)

add_executable(bike
    ${SRC_PATH}/mainwindow.ui
    ${SOURCES}
    ${HEADERS})

set_target_properties(bike PROPERTIES RUNTIME_OUTPUT_DIRECTORY "./bin")

target_link_libraries(bike Qt5::Core Qt5::Gui Qt5::Sql Qt5::Widgets)