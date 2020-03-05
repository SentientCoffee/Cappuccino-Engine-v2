print("Starting premake build...")

print("Building solution CappuccinoEngine...")
workspace "CappuccinoEngine"
	defines "_CRT_SECURE_NO_WARNINGS"
	architecture "x64"
	
	configurations {
		"Debug",
		"Release",
		"Distribution"
	}

	startproject "Sandbox"

include "Externals"
include "Cappuccino"
include "Sandbox"