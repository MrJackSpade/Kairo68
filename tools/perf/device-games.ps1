<#
.SYNOPSIS
Runs one measurement build of core/test/game_profile over many games on an Android device, to find
the games below full speed (Kairo68#29).

.DESCRIPTION
-Games is a folder of games, one folder per game as their archives unpack. Each game boots its
first disk (with its second in drive 1 when it has one) for a fixed -BootFrames, then runs -Frames
flat out with no input, usually its title or attract demo. Prints one line per game: frames a
second against the guest's, and the frame time's mean and 95th percentile. Writes each run's output
to <Out>\<game>.txt.

.EXAMPLE
tools\perf\device-games.ps1 -Device 192.168.1.118:5555 -Build F:\kairo68-perf\game_profile-plain.stripped -Games roms\more -Out F:\kairo68-perf\survey
#>
param(
    [Parameter(Mandatory)][string]$Device,
    [Parameter(Mandatory)][string]$Build,
    [Parameter(Mandatory)][string]$Games,
    [Parameter(Mandatory)][string]$Out,
    [string]$Bios = (Join-Path $PSScriptRoot '..\..\bios\x68000'),
    [int]$Clock = 25,
    [int]$BootFrames = 1200,
    [int]$Frames = 1800,
    # Only the games whose folder name matches this pattern.
    [string]$Filter = '.'
)
$ErrorActionPreference = 'Stop'
$remote = '/data/local/tmp/k68'
$adb = (Get-Command adb -CommandType Application | Select-Object -First 1).Source
function Adb { $out = & $adb -s $Device @args 2>&1; if ($LASTEXITCODE -ne 0) { throw "adb $args failed: $out" }; $out }

New-Item -ItemType Directory -Force $Out | Out-Null
Adb shell "mkdir -p $remote/bios/x68000 $remote/work" | Out-Null
Adb push (Join-Path $Bios 'iplrom.dat') (Join-Path $Bios 'cgrom.dat') "$remote/bios/x68000/" | Out-Null
Adb push $Build "$remote/survey-build" | Out-Null
Adb shell "chmod 755 $remote/survey-build" | Out-Null
foreach ($game in Get-ChildItem -Directory $Games | Where-Object Name -Match $Filter) {
    $disks = @(Get-ChildItem -File $game.FullName | Where-Object { $_.Extension -in '.dim', '.xdf', '.hdm', '.d88', '.2hd' } |
        Where-Object Name -NotMatch '\[xdf\]|demo' | Sort-Object Name)
    if (-not $disks) { "$($game.Name): no disk image"; continue }
    $first = $disks | Where-Object Name -NotMatch 'Disk [2-9]' | Select-Object -First 1
    if (-not $first) { $first = $disks[0] }
    $second = $disks | Where-Object { $_.Name -match 'Disk 2 of|Disk B\)' } | Select-Object -First 1
    # Plain names on the device: game names hold quotes and brackets.
    Adb push $first.FullName "$remote/survey-1$($first.Extension)" | Out-Null
    $extra = ''
    if ($second) {
        Adb push $second.FullName "$remote/survey-2$($second.Extension)" | Out-Null
        $extra = "--disk2 survey-2$($second.Extension)"
    }
    $text = Adb shell "cd $remote && rm -rf work/survey && mkdir -p work/survey && ./survey-build bios work/survey survey-1$($first.Extension) $Clock 4 $Frames - --boot-frames $BootFrames $extra"
    $text | Set-Content -Encoding utf8 (Join-Path $Out (($game.Name -replace '[^A-Za-z0-9]+', '-') + '.txt'))
    $rate = ($text | Select-String '^host ').Line -replace '^host .* s: ', ''
    $times = ($text | Select-String '^frame ms').Line -replace '^frame ms: ', ''
    "$($game.Name): $rate; $times"
    Adb shell "rm -f $remote/survey-1* $remote/survey-2*" | Out-Null
}
