import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2

import sfxr 1.0

ApplicationWindow {
    id: root
    visible: true
    minimumHeight: 600
    minimumWidth: leftColumn.width + mainContent.implicitWidth + fileActions.width + 2 * margin

    property real margin: 12
    property real sideColumnWidth: 200
    property real columnWidth: 350
    property Sound sound: {
        var row = soundListView.currentIndex;
        row >= 0 ? soundListModel.soundForRow(row) : null;
    }

    SoundPlayer {
        id: soundPlayer
        sound: root.sound
        property bool startup: true
        onSoundChanged: {
            // Hack to avoid playing directly at startup
            if (startup) {
                startup = false;
                return;
            }
            play();
        }
    }

    SoundListModel {
        id: soundListModel
    }

    Generator {
        id: generator
        onSoundGenerated: {
            soundListModel.addNew(sound);
            soundListView.currentIndex = 0;
        }
    }

    Item {
        id: rootItem
        anchors.fill: parent
        ColumnLayout {
            id: leftColumn
            anchors {
                left: parent.left
                top: parent.top
                bottom: parent.bottom
            }
            width: sideColumnWidth

            Generators {
                id: generators
                generator: generator
                sound: root.sound
            }

            VerticalSpacer {}

            SoundListView {
                id: soundListView
                model: soundListModel
                Layout.fillWidth: true
                Layout.fillHeight: true
                onCurrentSoundClicked: {
                    soundPlayer.play();
                }
            }
        }

        ColumnLayout {
            id: mainContent
            anchors {
                left: leftColumn.right
                right: fileActions.left
                top: parent.top
                bottom: parent.bottom
                leftMargin: margin
                rightMargin: margin
            }
            Layout.minimumWidth: implicitWidth

            RowLayout {
                id: waveFormRow
                spacing: 12
                Layout.alignment: Qt.AlignHCenter
                Label {
                    text: qsTr("Wave form")
                }
                WaveFormSelector {
                    sound: root.sound
                }
            }

            Row {
                spacing: margin * 2

                Layout.alignment: Qt.AlignHCenter

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
                        enabled: Number(sound.waveForm) === WaveForm.Square
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

            PlayBar {
                Layout.alignment: Qt.AlignHCenter
                soundPlayer: soundPlayer
            }
        }

        FileActions {
            id: fileActions
            sound: root.sound
            anchors {
                right: parent.right
                top: parent.top
            }
            width: sideColumnWidth
        }
    }

    function loadSound(url) {
        fileActions.loadSound(url);
    }
}
