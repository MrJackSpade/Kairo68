package com.mrjackspade.kairo68

import com.mrjackspade.kairo.frontend.CatalogGame
import com.mrjackspade.kairo.frontend.ControllerGuestSpec
import com.mrjackspade.kairo.frontend.ControllerLayout
import com.mrjackspade.kairo.frontend.DiskImageEntry
import com.mrjackspade.kairo.frontend.DiskImageLibrary
import com.mrjackspade.kairo.frontend.FirmwareSet
import com.mrjackspade.kairo.frontend.LayeredGameCatalog
import com.mrjackspade.kairo.frontend.LibraryStrings
import com.mrjackspade.kairo.frontend.MachineActivity
import com.mrjackspade.kairo.frontend.MachineFirstRun
import com.mrjackspade.kairo.frontend.NativeMachine
import com.mrjackspade.kairo.frontend.mediaFor

import java.io.File
import java.util.concurrent.atomic.AtomicBoolean

/** Kairo68: the Sharp X68000 on MAME's x68k driver, in the shared Kairo frontend. */
class MainActivity : MachineActivity<DiskImageEntry>() {
    override val identity = Identity("KAIRO68", 0xffFA2833.toInt(), "Kairo68", "X68000", "kairo68",
        LibraryStrings("KAIRO68", "Kairo68"),
        "Kairo68 uses MAME's X68000 emulation. Source and provenance: github.com/MrJackSpade/Kairo68.",
        "Choose a games folder to find floppy and hard disk images and the ZIP and 7z archives holding them",
        "Folder access expired. Select the games folder again.",
        MachineFirstRun.Folder("Choose a games folder", "Select the folder containing your X68000 games. Kairo68 " +
            "will scan its floppy and hard disk images and the ZIP and 7z archives holding them.", "Choose games folder…"),
        "SAVE STATES", "MrJackSpade/Kairo68")
    override val machine = Machine(768, 512, 4.0 / 3.0, "state.sta")
    override val bridge by lazy { NativeMachine("kairo68") }
    override val keyboard = X68kKeys.layout
    override val maxKeyCode = X68kKeys.MAX_CODE
    override fun guestKey(androidKey: Int) = X68kKeys.fromAndroid(androidKey)
    override val joystickControls get() = X68kKeys.joystick
    override val controllerSpec = ControllerGuestSpec("X68000", X68kKeys.bindableCodes, { X68kKeys.label(it) ?: "Key $it" },
        setOf(X68kKeys.SHIFT, X68kKeys.CTRL, X68kKeys.OPT1, X68kKeys.OPT2), X68kKeys.joystick.zip(X68kKeys.joystickLabels),
        "The pad in joystick port 1.", ControllerGuestSpec.STANDARD_ACTIONS, X68kKeys.layout)
    override val bindingsCodec = X68kKeys.bindings
    override val artworkDownloadsEnabled: Boolean get() = resources.getBoolean(R.bool.catalog_art_download_enabled)

    /** Floppy images in the formats MAME's X68000 reads, and SASI hard disk images. */
    private val disks by lazy {
        DiskImageLibrary(this, DiskImageLibrary.Format(
            kinds = listOf("xdf", "dim", "d88", "88d", "hdm", "2hd", "dup").associateWith { "fd" } + mapOf("hdf" to "hd"),
            labels = mapOf("fd" to "Floppy disk", "hd" to "Hard disk"),
            maxBytes = mapOf("fd" to 64L * 1024 * 1024, "hd" to 4L * 1024 * 1024 * 1024),
            guestName = "X68000"))
    }
    override fun createCatalog() = LayeredGameCatalog(this, X68kCatalog)
    override fun createLibrary(catalog: LayeredGameCatalog) = disks

    private fun isHardDisk(entry: DiskImageEntry) = entry.mediaLabel == "Hard disk"

    override val machineOptions = X68kCatalog.options
    override val mediaPageTitle = "Disks"
    // In the core's drive order (x68k_machine.h Drive).
    override val mediaSlots = listOf(MediaSlot<DiskImageEntry>("Floppy 0") { !isHardDisk(it) },
        MediaSlot("Floppy 1") { !isHardDisk(it) }, MediaSlot("Hard disk", ::isHardDisk))

    /** MAME's ROM path: it reads the X68000 set from its x68000 folder. */
    private val romDir by lazy { File(filesDir, "firmware") }
    private val firmwareDir by lazy { File(romDir, "x68000") }
    override val firmwareSets by lazy {
        fun rom(name: String, label: String, description: String, size: Int) =
            FirmwareSet.Part(name, label, "$name · $description", setOf(name), true, setOf(size.toLong()))
        listOf("X68000 BIOS" to FirmwareSet(firmwareDir, listOf(
            rom("iplrom.dat", "IPL", "the BIOS that starts the X68000 and loads games", 0x20000),
            rom("cgrom.dat", "Character font", "the X68000's text and kanji font", 0xC0000))))
    }

    override fun startGame(entry: DiskImageEntry, game: CatalogGame, cancelled: AtomicBoolean,
                           progress: (String) -> Unit) {
        requireFirmware()
        val floppy0 = entry.takeUnless(::isHardDisk)
        val floppy1 = game.mediaId("floppyB")?.let { id ->
            mediaFor(id, entry, libraryEntries) ?: error("The second disk this game needs is not in the library")
        }
        val hardDisk = entry.takeIf(::isHardDisk)
        val media = listOf(floppy0, floppy1, hardDisk)
        val images = media.map { disk -> disk?.let { disks.prepare(it, cancelled, progress).absolutePath } }
        val home = File(filesDir, "mame").apply { mkdirs() }
        check(bridge.start(romDir.absolutePath, home.absolutePath, images, optionValues(game))) {
            "Another machine is still running"
        }
        awaitRunning()
        mediaStarted(media)
    }

    override fun stateError(code: Int) = when (code) {
        2 -> "the emulator could not read or write the state"
        else -> "code $code"
    }
}
