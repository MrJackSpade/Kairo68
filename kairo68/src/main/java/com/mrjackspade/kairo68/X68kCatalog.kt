package com.mrjackspade.kairo68

import com.mrjackspade.kairo.frontend.CatalogSetting
import com.mrjackspade.kairo.frontend.ChoiceOption
import com.mrjackspade.kairo.frontend.LayeredGameCatalog
import com.mrjackspade.kairo.frontend.MachineOption
import com.mrjackspade.kairo.frontend.UiText
import org.json.JSONArray

/**
 * Kairo68's catalog records beyond the common fields: the machine a game boots (`machine`:
 * `memory` in megabytes) and the disk for floppy drive 1 (media role `floppyB`) for games that
 * start with two.
 */
object X68kCatalog : LayeredGameCatalog.Schema {
    override val platform = "x68000"
    override val catalogUrl = "https://raw.githubusercontent.com/MrJackSpade/Kairo68/main/catalog/"
    override val selfHostedArt = "/MrJackSpade/Kairo68/main/catalog/artwork/art/catalog/"
    override val contentId = Regex("sha256-(?:fd|hd)-v1:[0-9a-f]{64}")
    override val mediaExtensions = Regex("\\.(?:zip|7z|xdf|dim|d88|88d|hdm|2hd|dup|hdf)$", RegexOption.IGNORE_CASE)

    /** Main memory: MAME's X68000 takes 1 to 12 MB. */
    val memory = MachineOption("memory", UiText.res(R.string.option_memory),
        (1..12).map { ChoiceOption("$it", UiText.res(R.string.memory_mb, it)) }, "4", numeric = true)
    /** The 68000's clock: the X68000's 10 MHz, the XVI's 16, or the X68030's 25, the fastest by default. */
    val cpu = MachineOption("cpu", UiText.res(R.string.option_clock),
        listOf(25, 16, 10).map { ChoiceOption("$it", UiText.res(R.string.clock_mhz, it)) }, "25", numeric = true)
    val skipMemoryCheck = MachineOption.skipMemoryCheck()
    val options = listOf(memory, cpu, skipMemoryCheck)

    override val settings: List<CatalogSetting> = options.map(CatalogSetting::Choice) + 
        CatalogSetting.Drive("floppyB", UiText.res(R.string.floppy_1))
    override val fields = CatalogSetting.fields(settings)

    override fun validMedia(role: String, contentId: String) = role == "floppyB" && contentId.startsWith("sha256-fd-v1:")

    override fun validField(field: String, value: Any): Boolean =
        field in fields && CatalogSetting.validSection(settings, field, value)

    override fun validBindings(bindings: JSONArray) = X68kKeys.bindings.valid(bindings)
}
