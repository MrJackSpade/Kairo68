package com.mrjackspade.kairo68

import com.mrjackspade.kairo.KanaLegendsTest
import com.mrjackspade.kairo.LaunchTablesTest
import java.io.File

/** What Kairo68 builds as it launches (Kairo#125, Kairo68#82). */
class X68kLaunchTablesTest : LaunchTablesTest() {
    override fun tables() = LaunchTables(listOf(X68kMedia.format), listOf(X68kKeys.layout),
        X68kFirmware.sets(File("x68000")).map { it.second }, mapOf("controller bindings" to { X68kKeys.bindings }))
}

/** The X68000 keyboard's kana legends (Kairo#126, Kairo68#89). */
class X68kKanaLegendsTest : KanaLegendsTest() {
    override val layouts = listOf(X68kKeys.layout)
}
