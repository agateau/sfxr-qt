project(sfxr-qt)
cmake_minimum_required(VERSION 3.7)

find_package(SDL REQUIRED)
find_package(Qt5 REQUIRED Gui Quick Widgets Test)
include(3rdparty/qpropgen/cmake/qpropgen.cmake)
add_subdirectory(3rdparty/catch2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual")

# sfxr-qt-lib
set(sfxr_qt_lib_SRCS
    core/Synthesizer.cpp
    core/NoiseGenerator.cpp
    core/WavSaver.cpp
    core/Sound.cpp
    core/SoundUtils.cpp
    core/SoundPlayer.cpp
    core/SoundListModel.cpp
)

qpropgen(QPROPGEN_SRCS
    core/BaseSound.yaml
    core/BaseSoundListModel.yaml
    core/BaseWavSaver.yaml
)

add_library(sfxr-qt-lib STATIC ${sfxr_qt_lib_SRCS} ${QPROPGEN_SRCS})

target_link_libraries(sfxr-qt-lib
    SDL
    Qt5::Core
)

target_include_directories(sfxr-qt-lib
    PUBLIC core
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/core
    PRIVATE ${SDL_INCLUDE_DIR})

# sfxr-qt
set(sfxr_qt_app_SRCS
    ui/main.cpp
    ui/Generator.cpp
)

qt5_add_resources(QRC_SRCS ui/qml.qrc)

add_executable(sfxr-qt ${sfxr_qt_app_SRCS} ${QRC_SRCS})
target_link_libraries(sfxr-qt
    sfxr-qt-lib
    Qt5::Widgets
    Qt5::Quick
)

target_include_directories(sfxr-qt
    PRIVATE ui
)

add_subdirectory(tests)

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
