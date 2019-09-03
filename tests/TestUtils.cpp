#include "TestUtils.h"

std::ostream &operator<<(std::ostream &ostr, const QString &str)
{
    ostr << '"' << str.toStdString() << '"';
    return ostr;
}

std::ostream &operator<<(std::ostream &ostr, const QUrl &url)
{
    ostr << '"' << url.toEncoded().constData() << '"';
    return ostr;
}
