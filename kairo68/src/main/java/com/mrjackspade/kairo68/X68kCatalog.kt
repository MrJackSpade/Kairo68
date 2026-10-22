package com.mrjackspade.kairo68

import com.mrjackspade.kairo.frontend.ChoiceOption
import com.mrjackspade.kairo.frontend.LayeredGameCatalog
import com.mrjackspade.kairo.frontend.MachineOption
import org.json.JSONArray

/**
 * Kairo68's catalog records beyond the common fields: the machine a game boots (`machine`:
 * `memory` in megabytes) and the disk for floppy drive 1 (media role `floppyB`) for games that
 * start with two.
 */
object X68kCatalog : LayeredGameCatalog.Schema {
    override val platform = "x68000"
    override val updateUrl = "https://raw.githubusercontent.com/MrJackSpade/Kairo68/main/catalog/core-v3.json"
    override val metadataUrl = "https://raw.githubusercontent.com/MrJackSpade/Kairo68/main/catalog/core-v3.meta.json"
    override val coreVersion = 3
    override val selfHostedArt = "/MrJackSpade/Kairo68/main/catalog/artwork/art/catalog/"
    override val contentId = Regex("sha256-(?:fd|hd)-v1:[0-9a-f]{64}")
    override val mediaExtensions = Regex("\\.(?:zip|7z|xdf|dim|d88|88d|hdm|2hd|dup|hdf)$", RegexOption.IGNORE_CASE)

    /** Main memory: MAME's X68000 takes 1 to 12 MB. */
    val memory = MachineOption("memory", "Memory", (1..12).map { ChoiceOption("$it", "$it MB") }, "4", numeric = true)
    val options = listOf(memory)

    override val fields: Map<String, Set<String>?> = mapOf(MachineOption.SECTION to options.map { it.id }.toSet())
    override val singleMediaRoles = setOf("floppyB")

    override fun validMedia(role: String, contentId: String) = role == "floppyB" && contentId.startsWith("sha256-fd-v1:")

    override fun validField(field: String, value: Any): Boolean =
        field == MachineOption.SECTION && MachineOption.validSection(options, value)

    override fun validBindings(bindings: JSONArray) = X68kKeys.bindings.valid(bindings)
}
