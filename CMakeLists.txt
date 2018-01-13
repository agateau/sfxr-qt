project(sfxr-qt)
cmake_minimum_required(VERSION 3.7)

find_package(SDL REQUIRED)
find_package(Qt5 REQUIRED Gui Quick Widgets)
include(qpropgen/cmake/qpropgen.cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -std=c++11")

include_directories(
    ${SDL_INCLUDE_DIR}
    core
    ${CMAKE_CURRENT_BINARY_DIR}/core
)

set(SRCS
    ui/main.cpp
    core/synthesizer.cpp
    core/generator.cpp
    core/wavsaver.cpp
    core/sound.cpp
    core/soundplayer.cpp
)

qpropgen(QPROPGEN_SRCS core/basesound.yaml core/basewavsaver.yaml)

qt5_add_resources(QRC_SRCS ui/qml.qrc)

add_executable(sfxr-qt ${SRCS} ${QPROPGEN_SRCS} ${QRC_SRCS})
target_link_libraries(sfxr-qt SDL Qt5::Widgets Qt5::Quick)
