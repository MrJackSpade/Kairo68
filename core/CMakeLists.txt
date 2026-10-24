# The X68000 machine on MAME's x68k driver (third_party/mame), driven one frame at a time
# (x68k_machine.h), for the Android app and for host tests. Source lists come from MAME's own
# project files (generated/sources.cmake, tools/generate_mame.py); defines and include directories
# follow MAME's scripts/genie.lua and scripts/src/*.lua.
cmake_minimum_required(VERSION 3.22.1)
project(kairo68_core LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)

get_filename_component(MAME_ROOT "${CMAKE_CURRENT_LIST_DIR}/../third_party/mame" ABSOLUTE)
set(GENERATED "${CMAKE_CURRENT_LIST_DIR}/generated")
include("${GENERATED}/sources.cmake")

# The emulator runs in real time only optimized, so the core is optimized in debug builds too.
# The level is a setting so measurements can compare others (Kairo68#9).
set(KAIRO68_OPTIMIZE "-O3" CACHE STRING "The core's optimization level")
add_compile_options(${KAIRO68_OPTIMIZE} -Wno-deprecated-declarations -Wno-unused-variable -Wno-unknown-warning-option)
add_compile_definitions(CRLF=2 LSB_FIRST NDEBUG FLAC__NO_DLL NATIVE_DRC=drcbe_c PUGIXML_HEADER_ONLY)
# The sound manager mixes and hands sound to the OSD on the emulation thread, as in MAME's
# emscripten build, so each frame's sound arrives with the frame.
add_compile_definitions(SOUND_DISABLE_THREADING)
# A measurement build (Kairo68#28, kairo68_perf.h) counts what the guest did in each frame and
# where the emulation thread's time went. The counters cost time themselves: never in a release.
option(KAIRO68_PERF "Count guest work and stage times per frame" OFF)
if(KAIRO68_PERF)
    add_compile_definitions(KAIRO68_PERF)
endif()
# The device build of the core uses a profile of the device playing games (Kairo68#6,
# core/pgo/README.md). The profile describes this exact code as the device compiles it, so a
# measurement build (KAIRO68_PERF), a training build (KAIRO68_PGO_GENERATE) or another compiler
# turns it off with -DKAIRO_PGO=OFF.
if(ANDROID)
    set(KAIRO68_PGO_DEFAULT ON)
else()
    set(KAIRO68_PGO_DEFAULT OFF)
endif()
option(KAIRO_PGO "Build the core with its recorded profile (core/pgo)" ${KAIRO68_PGO_DEFAULT})
option(KAIRO68_PGO_GENERATE "Build the core to record a profile (tools/perf/pgo-train.ps1)" OFF)
if(KAIRO_PGO)
    if(KAIRO68_PERF OR KAIRO68_PGO_GENERATE)
        message(FATAL_ERROR "The recorded profile is for the ordinary core: add -DKAIRO_PGO=OFF to a KAIRO68_PERF or KAIRO68_PGO_GENERATE build")
    endif()
    include("${CMAKE_CURRENT_LIST_DIR}/../shared/cmake/KairoPgo.cmake")
    kairo_use_profile("${CMAKE_CURRENT_LIST_DIR}/pgo/arm64.proftext" "${CMAKE_CURRENT_LIST_DIR}/pgo/profile.json"
        "${CMAKE_CURRENT_LIST_DIR}/..")
endif()
if(KAIRO68_PGO_GENERATE)
    add_compile_options(-fprofile-generate)
    add_link_options(-fprofile-generate)
endif()
# The measurement tool (test/game_profile.cpp) for the device too, as a standalone executable.
option(KAIRO68_TOOLS "Build the measurement tool on Android" OFF)
if(WIN32)
    # MAME's Windows OSD configuration (scripts/src/osd/windows_cfg.lua), for the host test.
    add_compile_definitions(UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX _WIN32_WINNT=0x0A00)
endif()
# Some MAME files use std::vector, std::abs and friends without including their headers, which
# libc++ does not include for them.
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:SHELL:-include vector>" "$<$<COMPILE_LANGUAGE:CXX>:SHELL:-include cmath>"
    "$<$<COMPILE_LANGUAGE:CXX>:SHELL:-include cstdlib>")

add_library(mame_zlib STATIC ${MAME_ZLIB_SOURCES})
target_compile_definitions(mame_zlib PRIVATE ZLIB_CONST)
target_include_directories(mame_zlib PUBLIC "${MAME_ROOT}/3rdparty/zlib")

