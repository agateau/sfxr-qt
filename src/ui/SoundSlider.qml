import QtQuick 2.4
import QtQuick.Controls 2.0

import sfxr 1.0

Slider {
    id: root
    property Sound sound
    property string soundProperty
    property bool bipolar: false

    // For some reason setting from to -1 does not work anymore: the handle stays
    // stuck in the middle of the Slider (Qt 5.9.5 and Qt 5.11.1). To workaround
    // this, we shift the value range by 1
    // from: bipolar ? -1 : 0
    to: bipolar ? 2 : 1

    value: sliderValueForSoundValue(sound[soundProperty])

    onValueChanged: {
        updateTimer.start();
    }

    // Delay the update of the sound property to avoid glitchy sounds when
    // moving the slider continuously
    Timer {
        id: updateTimer
        interval: 200
        onTriggered: {
            root.sound[soundProperty] = soundValueForSliderValue(root.value);
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            root.value = sliderValueForSoundValue(0);
        }
    }

    Rectangle {
        width: 1
        height: parent.height
        z: -2
        visible: bipolar
        x: parent.width / 2
        color: "#ccc"
    }

    function soundValueForSliderValue(v) {
        return bipolar ? (v - 1) : v;
    }

    function sliderValueForSoundValue(v) {
        return bipolar ? (v + 1) : v;
    }
}
