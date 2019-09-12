add_executable(tests
    tests.cpp
    SoundTest.cpp
    SynthesizerTest.cpp
    TestUtils.cpp
)

configure_file(TestConfig.h.in TestConfig.h @ONLY)

target_link_libraries(tests
    sfxr-qt-lib
    Qt5::Test
    Catch2::Catch2
)

target_include_directories(tests
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME tests COMMAND tests)