add_library(mame_expat STATIC ${MAME_EXPAT_SOURCES})
target_compile_definitions(mame_expat PRIVATE HAVE_MEMMOVE HAVE_STDINT_H HAVE_STDIO_H HAVE_STDLIB_H
    HAVE_STRING_H STDC_HEADERS XML_CONTEXT_BYTES=1024 XML_DTD XML_GE=1 XML_NS BYTEORDER=1234
    XML_POOR_ENTROPY)
target_include_directories(mame_expat PUBLIC "${MAME_ROOT}/3rdparty/expat/lib")

add_library(mame_zstd STATIC ${MAME_ZSTD_SOURCES})
target_compile_definitions(mame_zstd PRIVATE XXH_NAMESPACE=ZSTD_ DEBUGLEVEL=0 ZSTD_DISABLE_ASM)
target_include_directories(mame_zstd PUBLIC "${MAME_ROOT}/3rdparty/zstd/lib")

add_library(mame_flac STATIC ${MAME_FLAC_SOURCES})
target_compile_definitions(mame_flac PRIVATE HAVE_CONFIG_H ENABLE_64_BIT_WORDS=1 OGG_FOUND=0 FLAC__HAS_OGG=0
    HAVE_LROUND=1 HAVE_INTTYPES_H HAVE_STDBOOL_H HAVE_STDINT_H HAVE_STDIO_H HAVE_STDLIB_H HAVE_STRING_H
    HAVE_BSWAP16 HAVE_BSWAP32 _FILE_OFFSET_BITS=64 _LARGEFILE_SOURCE CPU_IS_BIG_ENDIAN=0 CPU_IS_LITTLE_ENDIAN=1)
target_include_directories(mame_flac PRIVATE "${MAME_ROOT}/3rdparty/flac/src/libFLAC/include")
target_include_directories(mame_flac PUBLIC "${MAME_ROOT}/3rdparty/flac/include")

add_library(mame_utf8proc STATIC ${MAME_UTF8PROC_SOURCES})
target_compile_definitions(mame_utf8proc PUBLIC UTF8PROC_STATIC)
target_include_directories(mame_utf8proc PUBLIC "${MAME_ROOT}/3rdparty/utf8proc")

# SoftFloat 3 is built as C++ ("ForceCPP" in MAME's project).
add_library(mame_softfloat3 STATIC ${MAME_SOFTFLOAT3_SOURCES})
set_source_files_properties(${MAME_SOFTFLOAT3_SOURCES} PROPERTIES LANGUAGE CXX)
target_compile_definitions(mame_softfloat3 PRIVATE SOFTFLOAT_ROUND_ODD INLINE_LEVEL=5 SOFTFLOAT_FAST_DIV32TO16
    SOFTFLOAT_FAST_DIV64TO32 SOFTFLOAT_FAST_INT64 LITTLEENDIAN=1)
target_include_directories(mame_softfloat3 PRIVATE "${MAME_ROOT}/src/osd" "${MAME_ROOT}/3rdparty/softfloat3/build/MAME"
    "${MAME_ROOT}/3rdparty/softfloat3/source" "${MAME_ROOT}/3rdparty/softfloat3/source/include"
    "${MAME_ROOT}/3rdparty/softfloat3/source/8086" "${MAME_ROOT}/3rdparty/softfloat3/bochs_ext")

add_library(mame_7z STATIC ${MAME_7Z_SOURCES})
target_include_directories(mame_7z PUBLIC "${MAME_ROOT}/3rdparty/lzma/C")

add_library(mame_jpeg STATIC ${MAME_JPEG_SOURCES})
target_include_directories(mame_jpeg PUBLIC "${MAME_ROOT}/3rdparty/libjpeg")

add_library(mame_ymfm STATIC ${MAME_YMFM_SOURCES})
target_include_directories(mame_ymfm PUBLIC "${MAME_ROOT}/3rdparty/ymfm/src")

# The OSD's platform services: MAME's files, directories, sockets and timing for the platform.
set(OSD "${MAME_ROOT}/src/osd")
if(WIN32)
    set(OSD_PLATFORM "${OSD}/modules/file/windir.cpp" "${OSD}/modules/file/winfile.cpp"
        "${OSD}/modules/file/winptty.cpp" "${OSD}/modules/file/winsocket.cpp" "${OSD}/modules/lib/osdlib_win32.cpp")
else()
    set(OSD_PLATFORM "${OSD}/modules/file/posixdir.cpp" "${OSD}/modules/file/posixfile.cpp"
        "${OSD}/modules/file/posixptty.cpp" "${OSD}/modules/file/posixsocket.cpp" "${OSD}/modules/lib/osdlib_unix.cpp")
