cmake_minimum_required(VERSION 3.16)
project(MediaPlayer
    VERSION 1.1.0
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(PkgConfig REQUIRED)
pkg_check_modules(ALSA REQUIRED alsa)

add_executable(MAXMediaPlayer
    main.cpp
    src/ui/mainwindow.cpp
    src/ui/mainwindow.hpp
    src/ui/additive.cpp
    src/ui/additive.hpp
    src/audio/audiobrain.cpp
    src/audio/audiobrain.hpp
)
target_include_directories(MAXMediaPlayer PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${ALSA_INCLUDE_DIRS}
)
target_link_libraries(MAXMediaPlayer PRIVATE
    Qt6::Widgets
    ${ALSA_LIBRARIES}
    pthread
)