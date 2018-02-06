import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import sfxr 1.0

Item {
    id: root
    property alias model: listView.model
    property alias currentIndex: listView.currentIndex
    property Sound currentSound: listView.currentItem.sound
    signal currentSoundClicked()

    ListView {
        id: listView
        anchors.fill: parent
        clip: true
        currentIndex: 0
        delegate: ItemDelegate {
            anchors {
                left: parent.left
                right: parent.right
            }
            text: model.text
            highlighted: ListView.isCurrentItem

            property Sound sound: model.sound

            onClicked: {
                if (listView.currentIndex == model.index) {
                    currentSoundClicked();
                } else {
                    listView.currentIndex = model.index;
                }
            }
        }

        ScrollIndicator.vertical: ScrollIndicator {}
    }
}
