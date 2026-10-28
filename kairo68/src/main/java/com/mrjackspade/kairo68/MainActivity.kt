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
import com.mrjackspade.kairo.frontend.UiText
import com.mrjackspade.kairo.frontend.mediaFor
import com.mrjackspade.kairo.frontend.R as FrontendR

import java.io.File
import java.util.concurrent.atomic.AtomicBoolean

/** Kairo68: the Sharp X68000 on MAME's x68k driver, in the shared Kairo frontend. */
class MainActivity : MachineActivity<DiskImageEntry>() {
    override val identity = Identity("KAIRO68", 0xffFA2833.toInt(), "Kairo68", "X68000", "kairo68",
        LibraryStrings("KAIRO68", "Kairo68"),
        UiText.res(R.string.credits, "Kairo68", "MAME", "X68000", "MrJackSpade/Kairo68"),
        UiText.res(R.string.library_empty), UiText.res(R.string.library_expired),
        MachineFirstRun.Folder(UiText.res(R.string.folder_title), UiText.res(R.string.folder_description, "X68000", "Kairo68"),
            UiText.res(R.string.folder_button)),
        UiText.res(R.string.saves_group), "MrJackSpade/Kairo68")
    override val machine = Machine(768, 512, 4.0 / 3.0, "state.sta")
    override val bridge by lazy { NativeMachine("kairo68") }
    override val keyboard = X68kKeys.layout
    override val maxKeyCode = X68kKeys.MAX_CODE
    override fun guestKey(androidKey: Int) = X68kKeys.fromAndroid(androidKey)
    override val joystickControls get() = X68kKeys.joystick
    override val controllerSpec = ControllerGuestSpec("X68000", X68kKeys.bindableCodes, { X68kKeys.label(it) ?: "Key $it" },
        setOf(X68kKeys.SHIFT, X68kKeys.CTRL, X68kKeys.OPT1, X68kKeys.OPT2), X68kKeys.joystick.zip(X68kKeys.joystickLabels),
        UiText.res(R.string.joystick_help), ControllerGuestSpec.STANDARD_ACTIONS, X68kKeys.layout)
    override val bindingsCodec = X68kKeys.bindings
    override val artworkDownloadsEnabled: Boolean get() = resources.getBoolean(R.bool.catalog_art_download_enabled)

    /** Floppy images in the formats MAME's X68000 reads, and SASI hard disk images. */
    private val format = DiskImageLibrary.Format(
        kinds = listOf("xdf", "dim", "d88", "88d", "hdm", "2hd", "dup").associateWith { "fd" } + mapOf("hdf" to "hd"),
        labels = mapOf("fd" to UiText.res(R.string.floppy_disk), "hd" to UiText.res(R.string.hard_disk)),
        maxBytes = mapOf("fd" to 64L * 1024 * 1024, "hd" to 4L * 1024 * 1024 * 1024),
        guestName = "X68000")
    private val disks by lazy { DiskImageLibrary(this, format) }
    override fun createCatalog() = LayeredGameCatalog(this, X68kCatalog)
    override fun createLibrary(catalog: LayeredGameCatalog) = disks

    private fun isHardDisk(entry: DiskImageEntry) = format.kind(entry.displayName) == "hd"

    override val machineOptions = X68kCatalog.options
    // In the core's drive order (x68k_machine.h Drive).
    override val mediaSlots = listOf(MediaSlot<DiskImageEntry>(UiText.res(R.string.floppy_0)) { !isHardDisk(it) },
        MediaSlot(UiText.res(R.string.floppy_1)) { !isHardDisk(it) }, MediaSlot(UiText.res(R.string.hard_disk), ::isHardDisk))

    /** MAME's ROM path: it reads the X68000 set from its x68000 folder. */
    private val romDir by lazy { File(filesDir, "firmware") }
    private val firmwareDir by lazy { File(romDir, "x68000") }
    override val firmwareSets by lazy {
        fun rom(name: String, label: UiText, description: Int, size: Int) = FirmwareSet.Part(name, label,
            UiText.res(R.string.firmware_part, name, UiText.res(description, "X68000")), setOf(name), true, setOf(size.toLong()))
        listOf(UiText.res(FrontendR.string.kairo_firmware_title, "X68000") to FirmwareSet(firmwareDir, listOf(
            rom("iplrom.dat", UiText.fixed("IPL"), R.string.firmware_ipl_description, 0x20000),
            rom("cgrom.dat", UiText.res(R.string.firmware_font), R.string.firmware_font_description, 0xC0000))))
    }

    override fun startGame(entry: DiskImageEntry, game: CatalogGame, cancelled: AtomicBoolean,
                           progress: (String) -> Unit) {
        requireFirmware()
        val floppy0 = entry.takeUnless(::isHardDisk)
        val floppy1 = game.mediaId("floppyB")?.let { id ->
            mediaFor(id, entry, libraryEntries) ?: error(getString(R.string.second_disk_missing))
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
        2 -> getString(R.string.state_error_io)
        else -> getString(R.string.state_error_code, code)
    }
}
