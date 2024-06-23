# Changelog

## 1.5.1 - 2024-06-24

### Fixed

- Fix rendering of very short sounds (#21) (Aurélien Gâteau)
- Fix 22.5 khz wav files being reported as truncated by some apps (#22) (Aurélien Gâteau)

### Internal

- Use system version of Catch2 (Matthias Mailänder)
- Fix build failure (Alex Myczko)
- Port to Catch2 3.4.0 (Aurélien Gâteau)

## 1.5.0 - 2022-03-18

### Added

- Make it possible to adjust sliders with the mouse wheel (Aurélien Gâteau).
- Show a preview of the sound on top of the sliders (Aurélien Gâteau).

### Fixed

- Exit on multiple positional arguments (Linus Vanas).
- Fix CLI needing a graphical environment to work (Linus Vanas).
- Add a couple more error messages to the CLI (Linus Vanas).
- Fix wav final volume being higher than the preview (Aurélien Gâteau).


## 1.4.0 - 2022-01-12

### Added

- SFXR-Qt can now export wavs from the command line (#9) (Linus Vana).
- Added tooltips (Aurélien Gâteau).
- Implemented "Randomize", based on the original SFXR code (Aurélien Gâteau).
- Make it possible to load sounds from the command line (Aurélien Gâteau).

### Fixed

- Fixed crash when clicking too fast on the remove button (#8) (Aurélien Gâteau).
- Fixed build on big-endian machines (#7) (Aurélien Gâteau).

### Internal

- Reorganized source tree to match cookiecutter-qt-app layout (Aurélien Gâteau).
- Updated clang-format config to group and sort includes (Aurélien Gâteau).
- Use GitHub Actions (Aurélien Gâteau).

## 1.3.0 - 2019-09-15

### Added

- Changed to a new, JSON based, file format: SFXJ.
- Added a new wave form: triangle.
- Added continuous integration using Travis CI.

### Changed

- Open and save file dialogs now show an error message when something went wrong.

### Internal

- Ported loading and saving code from FILE to QFile.
- Applied my coding style.
- Split the code into a static lib and an app.
- Added tests.
- Cleaned some float vs qreal warnings.

## 1.2.0 - 2018-10-29

### Added

- Added a "Save" button.
- You can now open the file-open dialog with Ctrl+O and the file-save dialog with Ctrl+S.
- Show real filename in sound buttons.

### Changed

- Rework noise buffer so that output is reproducible: when looping the same sound should be replayed.
- Draw a thin line in the middle of bipolar sliders.

### Fixed

- Follow OS color theme, making the application usable if the theme is dark.
- Fixed invisible "close sound" button by using a different symbol, which works in all tested themes.
- Make sure user interface elements do not overlap.
- Fixed pip install doc (thanks to feinstaub).
- Workaround for a bug with Slider which made it behave strangely when values
  became negative.

## 1.1.0 - 2018-02-09

### Added
- Multiple sound support: clicking on a generator button creates a new sound. You can switch between the different sounds and easily drop the bad ones.

- Added a way to play a sound in loop.

### Changed
- Removed randomize, it's useless.

## 1.0.0 - 2018-01-13

- Ported to QtQuick

==============================================================================

# historical sfxr-sdl changelog

## 1.2.1
- Fixed bug, the loop function call in main was commented out by Johan Helsing
  (bobbaluba@gmail.com)
- Fixed broken target clean in Makefile

## 1.2
- Added support for gtk+-3.0 by Tim Oertel (toertel-sfxr@manax.org
  http://www.heroicproportions.org) (Sorry, you have to edit the makefile to
  switch back to gtk+-2.0)

## 1.1
- Various small improvements and 1 bugfix by Hans de Goede
  <hdegoede@redhat.com>:
  - Fix a small bug in the audio setup which could cause a quite noticable
    delay between pressing a button and hearing the sound
  - Add an icon and .desktop file
  - Add a make install target, note:  hardcoded to /usr but it does understand
    the DESTDIR make parameter

## 1.0
- Initial SDL port of sfxr by mjau/GerryJJ
