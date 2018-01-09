#include <QApplication>
#include <QQmlApplicationEngine>

#include "generator.h"
#include "sound.h"
#include "soundplayer.h"
#include "wavsaver.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationDomain("agateau.com");
    app.setApplicationName("sfxr2");

    QQmlApplicationEngine engine;
    qmlRegisterType<Sound>("sfxr2", 1, 0, "Sound");
    qmlRegisterType<SoundPlayer>("sfxr2", 1, 0, "SoundPlayer");
    qmlRegisterType<Generator>("sfxr2", 1, 0, "Generator");
    qmlRegisterType<WavSaver>("sfxr2", 1, 0, "WavSaver");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
