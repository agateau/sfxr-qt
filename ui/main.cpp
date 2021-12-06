#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QQmlApplicationEngine>

#include "Generator.h"
#include "Sound.h"
#include "SoundIO.h"
#include "SoundListModel.h"
#include "SoundPlayer.h"
#include "WavSaver.h"
#include "Result.h"

#include <optional>

using std::optional;

static QIcon createIcon() {
    QIcon icon;
    for (int size : {16, 32, 48}) {
        icon.addFile(QString(":/icons/sfxr-qt-%1.png").arg(size));
    }
    return icon;
}

struct Arguments {
    QUrl url;
    bool export_ = false;
    QUrl outputUrl;
    std::optional<int> outputBits;
    std::optional<int> outputFrequency;

    static optional<Arguments> parse(QCommandLineParser* parser) {
        Arguments instance;
        auto args = parser->positionalArguments();
        if (args.isEmpty()) {
            return {};
        }

        instance.url = QUrl::fromUserInput(args.first(), QDir::currentPath(), QUrl::AssumeLocalFile);

        instance.export_ = parser->isSet("export");
        if (!instance.export_) {
            return instance;
        }

        instance.outputUrl = QUrl::fromUserInput(parser->value("output"), QDir::currentPath(), QUrl::AssumeLocalFile);
        if (instance.outputUrl.isEmpty()) {
           instance.outputUrl = QUrl(instance.url.toString().append(".wav"));
        }

        int outputBits = parser->value("bits").toInt();
        if (outputBits > 0) {
            instance.outputBits = outputBits;
        }

        int outputFrequency = parser->value("samplerate").toInt();
        if (outputFrequency > 0) {
            instance.outputFrequency = outputFrequency;
        }

        return instance;
    }
};

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
    parser->addOption({{"b", "bits"},QApplication::translate("main", "Set the bits per sample of the exported wav."), "8 or 16"});
    parser->addOption({{"s", "samplerate"},QApplication::translate("main", "Set samplerate in hertz of the exported wav."), "22050 or 44100"});
}

static void processArguments(QCommandLineParser* parser, QQmlApplicationEngine* engine) {
    parser->process(*qApp);
    auto maybeArgs = Arguments::parse(parser);
    if (!maybeArgs.has_value()) {
        return;
    }
    auto args = maybeArgs.value();

    auto* root = engine->rootObjects().first();
    QMetaObject::invokeMethod(root, "loadSound", Q_ARG(QVariant, args.url));

    if (!args.export_) {
        return;
    }

    Sound sound;
    if (auto res = SoundIO::load(&sound, args.url); !res) {
        qWarning() << res.message();
        exit(1);
    }

    WavSaver saver;
    if (args.outputBits.has_value()) {
        saver.setBits(args.outputBits.value());
    }

    if (args.outputFrequency.has_value()) {
        saver.setFrequency(args.outputFrequency.value());
    }

    if (!saver.save(&sound, args.outputUrl)) {
        qWarning() << "Could not save sound to" << args.outputUrl.path();
        exit(1);
    }
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
