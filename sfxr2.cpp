#include <QGuiApplication>

#include "audio.h"


int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    InitSDLAudio();
    PickupCoin();
    PlaySample();
    return app.exec();
}
