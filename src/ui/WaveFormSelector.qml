import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import sfxr 1.0

RowLayout {
    id: root
    property Sound sound

    ListModel {
        id: waveFormModel
        // Must be kept in sync with WaveForm::Enum in WaveForm.h
        ListElement {
            waveForm: WaveForm.Square
            text: qsTr("Square")
        }
        ListElement {
            waveForm: WaveForm.Sawtooth
            text: qsTr("Sawtooth")
        }
        ListElement {
            waveForm: WaveForm.Triangle
            text: qsTr("Triangle")
        }
        ListElement {
            waveForm: WaveForm.Sine
            text: qsTr("Sine")
        }
        ListElement {
            waveForm: WaveForm.Noise
            text: qsTr("Noise")
        }
    }

    ButtonGroup {
        id: buttonGroup
    }

    Repeater {
        model: waveFormModel
        Button {
            checkable: true
            // Need to cast because waveFormModel.waveForm is an int
            checked: Number(sound.waveForm) === model.waveForm
            text: model.text
            ButtonGroup.group: buttonGroup
            onCheckedChanged: {
                if (checked) {
                    sound.waveForm = model.waveForm;
                }
            }
        }
    }
}
