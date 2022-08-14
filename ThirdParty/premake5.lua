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
		"./libs/assimp/",
		"./libs/PhysX/",
		"./libs/ozz/",
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
			"ozz_animation_fbx_r.lib",
			"ozz_animation_offline_r.lib",
			"ozz_animation_r.lib",
			"ozz_animation_tools_r.lib",
			"ozz_base_r.lib",
			"ozz_geometry_r.lib",
			"ozz_options_r.lib",
		}
		
	filter "configurations:Release"
		defines "RELEASE"
		symbols "On"
		runtime "Release"
		
		links
		{
			"assimp-vc143-mt.lib",
			"ozz_animation_fbx_r.lib",
			"ozz_animation_offline_r.lib",
			"ozz_animation_r.lib",
			"ozz_animation_tools_r.lib",
			"ozz_base_r.lib",
			"ozz_geometry_r.lib",
			"ozz_options_r.lib",
		}
		
	filter "configurations:Retail"
		defines "RETAIL"
		symbols "On"
		runtime "Release"
		
		links
		{
			"assimp-vc143-mt.lib",
			"ozz_animation_fbx_r.lib",
			"ozz_animation_offline_r.lib",
			"ozz_animation_r.lib",
			"ozz_animation_tools_r.lib",
			"ozz_base_r.lib",
			"ozz_geometry_r.lib",
			"ozz_options_r.lib",
		}