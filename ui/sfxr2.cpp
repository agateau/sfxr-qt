#include <QApplication>
#include <QQmlApplicationEngine>

#include "generator.h"
#include "sound.h"
#include "synthesizer.h"
#include "wavsaver.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationDomain("agateau.com");
    app.setApplicationName("sfxr2");

    QQmlApplicationEngine engine;
    qmlRegisterType<Synthesizer>("sfxr2", 1, 0, "Synthesizer");
    qmlRegisterType<Sound>("sfxr2", 1, 0, "Sound");
    qmlRegisterType<Generator>("sfxr2", 1, 0, "Generator");
    qmlRegisterType<WavSaver>("sfxr2", 1, 0, "WavSaver");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
