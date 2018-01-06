project(legacy)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(
    ${GTK3_INCLUDE_DIRS}
)

set(sfxr_SRCS
    main.cpp
    sdlkit.cpp
    ../core/audio.cpp
)

add_executable(sfxr ${sfxr_SRCS})
target_link_libraries(sfxr SDL ${GTK3_LIBRARIES})

install(TARGETS sfxr
    DESTINATION bin
)
