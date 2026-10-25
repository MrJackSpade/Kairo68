<#
.SYNOPSIS
Runs verify-same.ps1 over Kairo68's verification set (Kairo68#28): the two test games at every CPU
clock, and six more games' first disks at 25 MHz with a fixed boot (their boots wait at a prompt).

.DESCRIPTION
-Games is the folder holding the extra games' disk images, one folder per game as their archives
unpack. Prints one verdict per game and clock; every one must report 0 pictures and 0 sounds.

.EXAMPLE
tools\perf\verify-all.ps1 -Before D:\kairo68-perf\host-before\game_profile.exe -After .build\host\game_profile.exe -Games D:\kairo68-perf\games -Out D:\kairo68-perf\verify\partial
#>
param(
    [Parameter(Mandatory)][string]$Before,
    [Parameter(Mandatory)][string]$After,
    [Parameter(Mandatory)][string]$Games,
    [Parameter(Mandatory)][string]$Out
)
$ErrorActionPreference = 'Stop'
$verify = Join-Path $PSScriptRoot 'verify-same.ps1'
& $verify -Before $Before -After $After -Out (Join-Path $Out 'test-games') 2>&1 | Select-String 'MHz:' | ForEach-Object Line
$extra = @(
    'Akumajou Dracula (1993)(Konami)\Akumajou Dracula (1993)(Konami)(Disk 1 of 2).dim',
    'Afterburner II (1989)(Dempa)\Afterburner II (1989)(Dempa)(Disk 1 of 2).dim',
    'A-Jax (1989)(Konami)\A-Jax (1989)(Konami)(Disk 1 of 3)(Program).dim',
    'Arkanoid Revenge of Doh (1987)(Sharp - SPS)\Arkanoid Revenge of Doh (1987)(Sharp - SPS).dim',
    'Aquales\Aquales (1991)(Exact)[a](Disk 1 of 3)[xdf].xdf',
    'First Blow (199x)(Project G)\First Blow (199x)(Project G).dim') | ForEach-Object { Join-Path $Games $_ }
& $verify -Before $Before -After $After -Out (Join-Path $Out 'more-games') -Images $extra -Clocks 25 -Warmup 0 `
    -Extra '--boot-frames', '2400' 2>&1 | Select-String 'MHz:' | ForEach-Object Line
