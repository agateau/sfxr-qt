project(sfxr-qt)
cmake_minimum_required(VERSION 3.7)

find_package(SDL REQUIRED)
find_package(Qt5 REQUIRED Gui Quick Widgets)
include(3rdparty/qpropgen/cmake/qpropgen.cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -std=c++11")

include_directories(
    ${SDL_INCLUDE_DIR}
    core
    ${CMAKE_CURRENT_BINARY_DIR}/core
)

set(SRCS
    ui/main.cpp
    core/Synthesizer.cpp
    core/Generator.cpp
    core/NoiseGenerator.cpp
    core/WavSaver.cpp
    core/Sound.cpp
    core/SoundPlayer.cpp
    core/SoundListModel.cpp
)

qpropgen(QPROPGEN_SRCS
    core/BaseSound.yaml
    core/BaseSoundListModel.yaml
    core/BaseWavSaver.yaml
)

qt5_add_resources(QRC_SRCS ui/qml.qrc)

add_executable(sfxr-qt ${SRCS} ${QPROPGEN_SRCS} ${QRC_SRCS})
target_link_libraries(sfxr-qt SDL Qt5::Widgets Qt5::Quick)

# Installation
install(TARGETS sfxr-qt RUNTIME DESTINATION bin)

install(FILES ui/icons/sfxr-qt.desktop
    DESTINATION share/applications
    RENAME com.agateau.sfxr-qt.desktop
)

foreach(size 16 32 48)
    install(FILES ui/icons/sfxr-qt-${size}.png
        DESTINATION share/icons/hicolor/${size}x${size}/apps
        RENAME sfxr-qt.png
    )
endforeach()
