<#
.SYNOPSIS
Compares two game_profile --hashes files frame by frame (Kairo68#28).

.DESCRIPTION
Prints how many frames differ in their picture or sound, and the first that does. A change that
keeps the guest's behavior differs in none. Fails when the files cover different frames.
#>
param([Parameter(Mandatory)][string]$Before, [Parameter(Mandatory)][string]$After)
$ErrorActionPreference = 'Stop'
$a = @(Get-Content $Before)
$b = @(Get-Content $After)
if ($a.Count -ne $b.Count) { throw "$Before has $($a.Count) frames, $After has $($b.Count)" }
$pictures = 0; $sounds = 0; $first = $null
for ($i = 0; $i -lt $a.Count; $i++) {
    $x = $a[$i].Split(' '); $y = $b[$i].Split(' ')
    $picture = $x[1] -ne $y[1] -or $x[2] -ne $y[2]
    $sound = $x[3] -ne $y[3]
    if ($picture) { $pictures++ }
    if ($sound) { $sounds++ }
    if (($picture -or $sound) -and $null -eq $first) { $first = $x[0] }
}
"$($a.Count) frames: $pictures pictures and $sounds sounds differ" + $(if ($null -ne $first) { "; first at frame $first" } else { '' })
