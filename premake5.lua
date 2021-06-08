include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Sound"
	architecture "x64"
	startproject "SoundApp"

	configurations
	{
		"Debug"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Guacamole/vendor/GLFW/include/"
IncludeDir["Glad"] = "%{wks.location}/Guacamole/vendor/Glad/include/"
IncludeDir["ImGui"] = "%{wks.location}/Guacamole/vendor/imgui/"
IncludeDir["glm"] = "%{wks.location}/Guacamole/vendor/glm/"
IncludeDir["stb_image"] = "%{wks.location}/Guacamole/vendor/stb_image/"
IncludeDir["EnTT"] = "%{wks.location}/Guacamole/vendor/EnTT/"
IncludeDir["yaml_cpp"] = "%{wks.location}/Guacamole/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Guacamole/vendor/ImGuizmo"
IncludeDir["OpenAL"] = "%{wks.location}/SoundApp/vendor/openal-soft/include/AL"

group "Dependencies"
	include "vendor/premake"
	include "Guacamole/vendor/GLFW/"
	include "Guacamole/vendor/Glad/"
	include "Guacamole/vendor/imgui/"
	include "Guacamole/vendor/yaml-cpp/"
group ""

include "Guacamole"
include "SoundApp"