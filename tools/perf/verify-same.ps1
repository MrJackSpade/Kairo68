<#
.SYNOPSIS
Shows that a change keeps the guest's behavior (Kairo68#28): two host builds of game_profile run
the same games at the same clocks, and every frame's picture and sound must match.

.DESCRIPTION
Runs -Before and -After on each image at each clock with a fixed real-time clock, writing their
hashes under -Out, and prints compare-hashes.ps1's verdict for each. Runs under run-limited.ps1.

.EXAMPLE
tools\perf\verify-same.ps1 -Before D:\kairo68-perf\host-before\game_profile.exe -After .build\host\game_profile.exe -Out D:\kairo68-perf\verify\mfp
#>
param(
    [Parameter(Mandatory)][string]$Before,
    [Parameter(Mandatory)][string]$After,
    [Parameter(Mandatory)][string]$Out,
    [string[]]$Images = @(
        (Join-Path $PSScriptRoot '..\..\roms\aliensyndrome\Alien Syndrome (1992)(Dempa).dim'),
        (Join-Path $PSScriptRoot '..\..\roms\rr\R+R (1995) (NIN-NIKI).HDM')),
    [int[]]$Clocks = @(10, 16, 25),
    [int]$Warmup = 300,
    [int]$Frames = 3000,
    # More game_profile options for every run, such as --boot-frames for a game whose boot never ends.
    [string[]]$Extra = @()
)
$ErrorActionPreference = 'Stop'
$root = Resolve-Path (Join-Path $PSScriptRoot '..\..')
$limited = Join-Path $root 'shared\tools\run-limited.ps1'
$bios = Join-Path $root 'bios'
foreach ($image in $Images) {
    $name = [IO.Path]::GetFileNameWithoutExtension($image) -replace '[^A-Za-z0-9]+', '-'
    foreach ($clock in $Clocks) {
        $files = foreach ($build in @(@('before', $Before), @('after', $After))) {
            $label, $exe = $build
            $work = Join-Path $Out "$name-$clock-$label"
            New-Item -ItemType Directory -Force $work | Out-Null
            $text = & $limited $exe $bios $work $image $clock 4 $Frames - --warmup $Warmup --hashes "$work\hashes.txt" @Extra 2>&1
            if ($LASTEXITCODE -ne 0) { throw "$label failed on $name at $clock MHz: $text" }
            "$work\hashes.txt"
        }
        "$name $clock MHz: " + (& (Join-Path $PSScriptRoot 'compare-hashes.ps1') $files[0] $files[1])
    }
}
