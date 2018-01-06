import QtQuick 2.4
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import sfxr2 1.0

ColumnLayout {
    id: generatorLayout
    property Synthesizer synth
    Label {
        id: label
        text: qsTr("Generators")
    }

    Button {
        text: qsTr("Pickup/Coin")
        onClicked: {
            synth.generatePickup();
        }
    }

    Button {
        text: qsTr("Laser/Shoot")
        onClicked: {
            synth.generateLaser();
        }
    }

    Button {
        text: qsTr("Explosion")
        onClicked: {
            synth.generateExplosion();
        }
    }

    Button {
        text: qsTr("Power Up")
        onClicked: {
            synth.generatePowerup();
        }
    }

    Button {
        text: qsTr("Hit/Hurt")
        onClicked: {
            synth.generateHitHurt();
        }
    }

    Button {
        text: qsTr("Jump")
        onClicked: {
            synth.generateJump();
        }
    }

    Button {
        text: qsTr("Blip/Select")
        onClicked: {
            synth.generateBlipSelect();
        }
    }
}
