import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import sfxr 1.0

ColumnLayout {
    id: root
    property alias model: listView.model
    property alias currentIndex: listView.currentIndex
    signal currentSoundClicked()

    property int innerMargin: 4

    TitleLabel {
        text: qsTr("Sounds")
    }

    ListView {
        id: listView
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true
        delegate: ItemDelegate {
            anchors {
                left: parent.left
                right: parent.right
            }
            text: model.text
            highlighted: model.sound === root.model.currentSound

            onClicked: {
                if (highlighted) {
                    currentSoundClicked();
                } else {
                    root.model.setCurrentRow(model.index);
                }
            }

            Button {
                anchors {
                    right: parent.right
                    rightMargin: innerMargin
                    verticalCenter: parent.verticalCenter
                }
                height: parent.height - 2 * innerMargin
                width: height
                flat: true
                text: "⨯"

                onClicked: {
                    root.model.remove(model.index);
                }
            }
        }

        ScrollIndicator.vertical: ScrollIndicator {}
    }
}
