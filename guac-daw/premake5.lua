project "guac-daw"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
    
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    
	files
	{
        "src/**.h",
		"src/**.cpp"
	}
    
	includedirs
	{
		"guac-daw/src",
        
		"%{wks.location}/Guacamole/vendor/spdlog/include",
		"%{wks.location}/Guacamole/src",
		"%{wks.location}/Guacamole/vendor",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
        "%{IncludeDir.EnTT}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.OpenAL}",
        "%{wks.location}/guac-daw/vendor/openal-soft/common"
	}
	
	links
	{
		"Guacamole",
		"GLFW",
        "Glad",
        "ImGui",
        "yaml-cpp",
		"vendor/openal-soft/build/openal",
	}
    
	filter "system:linux"
    links
    {
        "pthread", -- This is instead of opengl32.lib
        "dl"
    }
	
	filter "configurations:Debug"
    defines "GM_DEBUG"
    runtime "Debug"
    symbols "on"