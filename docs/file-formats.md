# File formats

The original SFXR tool as well as SFXR-Qt up to 1.2.0 uses the SFXR file format.
This file format is a binary representation of the sound effect.

Since version 1.3.0, SFXR-Qt uses its own format: SFXJ. This file format uses
JSON to represent the sound effect. It is expected that JSON will make it
easier to process and extend the file format.

SFXR-Qt can still load SFXR files.

## SFXJ file format

This is what an SFXJ file version 1 looks like:

```
{
    "version": 1,
    "properties": {
        "attackTime": 0,
        "baseFrequency": 0.5019000172615051,
        "changeAmount": 0.5941200256347656,
        "changeSpeed": 0.5493800044059753,
        "decayTime": 0.2362000048160553,
        "deltaSlide": 0,
        "dutySweep": 0,
        "hpFilterCutoff": 0,
        "hpFilterCutoffSweep": 0,
        "lpFilterCutoff": 1,
        "lpFilterCutoffSweep": 0,
        "lpFilterResonance": 0,
        "minFrequency": 0,
        "phaserOffset": 0,
        "phaserSweep": 0,
        "repeatSpeed": 0,
        "slide": 0,
        "squareDuty": 0,
        "sustainPunch": 0.46599000692367554,
        "sustainTime": 0.05916000157594681,
        "vibratoDepth": 0,
        "vibratoSpeed": 0,
        "volume": 0.5,
        "waveForm": "Square"
    }
}
```
