import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    id: item1
    width: 400
    height: 400
    property alias laserButton: laserButton
    property alias pickupButton: pickupButton
    property alias explosionButton: explosionButton
    property alias powerupButton: powerupButton
    property alias hitHurtButton: hitHurtButton
    property alias jumpButton: jumpButton
    property alias blipSelectButton: blipSelectButton

    ColumnLayout {
        id: columnLayout
        width: 100
        anchors.left: parent.left
        anchors.top: parent.top

        Label {
            id: label
            text: qsTr("Generator")
        }

        Button {
            id: pickupButton
            x: 22
            y: 243
            text: qsTr("Pickup/Coin")
        }

        Button {
            id: laserButton
            x: 22
            y: 289
            text: qsTr("Laser/Shoot")
        }

        Button {
            id: explosionButton
            text: qsTr("Explosion")
        }

        Button {
            id: powerupButton
            text: qsTr("Power Up")
        }

        Button {
            id: hitHurtButton
            text: qsTr("Hit/Hurt")
        }

        Button {
            id: jumpButton
            text: qsTr("Jump")
        }

        Button {
            id: blipSelectButton
            text: qsTr("Blip/Select")
        }
    }
}
