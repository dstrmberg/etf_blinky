
workspace "Discolights"
    configurations {"Debug", "Release"}

project "Discolights"
    kind "ConsoleApp"
    language "C"
    targetdir "build/%{cfg.buildcfg}"
    targetname "discolights"

    -- set up cross compiler
    gccprefix ("avr-")

    files
    {
        "src/**.c",
        "src/**.h"
    }

    buildoptions
    {
        "-mmcu=attiny44a",
        "-Wall",
        "-Wextra",
        "-pedantic"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

