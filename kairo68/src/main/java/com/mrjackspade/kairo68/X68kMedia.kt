package com.mrjackspade.kairo68

import com.mrjackspade.kairo.frontend.DiskImageLibrary
import com.mrjackspade.kairo.frontend.R as FrontendR
import com.mrjackspade.kairo.frontend.UiText

/** The library's media format, as the app's launch builds it (Kairo#125). */
object X68kMedia {
    val format = DiskImageLibrary.Format(
        kinds = listOf("xdf", "dim", "d88", "88d", "hdm", "2hd", "dup").associateWith { "fd" } + mapOf("hdf" to "hd"),
        labels = mapOf("fd" to UiText.res(FrontendR.string.kairo_floppy_disk), "hd" to UiText.res(FrontendR.string.kairo_hard_disk)),
        maxBytes = mapOf("fd" to 64L * 1024 * 1024, "hd" to 4L * 1024 * 1024 * 1024),
        guestName = "X68000")
}
