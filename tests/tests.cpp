#define CATCH_CONFIG_RUNNER
#include "SynthesizerTest.h"

#include <QCoreApplication>
#include <QtTest>

#include <catch2/catch.hpp>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    if (app.arguments().contains("--update")) {
        SynthesizerTest::updateExpectedFiles();
        return 0;
    }
    QTEST_SET_MAIN_SOURCE_PATH
    return Catch::Session().run(argc, argv);
}
