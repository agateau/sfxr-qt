#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
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

    parser->addOption({"export", QApplication::translate("main", "Create a wav file from the given SFXR file and exit.")});
    parser->addOption({{"o", "output"},QApplication::translate("main", "Set the file to export to if --export is given."), "file"});
}

static void processArguments(QCommandLineParser* parser, QQmlApplicationEngine* engine) {
    parser->process(*qApp);
    const auto args = parser->positionalArguments();

    if (args.isEmpty()) {
        return;
    }
    const QUrl url = QUrl::fromUserInput(args.first(),QDir::currentPath(),QUrl::AssumeLocalFile);
    auto* root = engine->rootObjects().first();
    QMetaObject::invokeMethod(root, "loadSound", Q_ARG(QVariant, url));

    if (!parser->isSet("export")) {
        return;
    }

    QUrl outputUrl = QUrl::fromUserInput(parser->value("output"),QDir::currentPath(),QUrl::AssumeLocalFile);
    if (outputUrl.isEmpty()) {
       outputUrl = QUrl(url.toString().append(".wav"));
    }
    QMetaObject::invokeMethod(root, "saveWav", Q_ARG(QVariant, outputUrl));
    exit(0);
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
