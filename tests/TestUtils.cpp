#include "TestUtils.h"

#include <QDebug>
#include <QFile>

QByteArray loadFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Could not load" << path << ":" << file.errorString();
        return {};
    }
    return file.readAll();
}

QtDebugSilencer::QtDebugSilencer() {
    auto silentHandler = [](QtMsgType, const QMessageLogContext&, const QString&) {
    };
    mOldHandler = qInstallMessageHandler(silentHandler);
}

QtDebugSilencer::~QtDebugSilencer() {
    qInstallMessageHandler(mOldHandler);
}

std::ostream &operator<<(std::ostream &ostr, const QString &str) {
    ostr << '"' << str.toStdString() << '"';
    return ostr;
}

std::ostream &operator<<(std::ostream &ostr, const QUrl &url) {
    ostr << '"' << url.toEncoded().constData() << '"';
    return ostr;
}
