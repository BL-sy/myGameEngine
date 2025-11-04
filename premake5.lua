EngineName = "Hazel"

workspace (EngineName)
    architecture "x64" -- 平台
    startproject "Sandbox" -- 启动项目

    configurations {"Debug", "Release", "Dist"} -- 配置

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- 包含相对于根目录的其他项目
IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/imgui"

-- 将GLFW premake5.lua包含进来,相当于把内容拷贝在这里面
include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/ImGui"

project (EngineName)
    location (EngineName)
    kind "SharedLib" -- 配置类型
    language "C++"
    staticruntime "Off" -- 动态链接运行库

    targetdir ("bin/" .. outputdir .. "/%{prj.name}") -- 输出目录
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}") -- 中间文件目录

    -- 预编译头
    pchheader "hzpch.h"
    pchsource "Hazel/src/hzpch.cpp"

    -- 命令行选项
    buildoptions{
    "/utf-8",
    }

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    -- 包含目录
    includedirs {
        "%{prj.name}",
        "%{prj.name}/vendor",
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        -- 链接头文件目录
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}"
    }
    -- 链接库      
    links {
        "GLFW",
        "opengl32.lib",
        "Glad",
        "ImGui",
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        -- 预处理器
        defines {
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        -- dll复制命令
        postbuildcommands {
            -- 复制DLL到Sandbox输出目录
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        runtime "Debug" -- 运行库, Debug版本
        symbols "On"

    filter "configurations:Release"
        defines "HZ_RELEASE"
        runtime "Release" -- 运行库, Release版本
        optimize "On"


    filter "configurations:Dist"
        defines "HZ_DIST"
        runtime "Release"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "Off" -- 动态链接运行库

    targetdir ("bin/" .. outputdir .. "/%{prj.name}") -- 输出目录
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}") -- 中间文件目录

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Hazel/vendor/spdlog/include",
        "Hazel/src",
        "Hazel",
        "Hazel/vendor",
    }

    links { "Hazel" }

    -- 命令行选项
    buildoptions{
    "/utf-8"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        defines { "HZ_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        runtime "Debug" -- 运行库, Debug版本
        symbols "On"

    filter "configurations:Release"
        defines "HZ_RELEASE"
        runtime "Release" -- 运行库, Release版本
        optimize "On"


    filter "configurations:Dist"
        defines "HZ_DIST"
        runtime "Release"
        optimize "On"