PROJ_DIR = path.getabsolute("..")
project "wogui"
	kind "WindowedApp" --kind "StaticLib" "WindowedApp" "ConsoleApp"

	debugdir (PROJ_DIR)	
	
	includedirs {
		path.join(PROJ_DIR, "src"),		
		--path.join(PROJ_DIR, "3rd/glm"),
		path.join(PROJ_DIR, "3rd/GLEW2.1.0/include"),
	}

	files {
		path.join(PROJ_DIR, "src/**.h"),
		path.join(PROJ_DIR, "src/**.cpp"),	
	}
	excludes {
	}
	
	defines {
		"GLEW_STATIC",
	}
	
--[[
	links {
		"bx",
	}]]

	configuration { "Debug" }
		defines {
			"_Debug",
		}
	
	configuration { "vs* or mingw*" }
		excludes {
			
		}
		links {
			"glew32sd",
			"opengl32",
		}
		buildoptions {
			"/wd4244",
		}	
		libdirs {			
			path.join(PROJ_DIR, "3rd/GLEW2.1.0/lib"),
		}		

	configuration { "linux-*" }
		links {			
		}

	configuration { "osx" }
		links {
			"Cocoa.framework",
		}

	configuration {}

	strip()
