import QtQuick 2.4
import QtQuick.Window 2.3

import sfxr2 1.0

Window {
    visible: true
    width: 600
    height: 400

    Synthesizer {
        id: synth
    }

    MainWindow {
        pickupButton.onClicked: {
            synth.generatePickup();
        }
        laserButton.onClicked: {
            synth.generateLaser();
        }
        explosionButton.onClicked: {
            synth.generateExplosion();
        }
        powerupButton.onClicked: {
            synth.generatePowerup();
        }
        hitHurtButton.onClicked: {
            synth.generateHitHurt();
        }
        jumpButton.onClicked: {
            synth.generateJump();
        }
        blipSelectButton.onClicked: {
            synth.generateBlipSelect();
        }
    }
}
