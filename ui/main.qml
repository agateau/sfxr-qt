import QtQuick 2.4
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2

import sfxr2 1.0

Window {
    id: root
    visible: true
    width: 1000
    height: 800

    Sound {
        id: sound
    }

    Synthesizer {
        id: synth
        sound: sound
    }

    Generators {
        id: generators
        sound: sound
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
        color: "black"
    }

    WaveFormSelector {
        id: waveFormSelector
        sound: sound
        anchors {
            left: verticalLine.right
            leftMargin: 12
            top: parent.top
        }
    }

    Row {
        spacing: 12

        anchors {
            left: verticalLine.right
            leftMargin: 12
            top: waveFormSelector.bottom
            topMargin: 12
        }

        ColumnLayout {
            width: 300

            SliderGroup {
                Layout.fillWidth: true
                text: qsTr("Envelop")
                sound: sound
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
                sound: sound
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
                sound: sound
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
            width: 300

            SliderGroup {
                Layout.fillWidth: true
                enabled: synth.waveType === 0
                text: qsTr("Square")
                sound: sound
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
                sound: sound
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
                sound: sound
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
                sound: sound
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
        }
    }

    FileActions {
        anchors {
            top: parent.top
            right: parent.right
        }

        sound: sound
    }
}
