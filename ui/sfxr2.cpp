#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "synthesizer.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    qmlRegisterType<Synthesizer>("sfxr2", 1, 0, "Synthesizer");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
