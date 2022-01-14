#include "Generator.h"
#include "Result.h"
#include "Sound.h"
#include "SoundIO.h"
#include "SoundListModel.h"
#include "SoundPlayer.h"
#include "WavSaver.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QQmlApplicationEngine>

#include <optional>

using std::optional;

static QIcon createIcon() {
    QIcon icon;
    for (int size : {16, 32, 48}) {
        icon.addFile(QString(":/icons/%1-apps-sfxr-qt.png").arg(size));
    }
    return icon;
}

struct Arguments {
    QUrl url;
    bool export_ = false;
    QUrl outputUrl;
    std::optional<int> outputBits;
    std::optional<int> outputFrequency;

    static optional<Arguments> parse(const QCommandLineParser& parser) {
        Arguments instance;
        auto args = parser.positionalArguments();
        instance.export_ = parser.isSet("export");

        if (args.isEmpty()) {
            if (instance.export_) {
                qCritical() << QApplication::translate("main", "No file given to export.");
                exit(1);
            }
            return {};
        }

        if (args.size() > 1) {
            qWarning() << QApplication::translate(
                "main", "Too many positional arguments given. Only the first one is used.");
        }

        instance.url =
            QUrl::fromUserInput(args.first(), QDir::currentPath(), QUrl::AssumeLocalFile);

        if (!instance.export_) {
            return instance;
        }

        instance.outputUrl =
            QUrl::fromUserInput(parser.value("output"), QDir::currentPath(), QUrl::AssumeLocalFile);
        if (instance.outputUrl.isEmpty()) {
            auto path = instance.url.path().section('.', 0, -2) + ".wav";
            instance.outputUrl = QUrl::fromLocalFile(path);
        }

        if (parser.isSet("bits")) {
            int outputBits = parser.value("bits").toInt();
            if (!(outputBits == 8 || outputBits == 16)) {
                qCritical() << QApplication::translate(
                    "main", "Invalid bits per sample. Supported values are 8 and 16.");
                exit(1);
            }
            instance.outputBits = outputBits;
        }

        if (parser.isSet("rate")) {
            int outputFrequency = parser.value("rate").toInt();
            if (!(outputFrequency == 22050 || outputFrequency == 44100)) {
                qCritical() << QApplication::translate(
                    "main", "Invalid samplerate. Supported values are 22050 and 44100.");
                exit(1);
            }
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

    parser->addOption({"export",
                       QApplication::translate(
                           "main", "Creates a wav file from the given SFXR file and exits.")});
    parser->addOption(
        {{"o", "output"},
         QApplication::translate("main", "Specifies the path for the file created with --export."),
         "path"});
    parser->addOption(
        {{"b", "bits"},
         QApplication::translate("main",
                                 "Specifies the bits per sample for the wav file created with "
                                 "--export. Supported values are 8 and 16."),
         "number"});
    parser->addOption(
        {{"r", "rate"},
         QApplication::translate("main",
                                 "Specifies the samplerate for the wav file created with --export. "
                                 "Supported values are 22050 and 44100."),
         "number"});
}

static int exportSound(const Arguments& args) {
    Sound sound;
    if (auto res = SoundIO::load(&sound, args.url); !res) {
        qCritical("%s", qUtf8Printable(res.message()));
        return 1;
    }

    WavSaver saver;
    if (args.outputBits.has_value()) {
        saver.setBits(args.outputBits.value());
    }

    if (args.outputFrequency.has_value()) {
        saver.setFrequency(args.outputFrequency.value());
    }

    if (!saver.save(&sound, args.outputUrl)) {
        qCritical("Could not save sound to %s.", qUtf8Printable(args.outputUrl.path()));
        return 1;
    }
    return 0;
}

static void loadInitialSound(QQmlApplicationEngine* engine, const QUrl& url) {
    auto* root = engine->rootObjects().first();
    QMetaObject::invokeMethod(root, "loadSound", Q_ARG(QVariant, url));
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(qml);
    app.setOrganizationDomain("agateau.com");
    app.setApplicationName("sfxr-qt");
    app.setApplicationDisplayName("SFXR Qt");
    app.setWindowIcon(createIcon());
    registerQmlTypes();

    QCommandLineParser parser;
    setupCommandLineParser(&parser);
    parser.process(*qApp);

    auto maybeArgs = Arguments::parse(parser);
    if (maybeArgs.has_value() && maybeArgs.value().export_) {
        return exportSound(maybeArgs.value());
    }

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (maybeArgs.has_value()) {
        loadInitialSound(&engine, maybeArgs.value().url);
    }

    return app.exec();
}
