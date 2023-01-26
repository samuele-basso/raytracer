workspace "raytracer"
    architecture "amd64"
    startproject "raytracer"

    configurations {
        "Debug",
        "Release"
    }

    flags {
        "MultiProcessorCompile"
    }

    outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    include "raytracer"