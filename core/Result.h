#ifndef RESULT_H
#define RESULT_H

#include <QObject>

class Result {
    Q_GADGET
    Q_PROPERTY(bool ok READ isOk)
    Q_PROPERTY(QString message READ message)
public:
    Result();

    bool isOk() const;
    QString message() const;

    operator bool() const { return isOk(); }

    static Result createOk();
    static Result createError(const QString& message);

    static void registerType();

private:
    bool mOk = true;
    QString mMessage;
};

Q_DECLARE_METATYPE(Result)

#endif // RESULT_H
