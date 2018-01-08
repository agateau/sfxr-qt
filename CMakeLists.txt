project(sfxr)
cmake_minimum_required(VERSION 3.7)

find_package(SDL REQUIRED)
find_package(Qt5 REQUIRED Gui Quick Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -std=c++11")

include_directories(
    ${SDL_INCLUDE_DIR}
    core
    ${CMAKE_CURRENT_BINARY_DIR}/core
)

add_custom_command(
    OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/core/basesynthesizer.h
        ${CMAKE_CURRENT_BINARY_DIR}/core/basesynthesizer.cpp
    COMMAND
        qpropgen "${CMAKE_CURRENT_SOURCE_DIR}/core/basesynthesizer.yaml" -d "${CMAKE_CURRENT_BINARY_DIR}/core"
    DEPENDS
        core/basesynthesizer.yaml
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/core
)

# automoc cannot find generated files, so use qt5_wrap_cpp for them
qt5_wrap_cpp(sfxr2_MOC ${CMAKE_CURRENT_BINARY_DIR}/core/basesynthesizer.h)

set(sfxr2_SRCS
    ui/sfxr2.cpp
    core/synthesizer.cpp
    core/generator.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/core/basesynthesizer.cpp
)

qt5_add_resources(sfxr2_QRC ui/qml.qrc)

add_executable(sfxr2 ${sfxr2_SRCS} ${sfxr2_QRC} ${sfxr2_MOC})
target_link_libraries(sfxr2 SDL Qt5::Widgets Qt5::Quick)
