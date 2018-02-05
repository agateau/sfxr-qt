import QtQuick 2.4
import QtQuick.Controls 2.0

import sfxr 1.0

ListView {
    id: root
    signal soundClicked(Sound sound)
    delegate: Button {
        text: model.text
        onClicked: {
            root.soundClicked(model.sound);
        }
    }
}
