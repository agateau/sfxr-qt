import QtQuick 2.4
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import sfxr2 1.0

Window {
    id: root
    visible: true
    width: 600
    height: 400

    Synthesizer {
        id: synth
    }

    Generators {
        id: generators
        synth: synth
        anchors.left: parent.left
        anchors.top: parent.top
    }

    Rectangle {
        id: verticalLine
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: generators.right
            leftMargin: 12
        }
        width: 1
    }

    WaveFormSelector {
        synth: synth
        anchors {
            left: verticalLine.right
            leftMargin: 12
            top: parent.top
        }
    }
}
