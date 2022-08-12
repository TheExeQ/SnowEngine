project "ThirdParty"
	kind "StaticLib"
	language "C++"
	
	targetdir ("../libs/")
	targetname ("%{prj.name}-%{cfg.buildcfg}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")
	
	postbuildcommands 
	{
		"{COPY} ./libs/assimp/assimp-vc143-mt.dll ../bin/"
	}
	
	files
	{
		"./**.h",
		"./**.hpp",
		"./**.cpp",
		"./**.inl",
	}
	
	includedirs
	{
		"./includes/",
	}
	
	libdirs
	{
		"./libs/assimp/"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "on"
		systemversion "10.0"
		
		defines
		{
			"PLATFORM_WINDOWS",
			"NOMINMAX",
			"SPDLOG_COMPILED_LIB",
		}
		
	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"
		runtime "Debug"
		
		links
		{
			"assimp-vc143-mt.lib",
		}
		
	filter "configurations:Release"
		defines "RELEASE"
		symbols "On"
		runtime "Release"
		
		links
		{
			"assimp-vc143-mt.lib",
		}
		
	filter "configurations:Retail"
		defines "RETAIL"
		symbols "On"
		runtime "Release"
		
		links
		{
			"assimp-vc143-mt.lib",
		}