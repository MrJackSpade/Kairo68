package com.mrjackspade.kairo68

import com.mrjackspade.kairo.frontend.FirmwareSet
import com.mrjackspade.kairo.frontend.R as FrontendR
import com.mrjackspade.kairo.frontend.UiText
import java.io.File

/** The firmware the app's Machine page lists, as its launch builds it (Kairo#125). */
object X68kFirmware {
    /** Each set, titled, with its files in [directory]. */
    fun sets(directory: File): List<Pair<UiText, FirmwareSet>> {
        fun rom(name: String, label: UiText, description: Int, size: Int) = FirmwareSet.Part(name, label,
            UiText.res(FrontendR.string.kairo_firmware_part, name, UiText.res(description, "X68000")), setOf(name), true, setOf(size.toLong()))
        return listOf(UiText.res(FrontendR.string.kairo_firmware_title, "X68000") to FirmwareSet(directory, listOf(
            rom("iplrom.dat", UiText.fixed("IPL"), R.string.firmware_ipl_description, 0x20000),
            rom("cgrom.dat", UiText.res(R.string.firmware_font), R.string.firmware_font_description, 0xC0000))))
    }
}
