import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2

import sfxr 1.0

Window {
    id: root
    visible: true
    width: 1100
    height: 800

    property real margin: 12
    property real columnWidth: 350
    property alias sound: historyView.currentSound

    SoundPlayer {
        id: soundPlayer
        sound: root.sound
        onSoundChanged: {
            play();
        }
    }

    HistoryModel {
        id: historyModel
    }

    Generator {
        id: generator
        onSoundGenerated: {
            historyModel.append(name, sound);
            historyView.currentIndex = 0;
        }
    }

    ColumnLayout {
        id: leftColumn
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            margins: margin
        }
        width: 150
        spacing: margin

        Generators {
            id: generators
            generator: generator
            sound: root.sound
        }

        TitleLabel {
            text: qsTr("History")
        }

        HistoryView {
            id: historyView
            model: historyModel
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    Item {
        id: mainContent
        anchors {
            left: leftColumn.right
            right: fileActions.left
            top: parent.top
            bottom: parent.bottom
            margins: margin
        }

        RowLayout {
            id: waveFormRow
            spacing: 12
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
            }
            Label {
                text: qsTr("Wave form")
            }
            WaveFormSelector {
                sound: root.sound
            }
        }

        Row {
            spacing: margin * 2

            anchors {
                top: waveFormRow.bottom
                topMargin: margin
                horizontalCenter: parent.horizontalCenter
            }

            property real columnWidth: 300

            ColumnLayout {
                width: root.columnWidth

                SliderGroup {
                    Layout.fillWidth: true
                    text: qsTr("Envelop")
                    sound: root.sound
                    model: ListModel {
                        ListElement {
                            text: qsTr("Attack time")
                            soundProperty: "attackTime"
                        }
                        ListElement {
                            text: qsTr("Sustain time")
                            soundProperty: "sustainTime"
                        }
                        ListElement {
                            text: qsTr("Sustain punch")
                            soundProperty: "sustainPunch"
                        }
                        ListElement {
                            text: qsTr("Decay time")
                            soundProperty: "decayTime"
                        }
                    }
                }

                SliderGroup {
                    Layout.fillWidth: true
                    text: qsTr("Frequency")
                    sound: root.sound
                    model: ListModel {
                        ListElement {
                            text: qsTr("Start frequency")
                            soundProperty: "baseFrequency"
                        }
                        ListElement {
                            text: qsTr("Min frequency")
                            soundProperty: "minFrequency"
                        }
                        ListElement {
                            text: qsTr("Slide")
                            soundProperty: "slide"
                            bipolar: true
                        }
                        ListElement {
                            text: qsTr("Delta slide")
                            soundProperty: "deltaSlide"
                            bipolar: true
                        }
                        ListElement {
                            text: qsTr("Vibrato depth")
                            soundProperty: "vibratoDepth"
                        }
                        ListElement {
                            text: qsTr("Vibrato speed")
                            soundProperty: "vibratoSpeed"
                        }
                    }
                }

                SliderGroup {
                    Layout.fillWidth: true
                    text: qsTr("Change")
                    sound: root.sound
                    model: ListModel {
                        ListElement {
                            text: qsTr("Change amount")
                            soundProperty: "changeAmount"
                            bipolar: true
                        }
                        ListElement {
                            text: qsTr("Change speed")
                            soundProperty: "changeSpeed"
                        }
                    }
                }
            }

            ColumnLayout {
                width: root.columnWidth

                SliderGroup {
                    Layout.fillWidth: true
                    enabled: sound.waveType === 0
                    text: qsTr("Square")
                    sound: root.sound
                    model: ListModel {
                        ListElement {
                            text: qsTr("Square duty")
                            soundProperty: "squareDuty"
                        }
                        ListElement {
                            text: qsTr("Duty sweep")
                            soundProperty: "dutySweep"
                            bipolar: true
                        }
                    }
                }

                SliderGroup {
                    Layout.fillWidth: true
                    text: qsTr("Repeat")
                    sound: root.sound
                    model: ListModel {
                        ListElement {
                            text: qsTr("Repeat speed")
                            soundProperty: "repeatSpeed"
                        }
                    }
                }

                SliderGroup {
                    Layout.fillWidth: true
                    text: qsTr("Phaser")
                    sound: root.sound
                    model: ListModel {
                        ListElement {
                            text: qsTr("Phaser offset")
                            soundProperty: "phaserOffset"
                            bipolar: true
                        }
                        ListElement {
                            text: qsTr("Phaser sweep")
                            soundProperty: "phaserSweep"
                            bipolar: true
                        }
                    }
                }

                SliderGroup {
                    Layout.fillWidth: true
                    text: qsTr("Filters")
                    sound: root.sound
                    model: ListModel {
                        ListElement {
                            text: qsTr("LP filter cutoff")
                            soundProperty: "lpFilterCutoff"
                        }
                        ListElement {
                            text: qsTr("LP filter cutoff sweep")
                            soundProperty: "lpFilterCutoffSweep"
                            bipolar: true
                        }
                        ListElement {
                            text: qsTr("LP filter resonance")
                            soundProperty: "lpFilterResonance"
                        }
                        ListElement {
                            text: qsTr("HP filter cutoff")
                            soundProperty: "hpFilterCutoff"
                        }
                        ListElement {
                            text: qsTr("HP filter cutoff sweep")
                            soundProperty: "hpFilterCutoffSweep"
                            bipolar: true
                        }
                    }
                }

                SliderGroup {
                    Layout.fillWidth: true
                    text: qsTr("Others")
                    sound: root.sound
                    model: ListModel {
                        ListElement {
                            text: qsTr("Volume")
                            soundProperty: "volume"
                        }
                    }
                }
            }
        }

        Button {
            id: playButton
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
            }
            width: implicitWidth * 1.5
            height: implicitHeight * 1.5

            text: qsTr("Play (Return)")
            onClicked: {
                soundPlayer.play();
            }
        }

        CheckBox {
            anchors {
                left: playButton.right
                verticalCenter: playButton.verticalCenter
            }
            text: qsTr("Loop (L)")
            checked: soundPlayer.loop
            onCheckedChanged: {
                soundPlayer.loop = checked;
            }
        }

        Shortcut {
            sequence: "Return"
            onActivated: soundPlayer.play();
        }

        Shortcut {
            sequence: "L"
            onActivated: {
                soundPlayer.loop = !soundPlayer.loop;
            }
        }
    }

    FileActions {
        id: fileActions
        anchors {
            top: parent.top
            right: parent.right
            margins: margin
        }
        sound: root.sound
    }
}
