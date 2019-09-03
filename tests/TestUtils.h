#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <fstream>

#include <QString>
#include <QUrl>

// Let Catch know how to print some Qt types

std::ostream &operator<<(std::ostream &ostr, const QString &str);
std::ostream &operator<<(std::ostream &ostr, const QUrl &url);

#endif // TESTUTILS_H
