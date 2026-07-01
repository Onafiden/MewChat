cmake_minimum_required(VERSION 4.3.0)
project(MewChat VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network WebSockets Multimedia)

add_executable(MewChat
    src/main.cpp
    src/ui/chatwindow.h
    src/ui/chatwindow.cpp
    resources/resources.qrc
)

target_link_libraries(MewChat PRIVATE Qt6::Widgets Qt6::Network Qt6::WebSockets Qt6::Multimedia)