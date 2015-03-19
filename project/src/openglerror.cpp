#include "openglerror.h"
#include <assert.h>

void printError(const char *file, int line) {
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		std::cout<< "OpenGL Error in line " << line << " of file " << file << ":"
				 << std::endl << errorName(error) << std::endl
				 << errorDescription(error) << std::endl;
	}
	assert(error == GL_NO_ERROR);
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
