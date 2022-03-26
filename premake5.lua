BUILD_DIRECTORY = "%{wks.location}"

BUILD_SANDBOX = true
BUILD_TESTS = true

workspace "Stardust"
    location "build"
    architecture "x86_64"

    if BUILD_SANDBOX then
        startproject "Sandbox"
    end

    configurations {
        "Debug",
        "Release",
    }

include "dep"

include "Stardust"

if BUILD_SANDBOX then
    include "Sandbox"
end

if BUILD_TESTS then
    include "tests"
end
