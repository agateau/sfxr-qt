project(sfxr)
cmake_minimum_required(VERSION 3.7)

find_package(SDL REQUIRED)
find_package(Qt5 REQUIRED Gui Quick)

include_directories(
    ${SDL_INCLUDE_DIR}
    core
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -std=c++11")

set(sfxr2_SRCS
    ui/sfxr2.cpp
    core/synthesizer.cpp
)

qt5_add_resources(sfxr2_QRC ui/qml.qrc)

add_executable(sfxr2 ${sfxr2_SRCS} ${sfxr2_QRC})
target_link_libraries(sfxr2 SDL Qt5::Gui Qt5::Quick)
