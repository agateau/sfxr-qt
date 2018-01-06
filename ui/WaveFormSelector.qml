import QtQuick 2.4
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import sfxr2 1.0

RowLayout {
    id: root
    property Synthesizer synth

    ButtonGroup {
        buttons: root.children
    }

    Button {
        checkable: true
        text: qsTr("Square")
    }

    Button {
        checkable: true
        text: qsTr("Sawtooth")
    }

    Button {
        checkable: true
        text: qsTr("Sine")
    }

    Button {
        checkable: true
        text: qsTr("Noise")
    }
}
