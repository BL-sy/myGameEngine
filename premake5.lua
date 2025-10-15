EngineName = "Hazel"

workspace (EngineName)
    architecture "x64" -- 平台
    startproject "Sandbox" -- 启动项目

    configurations {"Debug", "Release", "Dist"} -- 配置

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project (EngineName)
    location (EngineName)
    kind "SharedLib" -- 配置类型
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}") -- 输出目录
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}") -- 中间文件目录

    -- 预编译头
    pchheader "hzpch.h"
    pchsource "Hazel/src/hzpch.cpp"

    -- 命令行选项
    buildoptions{
	"/utf-8"
    }

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    -- 包含目录
    includedirs {
	"%{prj.name}/vendor",
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        -- 预处理器
        defines {
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL"
        }

        -- dll复制命令
        postbuildcommands {
            -- 复制DLL到Sandbox输出目录
            "{COPY} \"%{cfg.buildtarget.relpath}\" \"../bin/" .. outputdir .. "/Sandbox/\""
        }

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "HZ_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "HZ_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}") -- 输出目录
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}") -- 中间文件目录

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Hazel/vendor/spdlog/include",
        "Hazel/src",
    }

    links { "Hazel" }

    -- 命令行选项
    buildoptions{
	"/utf-8"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        defines { "HZ_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "HZ_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "HZ_DIST"
        optimize "On"