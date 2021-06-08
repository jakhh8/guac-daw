project "Guacamole"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gmpch.h"
	pchsource "src/gmpch.cpp"
	
	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/**.hpp",
		"vendor/glm/**.inl",
		"vendor/ImGuizmo/*.h",
		"vendor/ImGuizmo/*.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.EnTT}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}"
	}
    
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp"
	}
	
	filter "files:vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }
	
	filter "system:linux"
		pic "On"
    
	links
	{
		"pthread", -- These are instead of opengl32.lib
		"dl"
	}
    
	filter "configurations:Debug"
		defines "GM_DEBUG"
		runtime "Debug"
		symbols "on"