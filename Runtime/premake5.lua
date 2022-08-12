project "Runtime"
	kind "WindowedApp"
	language "C++"
	
	targetdir ("../bin/")
	targetname ("%{prj.name}-%{cfg.buildcfg}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"./**.h",
		"./**.hpp",
		"./**.cpp",
		"./**.hlsli",
		"./**.hlsl"
	}
	
	includedirs
	{
		"../Engine/",
		"../ThirdParty/includes/"
	}
	
	libdirs
	{
		"../libs/"
	}
	
	filter { "files:**.hlsl" }
		shadermodel "5.0"

	filter { "files:**-vs.hlsl" }
		shadertype "Vertex"

	filter { "files:**-ps.hlsl" }
		shadertype "Pixel"
		
	filter { "files:**-gs.hlsl" }
		shadertype "Geometry"

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "on"
		systemversion "10.0"
		
		defines
		{
			"PLATFORM_WINDOWS",
			"NOMINMAX",
		}
		
	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"
		runtime "Debug"

		links
		{
			"Engine",
		}
		
	filter "configurations:Release"
		defines "RELEASE"
		symbols "On"
		runtime "Release"

		links
		{
			"Engine",
		}
		
	filter "configurations:Retail"
		defines "RETAIL"
		symbols "On"
		runtime "Release"

		links
		{
			"Engine",
		}