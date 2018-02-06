import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import sfxr 1.0

Item {
    id: root
    property alias model: listView.model
    signal soundClicked(Sound sound)

    ListView {
        id: listView
        anchors.fill: parent
        clip: true
        delegate: ItemDelegate {
            anchors {
                left: parent.left
                right: parent.right
            }
            text: model.text
            highlighted: ListView.isCurrentItem

            property Sound sound: model.sound

            onClicked: {
                listView.currentIndex = model.index;
            }
        }
        onCurrentItemChanged: {
            if (currentItem) {
                root.soundClicked(currentItem.sound);
            }
        }

        ScrollIndicator.vertical: ScrollIndicator {}
    }
}
