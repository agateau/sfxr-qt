import QtQuick 2.4
import QtQuick.Controls 2.2

import sfxr 1.0

Slider {
    property Sound sound
    property string soundProperty
    property bool bipolar: false

    from: bipolar ? -1 : 0

    value: sound[soundProperty]

    onValueChanged: {
        sound[soundProperty] = value;
    }
}
