project(sfxr)
cmake_minimum_required(VERSION 3.7)

find_package(SDL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Qt5 REQUIRED Gui)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(
    ${SDL_INCLUDE_DIR}
    ${GTK3_INCLUDE_DIRS}
)

set(sfxr_SRCS
    main.cpp
    sdlkit.cpp
    audio.cpp
)

add_executable(sfxr ${sfxr_SRCS})
target_link_libraries(sfxr SDL ${GTK3_LIBRARIES})

install(TARGETS sfxr
    DESTINATION bin
)

set(sfxr2_SRCS
    sfxr2.cpp
    audio.cpp
)
add_executable(sfxr2 ${sfxr2_SRCS})
target_link_libraries(sfxr2 SDL Qt5::Gui)
