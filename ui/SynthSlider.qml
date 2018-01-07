import QtQuick 2.4
import QtQuick.Controls 2.2

import sfxr2 1.0

Slider {
    property Synthesizer synth
    property string synthProperty
    property string name
    property bool bipolar: false

    from: bipolar ? -1 : 0

    value: synth[synthProperty]

    onValueChanged: {
        synth[synthProperty] = value;
    }
}
