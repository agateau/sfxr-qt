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
add_subdirectory(src)
if (BUILD_TESTING)
    enable_testing() # must come *before* adding tests directory
    add_subdirectory(tests)
endif()
add_subdirectory(packaging)
