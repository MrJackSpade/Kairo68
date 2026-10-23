package com.mrjackspade.kairo68.uitest

import android.os.SystemClock
import com.mrjackspade.kairo.uitest.AppUnderTest
import com.mrjackspade.kairo.uitest.AppUnderTest.BiosFile
import com.mrjackspade.kairo.uitest.KairoUiTest
import com.mrjackspade.kairo.uitest.suites.FirstRunTests
import com.mrjackspade.kairo.uitest.suites.GameTests
import com.mrjackspade.kairo.uitest.suites.LibraryTests
import com.mrjackspade.kairo.uitest.suites.SearchTests
import com.mrjackspade.kairo.uitest.suites.WordingTests
import org.junit.Test

/**
 * Kairo68 as the shared UI tests see it. Its test resources are TOSEC archives: Alien Syndrome,
 * and Mahjong Saikyou Musashi on two disks, with the X68000 IPL and character font.
 */
object Kairo68App : AppUnderTest() {
    override val name = "Kairo68"
    override val brand = "KAIRO68"
    override val biosTitle = "X68000 BIOS"
    override val biosFiles = listOf(BiosFile("iplrom.dat", true), BiosFile("cgrom.dat", true))
    override val bootGame = "Alien Syndrome"
    override val singleEntryGames = listOf("Mahjong Saikyou Musashi")
    override val searchTerm = "Alien"
}

class Kairo68FirstRunTests : FirstRunTests() { override val app = Kairo68App }
class Kairo68LibraryTests : LibraryTests() { override val app = Kairo68App }
class Kairo68SearchTests : SearchTests() { override val app = Kairo68App }
class Kairo68GameTests : GameTests() { override val app = Kairo68App }
class Kairo68WordingTests : WordingTests() { override val app = Kairo68App }

/** The X68000's own behavior: a second disk at launch, and a boot faster than the real machine. */
class X68000Tests : KairoUiTest() {
    override val app = Kairo68App

    @Test
    fun aTwoDiskGameStartsWithItsSecondDiskInFloppy1() {
        screens.completeFirstRun().open("Mahjong Saikyou Musashi").play()
        val probe = driver.probe()
        expect.that("floppy 0 holds disk 1", probe.details["media.Floppy 0"].orEmpty().contains("Disk 1 of 2")) { probe.details["media.Floppy 0"].orEmpty() }
        expect.that("floppy 1 holds disk 2", probe.details["media.Floppy 1"].orEmpty().contains("Disk 2 of 2")) { probe.details["media.Floppy 1"].orEmpty() }
    }

    /**
     * The X68000 draws 55.46 frames a second. While it boots it runs at full host speed, so the
     * first seconds after starting must run faster than that; the measured rate is logged.
     */
    @Test
    fun theBootRunsFasterThanTheRealMachine() {
        screens.completeFirstRun().open(Kairo68App.bootGame).play()
        val startFrames = driver.probeNow().details.getValue("frames").toLong()
        val startTime = SystemClock.elapsedRealtime()
        driver.waitFrames(166)
        val seconds = (SystemClock.elapsedRealtime() - startTime) / 1000.0
        val rate = (driver.probeNow().details.getValue("frames").toLong() - startFrames) / seconds
        driver.log.step("measure", "boot ran at %.1f frames a second".format(rate))
        expect.that("the boot runs faster than the real X68000's 55.46 frames a second", rate > 55.46 * 1.1) { "%.1f".format(rate) }
    }
}
