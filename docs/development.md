# Development

## Building

Build the app, and optionally install it, with the shared build script (see [Android build](../shared/docs/android-build.md)):

```powershell
shared\tools\build.ps1 -Install <adb serial>
```

## Host tests

The machine also builds for the host, without Android: build `core/` with CMake for the host. Run them under `shared\tools\run-limited.ps1`.

- `boot_test <rom dir> <work dir> <image> <seconds>` boots a floppy or SASI hard disk, presses the pad's A button, Space and Return, checks that a save state loads back to the same picture and sound, and starts a second machine in the same process, as the app does for its next game.
- `boot_profile <rom dir> <work dir> <image> <cpu MHz> <memory MB> <max guest seconds>` measures a boot: the guest and host seconds to the first picture that is not black and to the end of the boot, with both pictures written as PPM files.

<!-- kairo:ui-tests -->
## UI tests

Kairo68 has device tests in the style of Selenium tests, on the shared Kairo test framework: they
name the app's elements, act on them as a player would, and assert what the player would see
([how they work and how to run them](https://github.com/MrJackSpade/Kairo/blob/main/docs/ui-tests.md)). Run them from this
repository with a device connected over adb (Android 10 or later) and the `LOCAL_BUILD_TOKEN` key set:

```
shared\tools\uitest.ps1 -Device <serial>                                              every test
shared\tools\uitest.ps1 -Device <serial> -Filter 'MenuTests|ControllerTests'          by test name
shared\tools\uitest.ps1 -Device <serial> -Class com.mrjackspade.kairo68.uitest.<Class>   one class
shared\tools\uitest-all.ps1 -Device <serial>                                          every Kairo app
```

The tests are in `kairo68/src/androidTest/java/com/mrjackspade/kairo68/uitest/`. They run in a
build of their own, `com.loxifi.kairo68.uitest`, installed beside the app without touching its data, and
each run's logs, screenshots and summary land in `.build/uitest/<run id>/`.
<!-- /kairo:ui-tests -->
