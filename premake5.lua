workspace "SnowEngine"
	startproject "Editor"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Retail"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Editor/"
include "Engine/"
include "ThirdParty/"