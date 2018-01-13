import QtQuick 2.4
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import sfxr 1.0

ColumnLayout {
    id: root
    property Sound sound

    Generator {
        id: generator
        sound: root.sound
    }

    TitleLabel {
        id: label
        text: qsTr("Generators")
    }

    Button {
        text: qsTr("Pickup/Coin")
        onClicked: {
            generator.generatePickup();
        }
    }

    Button {
        text: qsTr("Laser/Shoot")
        onClicked: {
            generator.generateLaser();
        }
    }

    Button {
        text: qsTr("Explosion")
        onClicked: {
            generator.generateExplosion();
        }
    }

    Button {
        text: qsTr("Power Up")
        onClicked: {
            generator.generatePowerup();
        }
    }

    Button {
        text: qsTr("Hit/Hurt")
        onClicked: {
            generator.generateHitHurt();
        }
    }

    Button {
        text: qsTr("Jump")
        onClicked: {
            generator.generateJump();
        }
    }

    Button {
        text: qsTr("Blip/Select")
        onClicked: {
            generator.generateBlipSelect();
        }
    }

    Item {
        Layout.fillHeight: true
    }

    Button {
        text: qsTr("Mutate")
        onClicked: {
            generator.mutate();
        }
    }

    Button {
        text: qsTr("Randomize")
        onClicked: {
            generator.randomize();
        }
    }
}
