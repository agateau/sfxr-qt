#include <WaveForm.h>

#include <QQmlEngine>

namespace WaveForm {

Enum random(const std::vector<Enum>& values) {
    int idx = std::rand() % values.size();
    return values.at(idx);
}

void registerType() {
    qRegisterMetaType<WaveForm::Enum>();
    qmlRegisterUncreatableMetaObject(
        WaveForm::staticMetaObject, "sfxr", 1, 0, "WaveForm", "Only enums");
}

} // namespace WaveForm
