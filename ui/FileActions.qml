import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2

import sfxr2 1.0

ColumnLayout {
    property Sound sound
    Label {
        text: qsTr("Files")
    }

    WavSaver {
        id: wavSaver
    }

    Button {
        FileDialog {
            id: exportWavFileDialog
            selectExisting: false
            title: qsTr("Export as WAV")
            nameFilters: [qsTr("Wav files") + " (*.wav)",
                qsTr("All files") + " (*)"]
            onAccepted: {
                wavSaver.save(sound, fileUrl);
            }
        }
        text: qsTr("Export as WAV")
        onClicked: {
            exportWavFileDialog.open();
        }
    }
}
