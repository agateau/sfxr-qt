import QtQuick 2.4
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import sfxr2 1.0

Window {
    id: root
    visible: true
    width: 800
    height: 600

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
        id: waveFormSelector
        synth: synth
        anchors {
            left: verticalLine.right
            leftMargin: 12
            top: parent.top
        }
    }

    Row {
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
                synth: synth
                model: ListModel {
                    ListElement {
                        text: qsTr("Attack time")
                        synthProperty: "attackTime"
                    }
                    ListElement {
                        text: qsTr("Sustain time")
                        synthProperty: "sustainTime"
                    }
                    ListElement {
                        text: qsTr("Sustain punch")
                        synthProperty: "sustainPunch"
                    }
                    ListElement {
                        text: qsTr("Decay time")
                        synthProperty: "decayTime"
                    }
                }
            }

            SliderGroup {
                Layout.fillWidth: true
                text: qsTr("Frequency")
                synth: synth
                model: ListModel {
                    ListElement {
                        text: qsTr("Start frequency")
                        synthProperty: "baseFrequency"
                    }
                    ListElement {
                        text: qsTr("Min frequency")
                        synthProperty: "minFrequency"
                    }
                    ListElement {
                        text: qsTr("Slide")
                        synthProperty: "slide"
                        bipolar: true
                    }
                    ListElement {
                        text: qsTr("Delta slide")
                        synthProperty: "deltaSlide"
                        bipolar: true
                    }
                    ListElement {
                        text: qsTr("Vibrato depth")
                        synthProperty: "vibratoDepth"
                    }
                    ListElement {
                        text: qsTr("Vibrato speed")
                        synthProperty: "vibratoSpeed"
                    }
                }
            }
        }

        ColumnLayout {
            width: 300

            SliderGroup {
                Layout.fillWidth: true
                text: qsTr("Change")
                synth: synth
                model: ListModel {
                    ListElement {
                        text: qsTr("Change amount")
                        synthProperty: "changeAmount"
                        bipolar: true
                    }
                    ListElement {
                        text: qsTr("Change speed")
                        synthProperty: "changeSpeed"
                    }
                }
            }

            SliderGroup {
                Layout.fillWidth: true
                enabled: synth.waveType === 0
                text: qsTr("Square")
                synth: synth
                model: ListModel {
                    ListElement {
                        text: qsTr("Square duty")
                        synthProperty: "squareDuty"
                    }
                    ListElement {
                        text: qsTr("Duty sweep")
                        synthProperty: "dutySweep"
                        bipolar: true
                    }
                }
            }
        }
    }
}
