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

    Component {
        id: messageDialogComponent
        MessageDialog {
            icon: StandardIcon.Warning
            standardButtons: StandardButton.Ok
        }
    }

    Button {
        Layout.fillWidth: true
        FileDialog {
            id: loadFileDialog
            title: qsTr("Load sound")
            nameFilters: [
                qsTr("Supported formats (*.sfxj, *.sfxr)") + " (*.sfxj *.sfxr)",
                qsTr("All files") + " (*)"]
            onAccepted: {
                loadSound(fileUrl);
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
            saveSound(sound.url);
        }
    }

    Button {
        Layout.fillWidth: true
        FileDialog {
            id: saveFileDialog
            title: qsTr("Save sound")
            selectExisting: false
            nameFilters: [
                qsTr("SFXR Json (*.sfxj)") + " (*.sfxj)",
                qsTr("SFXR Binary (old) (*.sfxr)") + " (*.sfxr)",
                qsTr("All files") + " (*)"]
            onAccepted: {
                saveSound(fileUrl);
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
                saveSound(sound.url);
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

    function showError(title, text) {
        var dlg = messageDialogComponent.createObject(this, {title: title, text: text});
        dlg.open();
    }

    function saveSound(url) {
        var result = sound.save(url);
        if (!result.ok) {
            var message = qsTr("Could not save file to \"%1\".\n%2").arg(url).arg(result.message);
            showError(qsTr("Error saving file"), message);
        }
    }

    function loadSound(url) {
        var result = sound.load(url);
        if (!result.ok) {
            var message = qsTr("Could not load file \"%1\".\n%2").arg(url).arg(result.message);
            showError(qsTr("Error loading file"), message);
        }
    }
}
