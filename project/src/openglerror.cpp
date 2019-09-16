/*
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
*/

#include "openglerror.h"
#include <assert.h>

void printError(const char *file, int line) {
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		std::cout<< "OpenGL Error in line " << line << " of file " << file << ":"
				 << std::endl << errorName(error) << std::endl
				 << errorDescription(error) << std::endl;
	}
	//assert(error == GL_NO_ERROR);
}

std::string errorDescription(GLenum error)
{
	switch(error){
	case GL_NO_ERROR:
		return std::string("No error has been recorded. "
						   "The value of this symbolic constant is guaranteed to be 0.");
		break;
	case GL_INVALID_ENUM:
		return std::string("An unacceptable value is specified for an enumerated argument."
						   "The offending command is ignored and has no other side effect "
						   "than to set the error flag.");
		break;
	case GL_INVALID_VALUE:
		return std::string("A numeric argument is out of range."
						   "The offending command is ignored and has no other side effect"
						   "than to set the error flag.");
		break;
	case GL_INVALID_OPERATION:
		return std::string("The specified operation is not allowed in the current state."
						   "The offending command is ignored and has no other side effect"
						   "than to set the error flag.");
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
			return std::string("The framebuffer object is not complete. The offending command"
							   " is ignored and has no other side effect than to set the error flag.");
		break;
	case GL_OUT_OF_MEMORY:
			return std::string("There is not enough memory left to execute the command."
							   "The state of the GL is undefined, except for the state "
							   "of the error flags after this error is recorded.");
		break;
	case GL_STACK_UNDERFLOW:
			return std::string("An attempt has been made to perform an operation that would"
							   "cause an internal stack to underflow.");
		break;
	case GL_STACK_OVERFLOW:
			return std::string("An attempt has been made to perform an operation that would"
							   "cause an internal stack to overflow.");
		break;
	default:
		return std::string("Unbekannter Fehler");
	}
}

std::string errorName(GLenum error)
{
	switch(error){
	case GL_NO_ERROR:
		return std::string("GL_NO_ERROR");
		break;
	case GL_INVALID_ENUM:
		return std::string("GL_INVALID_ENUM");
		break;
	case GL_INVALID_VALUE:
		return std::string("GL_INVALID_VALUE");
		break;
	case GL_INVALID_OPERATION:
		return std::string("GL_INVALID_OPERATION");
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
			return std::string("GL_INVALID_FRAMEBUFFER_OPERATION");
		break;
	case GL_OUT_OF_MEMORY:
			return std::string("GL_OUT_OF_MEMORY");
		break;
	case GL_STACK_UNDERFLOW:
			return std::string("GL_STACK_UNDERFLOW");
		break;
	case GL_STACK_OVERFLOW:
			return std::string("GL_STACK_OVERFLOW");
		break;
	default:
		return std::string("Unknown errorcode");
	}
}
