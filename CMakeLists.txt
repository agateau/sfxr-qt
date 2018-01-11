project(sfxr)
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

set(sfxr2_SRCS
    ui/sfxr2.cpp
    core/synthesizer.cpp
    core/generator.cpp
    core/wavsaver.cpp
    core/sound.cpp
    core/soundplayer.cpp
)

qpropgen(sfxr2_QPROPGEN core/basesound.yaml core/basewavsaver.yaml)

qt5_add_resources(sfxr2_QRC ui/qml.qrc)

add_executable(sfxr2 ${sfxr2_SRCS} ${sfxr2_QPROPGEN} ${sfxr2_QRC})
target_link_libraries(sfxr2 SDL Qt5::Widgets Qt5::Quick)
