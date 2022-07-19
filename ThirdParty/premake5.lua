project "ThirdParty"
	kind "StaticLib"
	language "C++"
	
	targetdir ("../libs/")
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
		"./",
	}
	
	libdirs
	{
		"./libs/"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "off"
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
		
	filter "configurations:Release"
		defines "RELEASE"
		symbols "On"
		runtime "Release"
		
	filter "configurations:Retail"
		defines "RETAIL"
		symbols "On"
		runtime "Release"