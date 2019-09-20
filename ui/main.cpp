#include <QApplication>
#include <QCommandLineParser>
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

static void setupCommandLineParser(QCommandLineParser* parser) {
    parser->addHelpOption();
    parser->addPositionalArgument("sound_file", QApplication::translate("main", "File to load."));
}

static void processArguments(QCommandLineParser* parser, QQmlApplicationEngine* engine) {
    parser->process(*qApp);
    const auto args = parser->positionalArguments();
    if (args.isEmpty()) {
        return;
    }
    const QUrl url = QUrl::fromUserInput(args.first());
    auto* root = engine->rootObjects().first();
    QMetaObject::invokeMethod(root, "loadSound", Q_ARG(QVariant, url));
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationDomain("agateau.com");
    app.setApplicationName("sfxr-qt");
    app.setApplicationDisplayName("SFXR Qt");
    app.setWindowIcon(createIcon());

    QCommandLineParser parser;
    setupCommandLineParser(&parser);

    registerQmlTypes();
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    processArguments(&parser, &engine);

    return app.exec();
}
