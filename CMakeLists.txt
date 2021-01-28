cmake_minimum_required(VERSION 3.1.0)

project(bike LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Gui Sql Widgets REQUIRED)

set(SOURCES
    accessory.cpp
    basket.cpp
    database.cpp
    functions.cpp
    mainwindow.cpp
    main.cpp)

set(HEADERS
    accessory.h
    basket.h
    const.h
    database.h
    functions.h
    mainwindow.h)

add_executable(bike
    mainwindow.ui
    ${SOURCES}
    ${HEADERS})

target_link_libraries(bike Qt5::Core Qt5::Gui Qt5::Sql Qt5::Widgets)