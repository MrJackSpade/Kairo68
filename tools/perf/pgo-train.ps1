<#
.SYNOPSIS
Trains Kairo68's profile-guided build on the device (Kairo68#6): runs an instrumented game_profile
over the training games and merges what they record into one indexed profile.

.DESCRIPTION
-Tool is game_profile built to record a profile, from the committed tree (the profile records it):
  tools\perf\build-variant.ps1 -Name pgogen -Out <dir> -CMakeArgs '-DKAIRO_PGO=OFF','-DKAIRO68_PGO_GENERATE=ON'
Each training run boots a game and plays its attract demo at one clock, writing its own .profraw;
the runs are pulled to <Out>\raw and merged with equal weight into <Out>\kairo68.profdata by the
NDK's llvm-profdata, which shared/tools/pgo_freeze.py then writes into core/pgo as the profile the
device build uses.

The games below are the training set. A-Jax, Aquales and R+R are kept out of it, so the evaluation
can include games the profile has never seen.

.EXAMPLE
tools\perf\pgo-train.ps1 -Device 192.168.1.118:5555 -Tool F:\kairo68-perf\game_profile-pgogen.stripped -Games F:\kairo68-perf\games -Out F:\kairo68-perf\pgo
#>
param(
    [Parameter(Mandatory)][string]$Device,
    [Parameter(Mandatory)][string]$Tool,
    [Parameter(Mandatory)][string]$Games,
    [Parameter(Mandatory)][string]$Out,
    [string]$Bios = (Join-Path $PSScriptRoot '..\..\bios\x68000'),
    [string]$Ndk = 'F:\android-sdk\ndk\28.2.13676358',
    [int]$Frames = 1800
)
$ErrorActionPreference = 'Stop'
$remote = '/data/local/tmp/k68'
# The executable by its path: a bare "adb" inside the function named Adb would call the function.
$adb = (Get-Command adb -CommandType Application | Select-Object -First 1).Source
function Adb { $out = & $adb -s $Device @args 2>&1; if ($LASTEXITCODE -ne 0) { throw "adb $args failed: $out" }; $out }

# Name, image, clock and game_profile options. Games whose boot never ends on its own get a fixed one.
$alien = Join-Path $PSScriptRoot '..\..\roms\aliensyndrome\Alien Syndrome (1992)(Dempa).dim'
$runs = @(
    [pscustomobject]@{ Name = 'alien-25'; Image = $alien; Clock = 25; Options = @('--warmup', '0') }
    [pscustomobject]@{ Name = 'alien-10'; Image = $alien; Clock = 10; Options = @('--warmup', '0') }
    [pscustomobject]@{ Name = 'dracula'; Image = Join-Path $Games 'Akumajou Dracula (1993)(Konami)\Akumajou Dracula (1993)(Konami)(Disk 1 of 2).dim'; Clock = 25; Options = @('--boot-frames', '2400') }
    [pscustomobject]@{ Name = 'afterburner'; Image = Join-Path $Games 'Afterburner II (1989)(Dempa)\Afterburner II (1989)(Dempa)(Disk 1 of 2).dim'; Clock = 25; Options = @('--boot-frames', '2400') }
    [pscustomobject]@{ Name = 'arkanoid'; Image = Join-Path $Games 'Arkanoid Revenge of Doh (1987)(Sharp - SPS)\Arkanoid Revenge of Doh (1987)(Sharp - SPS).dim'; Clock = 25; Options = @('--boot-frames', '2400') }
    [pscustomobject]@{ Name = 'firstblow'; Image = Join-Path $Games 'First Blow (199x)(Project G)\First Blow (199x)(Project G).dim'; Clock = 25; Options = @('--warmup', '0') }
)

$raw = Join-Path $Out 'raw'
New-Item -ItemType Directory -Force $raw | Out-Null
Adb shell "mkdir -p $remote/bios/x68000 $remote/work $remote/pgo; rm -f $remote/pgo/*.profraw" | Out-Null
Adb push (Join-Path $Bios 'iplrom.dat') (Join-Path $Bios 'cgrom.dat') "$remote/bios/x68000/" | Out-Null
Adb push $Tool "$remote/game_profile-pgogen" | Out-Null
Adb shell "chmod 755 $remote/game_profile-pgogen" | Out-Null
foreach ($run in $runs) {
    Adb push $run.Image "$remote/train.img" | Out-Null
    $options = $run.Options -join ' '
    $text = Adb shell "cd $remote && rm -rf work/run && mkdir -p work/run && LLVM_PROFILE_FILE=$remote/pgo/$($run.Name).profraw ./game_profile-pgogen bios work/run train.img $($run.Clock) 4 $Frames - $options"
    "$($run.Name): " + ($text | Select-String '^host ').Line
    Adb pull "$remote/pgo/$($run.Name).profraw" (Join-Path $raw "$($run.Name).profraw") | Out-Null
}
$profdata = Join-Path $Ndk 'toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-profdata.exe'
$merged = Join-Path $Out 'kairo68.profdata'
& $profdata merge -o $merged @(Get-ChildItem $raw -Filter '*.profraw' | ForEach-Object FullName)
if ($LASTEXITCODE -ne 0) { throw 'llvm-profdata merge failed' }
"merged $merged"
$root = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path
$note = 'tools/perf/pgo-train.ps1: ' + (($runs | ForEach-Object { "$($_.Name) at $($_.Clock) MHz" }) -join ', ') +
    ", $Frames frames after each boot; A-Jax, Aquales and R+R held out"
& python (Join-Path $root 'shared\tools\pgo_freeze.py') --profdata $merged --llvm-profdata $profdata --root $root `
    --text (Join-Path $root 'core\pgo\arm64.proftext') --manifest (Join-Path $root 'core\pgo\profile.json') `
    --ndk (Split-Path $Ndk -Leaf) --sparse --note $note @(Get-ChildItem $raw -Filter '*.profraw' | ForEach-Object FullName)
if ($LASTEXITCODE -ne 0) { throw 'pgo_freeze.py failed' }
