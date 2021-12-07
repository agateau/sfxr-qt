cmake_minimum_required(VERSION 3.10)
project(sfxr-qt
    VERSION 1.3.0
    DESCRIPTION "Retro sound effect generator"
)

find_package(SDL REQUIRED)
find_package(Qt5 REQUIRED Gui Quick Widgets Test)
include(3rdparty/qpropgen/cmake/qpropgen.cmake)
add_subdirectory(3rdparty/catch2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual")
enable_testing()

# sfxr-qt-lib
set(sfxr_qt_lib_SRCS
    core/Synthesizer.cpp
    core/NoiseGenerator.cpp
    core/WavSaver.cpp
    core/Sound.cpp
    core/SoundUtils.cpp
    core/SoundPlayer.cpp
    core/SoundListModel.cpp
    core/SoundIO.cpp
    core/Result.cpp
    core/WaveForm.cpp
)

qpropgen(QPROPGEN_SRCS
    core/BaseSound.yaml
    core/BaseSoundListModel.yaml
    core/BaseWavSaver.yaml
)

add_library(sfxr-qt-lib STATIC ${sfxr_qt_lib_SRCS} ${QPROPGEN_SRCS})

target_link_libraries(sfxr-qt-lib
    SDL
    Qt5::Qml
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

# Packaging
set(PROJECT_URL "https://github.com/agateau/sfxr-qt")

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "Aurélien Gâteau <mail@agateau.com>")

set(CPACK_GENERATOR "DEB;RPM")
set(CPACK_SOURCE_GENERATOR "TBZ2")

## DEB config
set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${PROJECT_URL})
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_RELEASE "0agc1")

## RPM config
set(CPACK_RPM_FILE_NAME "RPM-DEFAULT")
set(CPACK_RPM_PACKAGE_URL ${PROJECT_URL})

## Source config
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_SOURCE_IGNORE_FILES "/build/;/.git/;/__pycache__/;~$")
include(CPack)
