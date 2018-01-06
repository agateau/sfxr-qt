#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "audio.h"

#include "synthesizer.h"

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    InitSDLAudio();
    /*
    PickupCoin();
    PlaySample();
    */
    QQmlApplicationEngine engine;
    qmlRegisterType<Synthesizer>("sfxr2", 1, 0, "Synthesizer");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
