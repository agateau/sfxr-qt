import QtQuick 2.4
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import sfxr2 1.0

ColumnLayout {
    id: root
    property string text
    property Sound sound
    property ListModel model

    Label {
        Layout.fillWidth: true
        text: root.text
        font.bold: true
    }

    Repeater {
        model: root.model
        Row {
            width: parent.width
            Label {
                text: model.text
                width: parent.width / 2
                anchors.verticalCenter: slider.verticalCenter
                horizontalAlignment: Qt.AlignRight
            }
            SoundSlider {
                id: slider
                sound: root.sound
                soundProperty: model.soundProperty
                bipolar: model.bipolar === true
                width: parent.width / 2
            }
        }
    }
}
