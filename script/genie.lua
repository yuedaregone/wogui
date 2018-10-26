
solution "wogui"
	configurations {
		"Debug",
		"Release",
	}

	platforms {
		"x32",
		"x64",
		"Native", -- for targets where bitness is not specified
	}

	language "C++"

SOLUTION_DIR = path.getabsolute("..")
dofile "toolchain.lua"
toolchain(path.join(SOLUTION_DIR, "build"), path.join(SOLUTION_DIR, "build"))

dofile "imgui.lua"
dofile "wogui.lua"


