cmake_minimum_required(VERSION 3.16)
project(MediaPlayer
    VERSION 1.0.2
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(ALSA REQUIRED)

add_executable(MediaPlayer
    main.cpp
    mainwindow.cpp
)
target_link_libraries(MediaPlayer PRIVATE 
    Qt6::Widgets
)