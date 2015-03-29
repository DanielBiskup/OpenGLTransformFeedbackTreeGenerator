--[[
Copyright (c) 2015 Daniel Biskup
All rights reserved.

Redistribution and use in source and binary forms, with
or without modification, are permitted provided that the
following conditions are met:

1. Redistributions of source code must retain the above
copyright notice, this list of conditions and the
following disclaimer.

2. Redistributions in binary form must reproduce the
above copyright notice, this list of conditions and the
following disclaimer in the documentation and/or other
materials provided with the distribution.

3. Neither the name of the copyright holder nor the names
of its contributors may be used to endorse or promote
products derived from this software without specific
prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
--]]

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
  libdirs { "../usedLibraries/glfw-3.0.4/build/src/"}
  includedirs { "../usedLibraries/glm/"} -- glm ist eine header-only library
  --buildoptions "-std=c++11"
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
    links { "GLEW", "GLU", "GL", "AntTweakBar"}
    libdirs {os.findlib("glfw")}
    libdirs {os.findlib("GLU")}
    libdirs {os.findlib("GL")}
    --libdirs {os.findlib("AntTweakBar")}
  
  configuration "windows"
    defines { "GLFW_DLL" }
    links { "glew32", "glu32", "opengl32"}
    libdirs {os.findlib("glew32")}
    libdirs {os.findlib("glu32")}
    libdirs {os.findlib("opengl32")}
    
-- http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:download
--   AntTewakBar linkage
-- http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:howto

-- http://glew.sourceforge.net/install.html    
--   Remember to link your project with glew32.lib, glu32.lib, and opengl32.lib on Windows and libGLEW.so, libGLU.so, and libGL.so on Unix (-lGLEW -lGLU -lGL). 

-- http://www.glfw.org/docs/latest/build.html#build_link_win32
--   GLFW_DLL is necessary when using the GLFW DLL on Windows, in order to explain to the compiler that the GLFW functions will be coming from another executable. It has no function on other platforms. 
