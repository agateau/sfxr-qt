cmake_minimum_required(VERSION 3.17)
project(sfxr-qt
    VERSION 1.3.0
    DESCRIPTION "Retro sound effect generator"
    HOMEPAGE_URL "https://github.com/agateau/sfxr-qt"
)
include(CTest)

set(AUTHOR_NAME "Aurélien Gâteau")
set(AUTHOR_EMAIL "mail@agateau.com")
set(ORGANIZATION_NAME "agateau.com")
set(INVERSE_ORGANIZATION_NAME "com.agateau")

set(APP_HUMAN_NAME "SFXR-Qt")

set(APP_NAME ${PROJECT_NAME})
set(APPLIB_NAME ${PROJECT_NAME}lib)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Dependencies
find_package(Qt5 CONFIG REQUIRED Gui Quick Widgets Test)
find_package(ECM REQUIRED NO_MODULE)
list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
find_package(SDL REQUIRED)
include(3rdparty/qpropgen/cmake/qpropgen.cmake)

# Build flags
if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
    add_compile_options(-Wall -Woverloaded-virtual)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

# Source dirs
add_subdirectory(3rdparty/catch2)

# sfxr-qt-lib
set(APPLIB_SRCS
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

add_library(${APPLIB_NAME} STATIC
    ${APPLIB_SRCS}
    ${QPROPGEN_SRCS}
)

target_include_directories(${APPLIB_NAME}
    PUBLIC core
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/core
    PRIVATE ${SDL_INCLUDE_DIR}
)

target_link_libraries(${APPLIB_NAME}
    SDL
    Qt5::Qml
    Qt5::Core
)

# App executable
set(APP_SRCS
    ui/main.cpp
    ui/Generator.cpp
)

qt5_add_resources(QRC_SRCS ui/qml.qrc)

include(ECMAddAppIcon)

add_subdirectory(ui/icons)

ecm_add_app_icon(
    APP_SRCS
    ICONS
        ui/icons/48-apps-${APP_NAME}.png
        ui/icons/32-apps-${APP_NAME}.png
        ui/icons/16-apps-${APP_NAME}.png
)
add_executable(${APP_NAME} WIN32 MACOSX_BUNDLE ${APP_SRCS} ${QRC_SRCS})

target_link_libraries(${APP_NAME}
    ${APPLIB_NAME}
    Qt5::Widgets
    Qt5::Quick
)

target_include_directories(${APP_NAME}
    PRIVATE ui
)

if (BUILD_TESTING)
    enable_testing() # must come *before* adding tests directory
    add_subdirectory(tests)
endif()
add_subdirectory(packaging)

# Install
install(
    TARGETS ${APP_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

if (UNIX AND NOT APPLE)
    install(FILES ui/linux/${APP_NAME}.desktop
        DESTINATION share/applications
        RENAME ${INVERSE_ORGANIZATION_NAME}.${APP_NAME}.desktop
    )
endif()

if (WIN32)
    include(DeployQt)
    windeployqt(${APP_NAME})
endif()

if (APPLE)
    include(DeployQt)
    macdeployqt(${APP_NAME})
endif()
