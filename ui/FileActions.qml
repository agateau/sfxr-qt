import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2

import sfxr 1.0

ColumnLayout {
    id: root
    property Sound sound

    TitleLabel {
        text: qsTr("File")
        font.bold: true
    }

    Button {
        Layout.fillWidth: true
        FileDialog {
            id: loadFileDialog
            title: qsTr("Load SFXR sound")
            nameFilters: [
                qsTr("New format (*.sfxj)") + " (*.sfxj)",
                qsTr("Old format (*.sfxr)") + " (*.sfxr)",
                qsTr("All files") + " (*)"]
            onAccepted: {
                sound.load(fileUrl);
            }
        }
        text: qsTr("Load...")
        onClicked: {
            loadFileDialog.open();
        }
    }

    Shortcut {
        sequence: "CTRL+O"
        onActivated: loadFileDialog.open();
    }

    Button {
        Layout.fillWidth: true
        enabled: sound.hasRealUrl
        text: qsTr("Save")
        onClicked: {
            saveSound();
        }
    }

    Button {
        Layout.fillWidth: true
        FileDialog {
            id: saveFileDialog
            title: qsTr("Save SFXR sound")
            selectExisting: false
            nameFilters: [
                qsTr("New format (*.sfxj)") + " (*.sfxj)",
                qsTr("Old format (*.sfxr)") + " (*.sfxr)",
                qsTr("All files") + " (*)"]
            onAccepted: {
                sound.save(fileUrl);
            }
        }
        text: qsTr("Save as...")
        onClicked: {
            saveFileDialog.open();
        }
    }

    Shortcut {
        sequence: "CTRL+S"
        onActivated: {
            if (sound.hasRealUrl) {
                saveSound();
            } else {
                saveFileDialog.open();
            }
        }
    }

    VerticalSpacer {}

    TitleLabel {
        text: qsTr("WAV export")
    }

    WavSaver {
        id: wavSaver
    }

    Button {
        Layout.fillWidth: true
        text: qsTr("%1 bits").arg(wavSaver.bits)
        onClicked: {
            wavSaver.bits = wavSaver.bits == 16 ? 8 : 16;
        }
    }

    Button {
        Layout.fillWidth: true
        text: qsTr("%1 hz").arg(wavSaver.frequency)
        onClicked: {
            wavSaver.frequency = wavSaver.frequency == 44100 ? 22050 : 44100;
        }
    }

    Button {
        Layout.fillWidth: true
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
        text: qsTr("Export as...")
        onClicked: {
            exportWavFileDialog.open();
        }
    }

    function saveSound() {
        sound.save(sound.url);
    }
}
