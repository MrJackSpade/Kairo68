<#
.SYNOPSIS
Builds core/test/game_profile for the device with other compiler settings, to compare them with the
plain measurement build (Kairo68#6-#9). A variant with the core in a shared library
(-DKAIRO68_TOOL_SHARED=ON, -Stl c++_shared) also writes its libraries to <Out>\libs-<Name>.

.DESCRIPTION
Configures and builds <Out>\android-<Name> with the NDK (KAIRO68_TOOLS=ON, release) and the given
extra CMake arguments, and writes the stripped executable to <Out>\game_profile-<Name>.stripped.

.EXAMPLE
tools\perf\build-variant.ps1 -Name o2 -Out D:\kairo68-perf -CMakeArgs '-DKAIRO68_OPTIMIZE=-O2'
#>
param(
    [Parameter(Mandatory)][string]$Name,
    [Parameter(Mandatory)][string]$Out,
    [string[]]$CMakeArgs = @(),
    [ValidateSet('c++_static', 'c++_shared')][string]$Stl = 'c++_static',
    [string]$Sdk = 'F:\android-sdk',
    [string]$Ndk = 'F:\android-sdk\ndk\28.2.13676358'
)
$ErrorActionPreference = 'Stop'
$cmake = Join-Path $Sdk 'cmake\3.22.1\bin\cmake.exe'
$ninja = (Join-Path $Sdk 'cmake\3.22.1\bin\ninja.exe') -replace '\\', '/'
$core = Join-Path $PSScriptRoot '..\..\core'
$dir = Join-Path $Out "android-$Name"
& $cmake -S $core -B $dir -G Ninja "-DCMAKE_MAKE_PROGRAM=$ninja" "-DCMAKE_TOOLCHAIN_FILE=$Ndk\build\cmake\android.toolchain.cmake" `
    -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-29 "-DANDROID_STL=$Stl" -DCMAKE_BUILD_TYPE=Release `
    -DKAIRO68_TOOLS=ON @CMakeArgs *> "$dir-configure.log"
if ($LASTEXITCODE -ne 0) { throw "configuring $Name failed; see $dir-configure.log" }
& $cmake --build $dir --target game_profile *> "$dir-build.log"
if ($LASTEXITCODE -ne 0) { throw "building $Name failed; see $dir-build.log" }
$strip = Join-Path $Ndk 'toolchains\llvm\prebuilt\windows-x86_64\bin\llvm-strip.exe'
& $strip -o (Join-Path $Out "game_profile-$Name.stripped") (Join-Path $dir 'game_profile')
if ($LASTEXITCODE -ne 0) { throw "stripping $Name failed" }
$libs = @(Get-ChildItem $dir -Filter '*.so')
if ($Stl -eq 'c++_shared') {
    $libs += Get-Item (Join-Path $Ndk 'toolchains\llvm\prebuilt\windows-x86_64\sysroot\usr\lib\aarch64-linux-android\libc++_shared.so')
}
if ($libs) {
    $libDir = Join-Path $Out "libs-$Name"
    New-Item -ItemType Directory -Force $libDir | Out-Null
    foreach ($lib in $libs) {
        & $strip -o (Join-Path $libDir $lib.Name) $lib.FullName
        if ($LASTEXITCODE -ne 0) { throw "stripping $($lib.Name) failed" }
    }
}
"built $Name"
