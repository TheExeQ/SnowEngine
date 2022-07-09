workspace "SnowEngine"
	startproject "Game"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Retail"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Game/"
include "Engine/"