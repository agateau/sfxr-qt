#include <QApplication>
#include <QQmlApplicationEngine>

#include "generator.h"
#include "synthesizer.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationDomain("agateau.com");
    app.setApplicationName("sfxr2");

    QQmlApplicationEngine engine;
    qmlRegisterType<Synthesizer>("sfxr2", 1, 0, "Synthesizer");
    qmlRegisterType<Generator>("sfxr2", 1, 0, "Generator");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
