# Changelog

## 1.1.0 - 2018-02-09

### Added
- Multiple sound support: clicking on a generator button creates a new sound.
  You can switch between the different sounds and easily drop the bad ones.

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
