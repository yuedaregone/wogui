PROJ_DIR = path.getabsolute("..")
project "imgui"
	kind "StaticLib" --kind "StaticLib" "WindowedApp" "ConsoleApp"
	
	includedirs {
		path.join(PROJ_DIR, "3rd/imgui"),
	}

	files {
		path.join(PROJ_DIR, "3rd/imgui/**.h"),
		path.join(PROJ_DIR, "3rd/imgui/**.cpp"),	
	}
	excludes {
		path.join(PROJ_DIR, "3rd/imgui/examples/**.h"),
		path.join(PROJ_DIR, "3rd/imgui/examples/**.cpp"),
		
		path.join(PROJ_DIR, "3rd/imgui/misc/freetype/**.h"),
		path.join(PROJ_DIR, "3rd/imgui/misc/freetype/**.cpp"),
	}
	
	defines {
		
	}


	configuration { "Debug" }
		defines {
			"_Debug",
		}
		
	
	configuration { "vs* or mingw*" }
		excludes {
			path.join(PROJ_DIR, "3rd/imgui/imgui_impl_opengl3.h"),
			path.join(PROJ_DIR, "3rd/imgui/imgui_impl_opengl3.cpp"),
		}
		links {			
			--"opengl32",
		}
		targetdir (path.join(PROJ_DIR, "build", "lib", "win32_" .. _ACTION))
		--buildoptions {
		--	"/wd4244",
		--}	
		--libdirs {			
		--	path.join(PROJ_DIR, "3rd/GLEW2.1.0/lib"),
		--}		

	configuration { "linux-*" }
		links {			
		}

	configuration { "osx" }
		links {
			"Cocoa.framework",
		}

	configuration {}	
