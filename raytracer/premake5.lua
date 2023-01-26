project "raytracer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ( "%{wks.location}/bin/" .. outdir .. "/%{prj.name}" )
    objdir ( "%{wks.location}/bin-int/" .. outdir .. "/%{prj.name}" )

    files {
        "source/**.h",
        "source/**.cpp"
    }

    includedirs {
        "source",
        "vendor"
    }

    debugargs { "> out.ppm" }

    filter "configurations:Debug"
        defines { "RT_DEBUG_ENABLED" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        flags "LinkTimeOptimization"
        optimize "Speed"
        symbols "off"