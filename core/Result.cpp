#include "Result.h"

#include <QQmlApplicationEngine>

Result::Result() {
}

bool Result::isOk() const {
    return mOk;
}

QString Result::message() const {
    return mMessage;
}

Result Result::createOk() {
    return Result();
}

Result Result::createError(const QString& message) {
    Result obj;
    obj.mOk = false;
    obj.mMessage = message;
    return obj;
}

void Result::registerType() {
    qRegisterMetaType<Result>();
    QMetaType::registerEqualsComparator<Result>();
}
