# Kairo68

**Play your own X68000 games on Android phones and handhelds.** Kairo68 is an Android emulator for the Sharp X68000, built on the [Kairo shared frontend](https://github.com/MrJackSpade/Kairo): the same game library, controller and touch controls, on-screen keyboard, and per-game settings as [Kairo98](https://github.com/MrJackSpade/Kairo98) and [KairoDos](https://github.com/MrJackSpade/KairoDos). It runs a pinned copy of [MAME](https://www.mamedev.org/)'s X68000 driver. It is an independent project, not an official MAME release.

## Status

Early development. There is no release yet. The emulator core is pinned in this repository, and the Android app is being built around it.

## Games and firmware

Games, operating systems, and firmware are not supplied. The X68000 needs its system ROMs (IPL and character generator), which you provide from your own machine.

The pinned driver reads XDF and DIM floppy images and SASI hard disk images.

## Documentation

- [Licensing and credits](docs/licensing.md): code license and third-party components
- [Source provenance](docs/source-import.md): where the emulator source came from and what was copied

## Help and community

- **Chat** with other players on [Discord](https://discord.gg/ZkYQ3bPTTq).

### Related projects

- [Kairo98](https://github.com/MrJackSpade/Kairo98): PC-98 emulator for Android
- [KairoDos](https://github.com/MrJackSpade/KairoDos): DOS emulator for Android
- [Kairo88](https://github.com/MrJackSpade/Kairo88): PC-88 emulator for Android
- [KairoTowns](https://github.com/MrJackSpade/KairoTowns): FM Towns emulator for Android
- [KairoMSX](https://github.com/MrJackSpade/KairoMSX): MSX emulator for Android
- [Kairo](https://github.com/MrJackSpade/Kairo): shared frontend used by every Kairo app

## Building from source

Clone with submodules to get the pinned shared frontend:

```sh
git clone --recurse-submodules https://github.com/MrJackSpade/Kairo68.git
```

The Android build is not set up yet.

## License

First-party code is [GPL-2.0-or-later](LICENSE.md). Third-party components keep their own licenses; see [licensing and credits](docs/licensing.md).
