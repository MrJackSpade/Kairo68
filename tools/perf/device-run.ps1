<#
.SYNOPSIS
Kairo68's repeatable game run on an Android device (Kairo68#28).

.DESCRIPTION
Pushes the measurement builds of core/test/game_profile (built with the NDK, KAIRO68_TOOLS=ON,
plain and with KAIRO68_PERF), the X68000 firmware and the test game to the device, then runs the
game's attract demo at each CPU clock: the plain build Repeats times (the frame rate and its
spread) and the counting build once (where the time goes, and what counting costs). Writes every
run's output to <Out>\<clock>-<build>-<n>.txt and prints each run's summary line.

The game's attract demo plays the same gameplay with no input, so every build runs the same frames.

.EXAMPLE
tools\perf\device-run.ps1 -Device 192.168.1.118:5555 -Plain D:\kairo68-perf\game_profile-plain.stripped -Perf D:\kairo68-perf\game_profile-perf.stripped -Out D:\kairo68-perf\runs\baseline
#>
param(
    [Parameter(Mandatory)][string]$Device,
    [Parameter(Mandatory)][string]$Plain,
    [Parameter(Mandatory)][string]$Perf,
    [Parameter(Mandatory)][string]$Out,
    [string]$Game = (Join-Path $PSScriptRoot '..\..\roms\aliensyndrome\Alien Syndrome (1992)(Dempa).dim'),
    [string]$Bios = (Join-Path $PSScriptRoot '..\..\bios\x68000'),
    [int[]]$Clocks = @(10, 16, 25),
    [int]$Repeats = 3,
    [int]$Warmup = 300,
    [int]$Frames = 1200,
    # Shared libraries the builds need (build-variant.ps1's libs-<Name>), pushed beside them.
    [string]$Libs
)
$ErrorActionPreference = 'Stop'
$remote = '/data/local/tmp/k68'
# The executable by its path: a bare "adb" inside the function named Adb would call the function.
$adb = (Get-Command adb -CommandType Application | Select-Object -First 1).Source
function Adb { $out = & $adb -s $Device @args 2>&1; if ($LASTEXITCODE -ne 0) { throw "adb $args failed: $out" }; $out }

New-Item -ItemType Directory -Force $Out | Out-Null
Adb shell "mkdir -p $remote/bios/x68000 $remote/work" | Out-Null
Adb push (Join-Path $Bios 'iplrom.dat') (Join-Path $Bios 'cgrom.dat') "$remote/bios/x68000/" | Out-Null
Adb push $Game "$remote/game.dim" | Out-Null
Adb push $Plain "$remote/game_profile-plain" | Out-Null
Adb push $Perf "$remote/game_profile-perf" | Out-Null
Adb shell "chmod 755 $remote/game_profile-plain $remote/game_profile-perf" | Out-Null
Adb shell "rm -rf $remote/lib; mkdir -p $remote/lib" | Out-Null
if ($Libs) { foreach ($lib in Get-ChildItem $Libs -Filter '*.so') { Adb push $lib.FullName "$remote/lib/" | Out-Null } }

foreach ($clock in $Clocks) {
    $runs = @(1..$Repeats | ForEach-Object { [pscustomobject]@{ Build = 'plain'; N = $_ } }) +
        @([pscustomobject]@{ Build = 'perf'; N = 1 })
    foreach ($run in $runs) {
        $build = $run.Build; $n = $run.N
        $text = Adb shell "cd $remote && rm -rf work/run && mkdir -p work/run && LD_LIBRARY_PATH=$remote/lib ./game_profile-$build bios work/run game.dim $clock 4 $Frames - --warmup $Warmup"
        $file = Join-Path $Out "$clock-$build-$n.txt"
        $text | Set-Content -Encoding utf8 $file
        $summary = ($text | Select-String '^host ').Line
        "$clock MHz $build #${n}: $summary"
    }
}