endif()
add_library(mame_osd STATIC "${OSD}/osdcore.cpp" "${OSD}/osdsync.cpp" "${OSD}/strconv.cpp"
    "${OSD}/interface/audio.cpp" "${OSD}/interface/inputseq.cpp" "${OSD}/interface/nethandler.cpp" ${OSD_PLATFORM})
target_include_directories(mame_osd PUBLIC "${OSD}" "${MAME_ROOT}/src/lib" "${MAME_ROOT}/src/lib/util" "${MAME_ROOT}/3rdparty")
if(WIN32)
    target_sources(mame_osd PRIVATE "${OSD}/windows/winutil.cpp" "${OSD}/windows/winutf8.cpp")
    target_include_directories(mame_osd PUBLIC "${OSD}/windows")
endif()
target_compile_definitions(mame_osd PRIVATE $<$<NOT:$<BOOL:${WIN32}>>:ASIO_HAS_PTHREADS>)

add_library(mame_utils STATIC ${MAME_UTILS_SOURCES})
target_include_directories(mame_utils PUBLIC "${MAME_ROOT}/src/lib/util" "${MAME_ROOT}/src/lib")
target_include_directories(mame_utils PUBLIC "${MAME_ROOT}/3rdparty/pugixml/src")
target_link_libraries(mame_utils PUBLIC mame_osd mame_expat mame_zlib mame_zstd mame_flac mame_utf8proc mame_jpeg mame_7z)

# The emulator core, the X68000's devices and driver, and Kairo68's host side.
add_library(kairo68_core STATIC ${MAME_EMU_SOURCES} ${MAME_DEVICE_SOURCES}
    "${GENERATED}/drivlist.cpp" "${GENERATED}/version.cpp" x68k_machine.cpp)
target_include_directories(kairo68_core PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
# kairo/ordered_worker.h, the shared worker the YM2151's synthesis runs on (Kairo68#22).
target_include_directories(kairo68_core PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../shared/frontend/src/main/cpp")
target_include_directories(kairo68_core PRIVATE "${MAME_ROOT}/src/emu" "${MAME_ROOT}/src/devices" "${MAME_ROOT}/src/mame"
    "${MAME_ROOT}/src/mame/shared" "${MAME_ROOT}/src/lib/netlist" "${MAME_ROOT}/3rdparty/softfloat3/source/include"
    "${MAME_ROOT}/3rdparty/rapidjson/include" "${MAME_ROOT}/3rdparty/asio/include" "${GENERATED}" "${GENERATED}/ui" "${GENERATED}/emu" "${MAME_ROOT}/src/lib/formats")
target_link_libraries(kairo68_core PUBLIC mame_utils mame_softfloat3 mame_ymfm)
if(ANDROID)
    target_link_libraries(kairo68_core PUBLIC log)
elseif(WIN32)
    target_link_libraries(kairo68_core PUBLIC ws2_32 mswsock shlwapi)
endif()

# The measurement tool can hold the core in a shared library, as the app does, so calls inside the
# core bind as in libkairo68.so (Kairo68#8); build it with ANDROID_STL=c++_shared.
option(KAIRO68_TOOL_SHARED "Link the measurement tool's core as a shared library" OFF)

if(NOT ANDROID OR KAIRO68_TOOLS)
    if(KAIRO68_TOOL_SHARED)
        # The tool and the core it uses in one library, as the app's library holds its code and the
        # core; the executable only starts it.
        add_library(kairo68_tool_core SHARED test/game_profile.cpp)
        target_compile_definitions(kairo68_tool_core PRIVATE GAME_PROFILE_LIBRARY)
        target_link_libraries(kairo68_tool_core PRIVATE kairo68_core)
        add_executable(game_profile test/tool_core.cpp)
        target_link_libraries(game_profile PRIVATE kairo68_tool_core)
    else()
        add_executable(game_profile test/game_profile.cpp)
        target_link_libraries(game_profile PRIVATE kairo68_core)
    endif()
    if(WIN32 OR (ANDROID AND NOT KAIRO68_TOOL_SHARED))
        target_link_options(game_profile PRIVATE -static-libstdc++)
    endif()
    if(WIN32)
        target_link_options(game_profile PRIVATE -static)
    endif()
endif()

if(NOT ANDROID)
    add_executable(boot_test test/boot_test.cpp)
    target_link_libraries(boot_test PRIVATE kairo68_core)
    add_executable(boot_profile test/boot_profile.cpp)
    target_link_libraries(boot_profile PRIVATE kairo68_core)
    if(WIN32)
        target_link_options(boot_test PRIVATE -static)
        target_link_options(boot_profile PRIVATE -static)
    endif()
endif()
