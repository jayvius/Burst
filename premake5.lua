workspace "burst"
   configurations { "debug", "release" }

project "burst"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"
   includedirs { "external/gl3w/include", "external/glfw/include", "external/glm" }
   cppdialect "C++23" 
   libdirs { "external/glfw/build/src" }
   links { "glfw3" }
   linkoptions { "-framework OpenGL", "-framework Cocoa", "-framework IOKit", "-framework QuartzCore" }

   files { "src/*.hpp", "src/*.cpp", "external/gl3w/src/*.c" }

   filter "configurations:debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:release"
      defines { "NDEBUG" }
      optimize "On"


