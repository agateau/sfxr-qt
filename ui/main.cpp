#include <QApplication>
#include <QIcon>
#include <QQmlApplicationEngine>

#include "Generator.h"
#include "Sound.h"
#include "SoundListModel.h"
#include "SoundPlayer.h"
#include "WavSaver.h"
#include "Result.h"

static QIcon createIcon() {
    QIcon icon;
    for (int size : {16, 32, 48}) {
        icon.addFile(QString(":/icons/sfxr-qt-%1.png").arg(size));
    }
    return icon;
}

static void registerQmlTypes() {
    qmlRegisterType<Sound>("sfxr", 1, 0, "Sound");
    qmlRegisterType<SoundPlayer>("sfxr", 1, 0, "SoundPlayer");
    qmlRegisterType<Generator>("sfxr", 1, 0, "Generator");
    qmlRegisterType<SoundListModel>("sfxr", 1, 0, "SoundListModel");
    qmlRegisterType<WavSaver>("sfxr", 1, 0, "WavSaver");
    WaveForm::registerType();
    Result::registerType();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationDomain("agateau.com");
    app.setApplicationName("sfxr-qt");
    app.setApplicationDisplayName("SFXR Qt");
    app.setWindowIcon(createIcon());

    registerQmlTypes();
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
