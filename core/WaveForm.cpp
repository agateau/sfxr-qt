#include <WaveForm.h>

#include <QQmlEngine>

namespace WaveForm {

static int rnd(int n) {
    return rand() % (n + 1);
}

Enum random(int max) {
    Q_ASSERT(max <= Last);
    return static_cast<Enum>(rnd(max));
}

void registerType() {
    qRegisterMetaType<WaveForm::Enum>();
    qmlRegisterUncreatableMetaObject(WaveForm::staticMetaObject,
                                     "sfxr", 1, 0, "WaveForm",
                                     "Only enums");
}

} // namespace
