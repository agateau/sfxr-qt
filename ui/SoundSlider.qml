import QtQuick 2.4
import QtQuick.Controls 2.0

import sfxr 1.0

Slider {
    id: root
    property Sound sound
    property string soundProperty
    property bool bipolar: false

    from: bipolar ? -1 : 0

    value: sound[soundProperty]

    onValueChanged: {
        sound[soundProperty] = value;
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            root.value = 0;
        }
    }
}
