import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import sfxr 1.0

ColumnLayout {
    id: root
    property Generator generator
    property Sound sound

    TitleLabel {
        id: label
        text: qsTr("Generators")
    }

    Button {
        text: qsTr("Pickup/Coin")
        Layout.fillWidth: true
        onClicked: {
            generator.generatePickup();
        }
    }

    Button {
        text: qsTr("Laser/Shoot")
        Layout.fillWidth: true
        onClicked: {
            generator.generateLaser();
        }
    }

    Button {
        text: qsTr("Explosion")
        Layout.fillWidth: true
        onClicked: {
            generator.generateExplosion();
        }
    }

    Button {
        text: qsTr("Power Up")
        Layout.fillWidth: true
        onClicked: {
            generator.generatePowerup();
        }
    }

    Button {
        text: qsTr("Hit/Hurt")
        Layout.fillWidth: true
        onClicked: {
            generator.generateHitHurt();
        }
    }

    Button {
        text: qsTr("Jump")
        Layout.fillWidth: true
        onClicked: {
            generator.generateJump();
        }
    }

    Button {
        text: qsTr("Blip/Select")
        Layout.fillWidth: true
        onClicked: {
            generator.generateBlipSelect();
        }
    }

    VerticalSpacer {}

    Button {
        text: qsTr("Randomize")
        Layout.fillWidth: true
        onClicked: {
            generator.randomize(sound.waveForm);
        }
    }

    Button {
        text: qsTr("Mutate")
        Layout.fillWidth: true
        onClicked: {
            generator.mutate(sound);
        }
    }
}
