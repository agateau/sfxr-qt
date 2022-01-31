
include(ECMAddAppIcon)

add_subdirectory(ui/icons)

# Sources
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
    ui/SoundPreview.cpp
)

qpropgen(QPROPGEN_SRCS
    core/BaseSound.yaml
    core/BaseSoundListModel.yaml
    core/BaseWavSaver.yaml
)

# App library
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
    Qt5::Concurrent
    Qt5::Core
    Qt5::Quick
)

# App executable
set(APP_SRCS
    ui/main.cpp
    ui/Generator.cpp
)

qt5_add_resources(QRC_SRCS ui/qml.qrc)

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

# Install
install(
    TARGETS ${APP_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

if (UNIX AND NOT APPLE)
    install(FILES linux/${APP_NAME}.desktop
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
