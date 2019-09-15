#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <fstream>

#include <QString>
#include <QUrl>

QByteArray loadFile(const QString& path);

/**
 * Disable all qDebug output while it is alive
 */
class QtDebugSilencer {
public:
    QtDebugSilencer();
    ~QtDebugSilencer();

private:
    QtMessageHandler mOldHandler;
};

// Let Catch know how to print some Qt types
std::ostream& operator<<(std::ostream& ostr, const QString& str);
std::ostream& operator<<(std::ostream& ostr, const QUrl& url);

#endif // TESTUTILS_H
