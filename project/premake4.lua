solution "ProceduralTreeGeneration"
  configurations { "Debug", "Release" }

project "MyFirstTree"
  kind "WindowedApp"
  language "C++"
  location "build"
  files {
   "**.h",
   "**.c",
   "**.cpp"
  }
  links { "glfw"}
  --libdirs {os.findlib("glfw")} -- funktionier so wie die obere, nur dass platformunabhängig ist.
  libdirs { "../usedLibraries/glfw-3.0.4/build/src/" }
  includedirs { "../usedLibraries/glm/" } -- glm ist eine header-only library
  buildoptions "-std=c++11"
  --os.execute() //z.B. um Doxygen aufzurufen.

  configuration "Debug"
    targetdir "bin/debug"
    defines { "_DEBUG", "DEBUG" }
    flags { "Symbols", "ExtraWarnings", "FatalWarnings" }    
 
  configuration "Release"
    targetdir "bin/release"
    defines { "NDEBUG" }
    flags   { "Optimize" }
  
  configuration {"gmake", "linux"}
    links { "GLEW", "GLU", "GL"}
    libdirs {os.findlib("glfw")}
    libdirs {os.findlib("GLU")}
    libdirs {os.findlib("GL")}
  
  configuration "windows"
    defines { "GLFW_DLL" }
    links { "glew32", "glu32", "opengl32"}
    libdirs {os.findlib("glew32")}
    libdirs {os.findlib("glu32")}
    libdirs {os.findlib("opengl32")}

-- http://glew.sourceforge.net/install.html    
--   Remember to link your project with glew32.lib, glu32.lib, and opengl32.lib on Windows and libGLEW.so, libGLU.so, and libGL.so on Unix (-lGLEW -lGLU -lGL). 

-- http://www.glfw.org/docs/latest/build.html#build_link_win32
--   GLFW_DLL is necessary when using the GLFW DLL on Windows, in order to explain to the compiler that the GLFW functions will be coming from another executable. It has no function on other platforms. 
