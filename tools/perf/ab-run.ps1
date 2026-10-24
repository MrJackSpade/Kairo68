<#
.SYNOPSIS
Compares measurement builds of core/test/game_profile on the device, interleaved (Kairo68#6-#9).

.DESCRIPTION
-Builds maps a name to a stripped game_profile (and, for a shared-library variant, -Libs maps the
same name to its libs-<Name> folder). Each round runs every build once, in order, so the device's
drift reaches all of them alike; prints each run's frame rate and each build's mean and spread.

.EXAMPLE
tools\perf\ab-run.ps1 -Device 192.168.1.118:5555 -Builds @{ o3 = 'D:\kairo68-perf\game_profile-plain.stripped'; o2 = 'D:\kairo68-perf\game_profile-o2.stripped' }
#>
param(
    [Parameter(Mandatory)][string]$Device,
    [Parameter(Mandatory)][hashtable]$Builds,
    [hashtable]$Libs = @{},
    [string]$Game = (Join-Path $PSScriptRoot '..\..\roms\aliensyndrome\Alien Syndrome (1992)(Dempa).dim'),
    [string]$Bios = (Join-Path $PSScriptRoot '..\..\bios\x68000'),
    [int]$Clock = 25,
    [int]$Rounds = 3,
    [int]$Warmup = 300,
    [int]$Frames = 1200,
    # More game_profile options for every run, such as --boot-frames for a game whose boot never ends.
    [string[]]$Extra = @()
)
$ErrorActionPreference = 'Stop'
$remote = '/data/local/tmp/k68'
# The executable by its path: a bare "adb" inside the function named Adb would call the function.
$adb = (Get-Command adb -CommandType Application | Select-Object -First 1).Source
function Adb { $out = & $adb -s $Device @args 2>&1; if ($LASTEXITCODE -ne 0) { throw "adb $args failed: $out" }; $out }

Adb shell "mkdir -p $remote/bios/x68000 $remote/work" | Out-Null
Adb push (Join-Path $Bios 'iplrom.dat') (Join-Path $Bios 'cgrom.dat') "$remote/bios/x68000/" | Out-Null
Adb push $Game "$remote/game.dim" | Out-Null
$names = @($Builds.Keys | Sort-Object)
foreach ($name in $names) {
    Adb push $Builds[$name] "$remote/ab-$name" | Out-Null
    Adb shell "chmod 755 $remote/ab-$name; rm -rf $remote/ablib-$name; mkdir -p $remote/ablib-$name" | Out-Null
    if ($Libs.ContainsKey($name)) {
        foreach ($lib in Get-ChildItem $Libs[$name] -Filter '*.so') { Adb push $lib.FullName "$remote/ablib-$name/" | Out-Null }
    }
}
$rates = @{}
foreach ($round in 1..$Rounds) {
    foreach ($name in $names) {
        $text = Adb shell "cd $remote && rm -rf work/run && mkdir -p work/run && LD_LIBRARY_PATH=$remote/ablib-$name ./ab-$name bios work/run game.dim $Clock 4 $Frames - --warmup $Warmup $($Extra -join ' ')"
        $line = ($text | Select-String '^host ').Line
        $rate = [double]([regex]::Match($line, '([0-9.]+) frames/s').Groups[1].Value)
        $rates[$name] += @($rate)
        "round $round $name : $rate frames/s"
    }
}
foreach ($name in $names) {
    $r = $rates[$name]
    $mean = ($r | Measure-Object -Average).Average
    "{0}: mean {1:F2} frames/s, range {2:F2}-{3:F2}" -f $name, $mean, ($r | Measure-Object -Minimum).Minimum, ($r | Measure-Object -Maximum).Maximum
}
